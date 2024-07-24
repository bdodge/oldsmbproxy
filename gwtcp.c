
#include "gwtcp.h"

int tcp_listen_on_port(uint16_t port, socket_t *socket_ptr, int max_connections)
{
    struct sockaddr_in serv_addr;
    #ifdef Windows
    unsigned long nonblock;
    #else
    uint32_t nonblock;
    #endif
    int enable;
    int result;
    socket_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        butil_log(0, "Can't create server socket\n");
        return INVALID_SOCKET;
    }

    enable = 1;
    result = setsockopt(
                        sock,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        (char*)&enable,
                        sizeof(enable)
                      );
    if (result < 0)
    {
        return result;
    }
#ifndef VPNX_GUI
    nonblock = 0;
#else
    // gui wants to control the threading for cancel events so
    // don't block (much) here
    nonblock = 1;
#endif
    nonblock = 1;
    result = ioctlsocket(sock, FIONBIO, &nonblock);
    if (result < 0)
    {
        return result;
    }
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    result = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (result < 0)
    {
        butil_log(0, "Can't bind server socket to port %u\n", port);
        closesocket(sock);
        return result;
    }
    result = listen(sock, max_connections);
    if (result < 0)
    {
        closesocket(sock);
        butil_log(0, "Can't listen on port\n");
    }
    butil_log(3, "Listening on port %u for TCP\n", port);
    *socket_ptr = sock;
    return result;
}

int tcp_accept_connection(socket_t serversock, socket_t *clientsock_ptr, int timeoutms)
{
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    socket_t clientsock;
    fd_set  afds;
    struct  timeval timeout;
    int sv;

    *clientsock_ptr = INVALID_SOCKET;

    butil_log(6, "Accepting connections now\n");

    // select for connections to use non-blocking sockets. there is nothing to do
    // as a server while waiting for a connection, so no reason to not wait long but
    // then we could miss a usb message, and/or make a GUI non-responsive, so dont
    // wait too long here
    //
    FD_ZERO (&afds);
    FD_SET  (serversock, &afds);

    timeout.tv_sec  = timeoutms / 1000;
    timeout.tv_usec = (timeoutms - ((timeoutms / 1000) * 1000)) / 1000;

    sv = select(serversock + 1, &afds, NULL, NULL, &timeout);

    clientsock = INVALID_SOCKET;
    if (sv > 0)
    {
        clilen = sizeof(cli_addr);
        clientsock = accept(serversock, (struct sockaddr *)&cli_addr, &clilen);
    }
    if (clientsock != INVALID_SOCKET)
    {
        #ifdef Windows
        unsigned long nonblock;
        #else
        uint32_t nonblock;
        #endif

        butil_log(5, "Connection: socket=%d\n", clientsock);

        nonblock = 1;
        if (ioctlsocket(clientsock, FIONBIO, &nonblock) < 0)
        {
            butil_log(0, "Can't make client socket non-blocking\n");
            closesocket(clientsock);
            return -1;
        }
		*clientsock_ptr = clientsock;
	    return 0;
    }
    else if (sv > 0)
    {
        butil_log(0, "Accept failed\n");
        return -1;
    }
    else if (sv < 0)
    {
        butil_log(0, "Server socket failed\n");
        closesocket(serversock);
        serversock = INVALID_SOCKET;
        return -1;
    }
    return 0;
}

int tcp_connect(const char *host, uint16_t port, socket_t *socket_ptr)
{
    struct sockaddr_in serv_addr;
    socket_t sock;
    #ifdef Windows
    unsigned long nonblock;
    #else
    uint32_t nonblock;
    #endif
    int result;
    bool isname;
    int hostlen;
    int i;

	*socket_ptr = INVALID_SOCKET;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        butil_log(0, "Can't create socket\n");
        return -1;
    }
	#if 1
    nonblock = 1;
    if (ioctlsocket(sock, FIONBIO, &nonblock) < 0)
    {
        butil_log(0, "Can't make nonblocking");
        closesocket(sock);
        return -1;
    }
	#endif

    // if host is an IP address, use directly
	//
    for (i = 0, isname = false, hostlen = (int)strlen(host); i < hostlen; i++)
    {
        if ((host[i] < '0' || host[i] > '9') && host[i] != '.')
        {
            isname = true;
            break;
        }
    }
    if (isname)
    {
        struct hostent *hostname = gethostbyname(host);

        if (! hostname)
        {
            butil_log(0, "Can't find address %s\n", host);
            closesocket(sock);
            return -1;
        }
        memcpy(&serv_addr.sin_addr, hostname->h_addr, hostname->h_length);
    }
    else
    {
        if (! inet_pton(AF_INET, host, &serv_addr.sin_addr))
        {
            butil_log(0, "Invalid address %s\n", host);
            closesocket(sock);
            return -1;
        }
    }
    // connect to remote server
    //
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

	butil_log(3, "Connecting to %s:%u\n", host, port);

    result = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (result < 0)
    {
        // this is non blocking, so expect error.
        //
        #ifdef windows
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        #else
        if (errno == EWOULDBLOCK || errno == EINPROGRESS)
        #endif
        {
            result = 0;
        }
        else
        {
            butil_log(0, "Can't connect to remote host\n");
            closesocket(sock);
            return -1;
        }
    }
	*socket_ptr = sock;
	return 0;
}


int tcp_write(socket_t sock, uint8_t *buffer, int count)
{
    int sent;
    int wc, sv;
    fd_set  wfds;
    struct  timeval timeout;

    int waitms = 15000;

    sent = 0;
    do
    {
        FD_ZERO (&wfds);
        FD_SET  (sock, &wfds);

        timeout.tv_sec  = waitms / 1000;
        timeout.tv_usec = (waitms - ((waitms / 1000) * 1000)) * 1000;

        sv = select(sock + 1, NULL, &wfds, NULL, &timeout);
        if (sv < 0)
        {
            butil_log(0, "TCP connection broken\n");
            return -1;
        }
        if (sv == 0)
        {
            butil_log(0, "TCP connection blocked for write\n");
            return -1;
        }
        wc = (int)send(sock, (char*)buffer + sent, (size_t)(count - sent), 0);
        if (wc < 0)
        {
#ifdef Windows
            butil_log(1, "TCP write err=%d\n", WSAGetLastError());
#else
            butil_log(1, "TCP write err=%d\n", errno);
#endif
            return wc;
        }
        if (wc == 0)
        {
            butil_log(2, "TCP connection closed on write\n");
            return -1;
        }
        sent += wc;
    }
    while (sent < count);

    return count;
}

int tcp_read(socket_t sock, uint8_t *buffer, int count, int timeoutms)
{
    int rc, sv;
    fd_set  rfds;
    struct  timeval timeout;

    FD_ZERO (&rfds);
    FD_SET  (sock, &rfds);

    timeout.tv_sec  = timeoutms / 1000;
    timeout.tv_usec = (timeoutms - ((timeoutms / 1000) * 1000)) * 1000;

    sv = select(sock + 1, &rfds, NULL, NULL, &timeout);
    if (sv < 0)
    {
        butil_log(0, "TCP connection broke\n");
        return -1;
    }
    if (sv == 0)
    {
        // no data available
        return 0;
    }
    rc = (int)recv(sock, (char*)buffer, (size_t)count, 0);
    if (rc < 0)
    {
#ifdef Windows
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return 0;
		}
#else
		if (errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return 0;
		}
#endif
		// 0 count after select > 0 means socket closed
		butil_log(1, "TCP connection recv: %d\n", errno);
        return rc;
    }
    if (rc == 0)
    {
        butil_log(2, "TCP connection closed on read\n");
        return -1;
    }
    return rc;
}

