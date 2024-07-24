
#include "smbgw.h"
#include "gwtcp.h"
#include "smbproto.h"

#include <pthread.h>

static char s_server_host[MAX_SMB_URL];
static uint16_t s_server_port;

static smb_ctx_t *s_ctx;
static int        s_ctx_cnt;
static bool		  s_exit_threads;

static void osal_sleep(int secs, int usecs)
{
    struct timespec ts;
    int result;

    ts.tv_sec = secs;
    ts.tv_nsec = usecs * 1000;
    while (ts.tv_nsec > 1000000000)
    {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    do
    {
        result = nanosleep(&ts, &ts);
    }
    while (result == -1 && errno == EINTR);
}

static int smbgw_client_packet(smb_ctx_t *ctx, uint8_t *pkt, int bytes)
{
	return smbproto_packet(ctx, true, pkt, bytes);
}

static int smbgw_server_packet(smb_ctx_t *ctx, uint8_t *pkt, int bytes)
{
	return smbproto_packet(ctx, false, pkt, bytes);
}

static void *smbgw_thread(void *param)
{
	smb_ctx_t *ctx = (smb_ctx_t *)param;
	int result;
	int room;
	int chunk;
	uint8_t *ioptr;

	enum { psHeader, psBody, psPacket } clientPacketState, serverPacketState;
	int clientPacketRemain;
	int serverPacketRemain;

	while (!s_exit_threads)
	{
		// wait for a client socket to hanle
		//
		while (ctx->client_socket == INVALID_SOCKET)
		{
			osal_sleep(0, 10000);
		}

		if (ctx->client_socket != INVALID_SOCKET)
		{
			butil_log(2, "X[%d] Got client socket\n", ctx->ordinal);

			// open a connection to the server
			//
			result = tcp_connect(s_server_host, s_server_port, &ctx->server_socket);
			if (result)
			{
				butil_log(0, "X[%d] Can't connect to server, closing client\n", ctx->ordinal);
				closesocket(ctx->client_socket);
				ctx->client_socket = INVALID_SOCKET;
				continue;
			}

			if (ctx->server_socket == INVALID_SOCKET)
			{
				butil_log(0, "X[%d] Can't connect to server, closing client\n", ctx->ordinal);
				closesocket(ctx->client_socket);
				ctx->client_socket = INVALID_SOCKET;
				continue;
			}

			butil_log(2, "X[%d] Connected to server\n", ctx->ordinal);

			// tcp proxy loop.
			//
			result = 0;

			// [MS-SMB] 2.1 - each SMB message must have a 4 byte direct-TCP header prepending (1-byte 0, 3-bytes length)
			//
			clientPacketRemain = 4;
			clientPacketState = psHeader;

			serverPacketRemain = 4;
			serverPacketState = psHeader;

			do
			{
				// read any bytes up till packet remaining bytes from client
				//
				room = butil_ring_contiguous_write_ptr(&ctx->client_ring, &ioptr);
				chunk = clientPacketRemain - ctx->client_ring.count;

				// sanity - insist room > chunk
				if (room < chunk)
				{
					butil_log(0, "Internal Error: client buffer size\n");
					result = -1;
					break;
				}
				result = tcp_read(ctx->client_socket, ioptr, chunk, 2);
				if (result > 0)
				{
					butil_log(6, "C[%d] Read %d of %d\n", ctx->ordinal, result, chunk);
					ctx->client_ring.count += result;
					ctx->client_ring.head += result;
					if (ctx->client_ring.head >= ctx->client_ring.size)
					{
						ctx->client_ring.head = 0;
					}

					result = 0;
				}

				if (result)
				{
					butil_log(1, "C[%d] Read Failed\n", ctx->ordinal);
					break;
				}

				if (ctx->client_ring.count == clientPacketRemain)
				{
					if (clientPacketState == psHeader)
					{
						uint32_t packetHeader;
						uint32_t temp;

						memcpy(&temp, ioptr, sizeof(uint32_t));
						packetHeader = ntohl(temp);

						if (packetHeader > 0x1FFFFF)
						{
							butil_log(0, "Invslid TCP packet header\n");
							result = -1;
							break;
						}

						clientPacketState = psBody;
						clientPacketRemain = packetHeader + 4;

						butil_log(6, "C[%d] SMB want %d bytes\n", ctx->ordinal, packetHeader);
					}
					else /* clientPacketState == psBody, and body is all read */
					{
						// make sure packet is fully contiguous
						butil_normalize_ring(&ctx->client_ring, NULL);

						// get a pointer to tcp header + packet
						chunk = butil_ring_contiguous_read_ptr(&ctx->client_ring, &ioptr);

						chunk -= sizeof(uint32_t);
						ioptr += sizeof(uint32_t);

						if (!ctx->passthrough)
						{
							// Validate/Process SMB message
							result = smbgw_client_packet(ctx, ioptr, chunk);
							if (result)
							{
								break;
							}
						}
						clientPacketState = psHeader;
						clientPacketRemain = 4;

						// write header and body to server
						//
						chunk = butil_ring_contiguous_read_ptr(&ctx->client_ring, &ioptr);
						if (chunk > 0)
						{
							result = tcp_write(ctx->server_socket, ioptr, chunk);
							if (result >= 0)
							{
								butil_log(6, "C[%d] Write %d\n", ctx->ordinal, result);
								ctx->client_ring.count -= result;
								ctx->client_ring.tail += result;
								if (ctx->client_ring.tail >= ctx->client_ring.size)
								{
									ctx->client_ring.tail = 0;
								}

								// reset ring if empty, to help remain more contiguous
								if (ctx->client_ring.count == 0)
								{
									butil_reset_ring(&ctx->client_ring);
								}

								result = 0;
							}

							if (result)
							{
								butil_log(1, "C[%d] Write Failed\n", ctx->ordinal);
								break;
							}
						}
					}
				}

				// read any bytes up till packet remaining bytes from server
				//
				room = butil_ring_contiguous_write_ptr(&ctx->server_ring, &ioptr);
				chunk = serverPacketRemain - ctx->server_ring.count;

				// sanity - insist room > chunk
				if (room < chunk)
				{
					butil_log(0, "Internal Error: server buffer size\n");
					result = -1;
					break;
				}
				result = tcp_read(ctx->server_socket, ioptr, chunk, 0);
				if (result > 0)
				{
					butil_log(6, "S[%d] Read %d of %d\n", ctx->ordinal, result, chunk);
					ctx->server_ring.count += result;
					ctx->server_ring.head += result;
					if (ctx->server_ring.head >= ctx->server_ring.size)
					{
						ctx->server_ring.head = 0;
					}

					result = 0;
				}

				if (result)
				{
					butil_log(1, "S[%d] Read Failed\n", ctx->ordinal);
					break;
				}

				if (ctx->server_ring.count == serverPacketRemain)
				{
					if (serverPacketState == psHeader)
					{
						uint32_t packetHeader;
						uint32_t temp;

						memcpy(&temp, ioptr, sizeof(uint32_t));
						packetHeader = ntohl(temp);

						if (packetHeader > 0x1FFFFF)
						{
							butil_log(0, "Invslid TCP packet header\n");
							result = -1;
							break;
						}

						serverPacketState = psBody;
						serverPacketRemain = packetHeader + 4;

						butil_log(6, "S[%d] SMB want %d bytes\n", ctx->ordinal, packetHeader);
					}
					else /* serverPacketState == psBody, and body is all read */
					{
						// make sure packet is fully contiguous
						butil_normalize_ring(&ctx->server_ring, NULL);

						// get a pointer to tcp header + packet
						chunk = butil_ring_contiguous_read_ptr(&ctx->server_ring, &ioptr);

						chunk -= sizeof(uint32_t);
						ioptr += sizeof(uint32_t);

						// Validate/Process SMB message
						if (!ctx->passthrough)
						{
							result = smbgw_server_packet(ctx, ioptr, chunk);
							if (result)
							{
								break;
							}
						}
						serverPacketState = psHeader;
						serverPacketRemain = 4;

						// write header and body to client
						//
						chunk = butil_ring_contiguous_read_ptr(&ctx->server_ring, &ioptr);
						if (chunk > 0)
						{
							result = tcp_write(ctx->client_socket, ioptr, chunk);
							if (result >= 0)
							{
								butil_log(6, "S[%d] Write %d\n", ctx->ordinal, result);
								ctx->server_ring.count -= result;
								ctx->server_ring.tail += result;
								if (ctx->server_ring.tail >= ctx->server_ring.size)
								{
									ctx->server_ring.tail = 0;
								}

								// reset ring if empty, to help remain more contiguous
								if (ctx->server_ring.count == 0)
								{
									butil_reset_ring(&ctx->server_ring);
								}

								result = 0;
							}

							if (result)
							{
								butil_log(1, "S[%d] Write Failed\n", ctx->ordinal);
								break;
							}
						}
					}
				}
			}
			while (!result);

			butil_log(2, "X[%d] Client connection done\n", ctx->ordinal);

			if (ctx->client_socket != INVALID_SOCKET)
			{
				closesocket(ctx->client_socket);
				ctx->client_socket = INVALID_SOCKET;
			}

			if (ctx->server_socket != INVALID_SOCKET)
			{
				closesocket(ctx->server_socket);
				ctx->server_socket = INVALID_SOCKET;
			}
		}
	}

	return 0;
}

int smbgw_proxy_init(
					const char *server_host,
					uint16_t    server_port,
					int max_connections,
					int iobuffer_size
					)
{
	int i;
	bool allset = true;

	if (!server_host)
	{
		return -1;
	}

	strncpy(s_server_host, server_host, sizeof(s_server_host) - 1);
	s_server_host[sizeof(s_server_host) - 1] = '\0';
	s_server_port = server_port;

	s_ctx = (smb_ctx_t *)malloc(sizeof(smb_ctx_t) * max_connections);
	if (! s_ctx)
	{
		butil_log(0, "Out of memory: can't alloc connection context\n");
		return -1;
	}

	memset(s_ctx, 0, sizeof(smb_ctx_t) * max_connections);

	for (i = 0; i < max_connections; i++)
	{
		s_ctx[i].ordinal = i;
		s_ctx[i].client_socket = INVALID_SOCKET;
		s_ctx[i].server_socket = INVALID_SOCKET;

		s_ctx[i].client_ring.size = iobuffer_size;
		s_ctx[i].client_ring.data = (uint8_t*)malloc(iobuffer_size);
		if (!s_ctx[i].client_ring.data)
		{
			butil_log(0, "Out of memory: can't alloc client buffer\n");
			allset = false;
			break;
		}
		butil_reset_ring(&s_ctx[i].client_ring);

		s_ctx[i].server_ring.size = iobuffer_size;
		s_ctx[i].server_ring.data = (uint8_t*)malloc(iobuffer_size);
		if (!s_ctx[i].server_ring.data)
		{
			butil_log(0, "Out of memory: can't alloc server buffer\n");
			allset = false;
			break;
		}
		butil_reset_ring(&s_ctx[i].server_ring);
	}

	if (!allset)
	{
		for (i = 0; i < max_connections; i++)
		{
			if (s_ctx[i].client_ring.data)
			{
				free(s_ctx[i].client_ring.data);
			}

			if (s_ctx[i].server_ring.data)
			{
				free(s_ctx[i].server_ring.data);
			}
		}

		free(s_ctx);
		s_ctx_cnt = 0;

		return -1;
	}

	s_ctx_cnt = max_connections;

	return 0;
}

int smbgw_proxy_start(socket_t client, bool passthrough)
{
	int result;
	int i;

	if (client == INVALID_SOCKET)
	{
		return -1;
	}

	butil_log(2, "New client %d\n", client);

	// find a context to use to handle this connection
	//
	for (i = 0; i < s_ctx_cnt; i++)
	{
		if (s_ctx[i].client_socket == INVALID_SOCKET)
		{
			if (!s_ctx[i].thread)
			{
				result = pthread_create(&s_ctx[i].thread, NULL, smbgw_thread, &s_ctx[i]);
				if (result)
				{
					butil_log(0, "Can't create thread for connection\n");
					return -1;
				}
			}

			butil_reset_ring(&s_ctx[i].client_ring);
			butil_reset_ring(&s_ctx[i].server_ring);

			s_ctx[i].client_socket = client;
			s_ctx[i].passthrough = passthrough;

			break;
		}
	}

	if (i >= s_ctx_cnt)
	{
		butil_log(0, "Can't find context to handle connection, internal error\n");
		closesocket(client);
		return -1;
	}

	return 0;
}

int smbgw_proxy_exit(void)
{
	return 0;
}

