
#include "butil.h"
#include "gwtcp.h"
#include "smbgw.h"

static int useage(const char *progname)
{
	fprintf(stderr, "Usage: %s [-p local-port] [-l log-level] [-x] smb-server-url\n", progname);
	return -1;
}

int main(int argc, char **argv)
{
	const char *progname;
	char *arg;
	char server_url[MAX_SMB_URL];
	char server_host[MAX_SMB_URL];
	butil_url_scheme_t server_scheme;
	char server_path[MAX_SMB_URL];
	uint16_t server_port;
	uint16_t local_port;
	socket_t server_socket;
	socket_t client_socket;
	int max_connections;
	int iobuffer_size;
	int result;
	int loglevel;
	bool passthrough;

	server_port = 445;
	local_port = 4445;

	passthrough = false;

	progname = *argv++;
	argc--;
	
	server_url[0] = '\0';
    result = 0;

	loglevel = 5;
	max_connections = 4;

	// SMB TCP wrapper has 3 byte length, and limit in size to 1FFFF payload
	iobuffer_size = 0x20008;

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
		
        if (arg[0] == '-')
        {
            int argi = 1;
			
			do
			{
	            switch (arg[1])
	            {
	            case 'p':
					argi++;
					if (!arg[argi])
					{
		                if (argc > 0)
		                {
							argi = 0;
		                    arg = *argv++;
		                    argc--;
		                }
		                else
		                {
							fprintf(stderr, "Use: -p [port]");
							result = 1;
		                }
					}
	                local_port = strtoul(arg + argi, &arg, 0);
					argi = 0;
	                break;
	            case 'l':
					argi++;
					if (!arg[argi])
					{
		                if (argc > 0)
		                {
							argi = 0;
		                    arg = *argv++;
		                    argc--;
		                }
		                else
		                {
							fprintf(stderr, "Use: -l [level]");
		                }
					}
	                loglevel = strtoul(arg + argi, &arg, 0);
					argi = 0;
	                break;
	            case 'x':
	                passthrough = true;
	                break;
	            default:
					fprintf(stderr, "Bad switch: %s\n", arg);
					result = 1;
	            }
			}
			while (!result && arg && arg[argi]);
        }
        else
        {
            strncpy(server_url, arg, sizeof(server_url) - 1);
            server_url[sizeof(server_url) - 1] = '\0';
        }
    }

	if (result || (server_url[0] == '\0'))
	{
		return useage(progname);
	}

	butil_set_log_level(loglevel);

	// split server url into host/port/path
	//
	result = butil_parse_url(
							server_url,
							&server_scheme,
							server_host,
							sizeof(server_host),
							&server_port,
							server_path,
							sizeof(server_path)
							);
	if (result)
	{
		fprintf(stderr, "Bad server URL: %s\n", server_url);
		return -1;
	}

	// create the connection handling thread pool
	//
	result = smbgw_proxy_init(
							server_host,
							server_port,
							max_connections,
							iobuffer_size
							);
	if (result)
	{
		return -1;
	}

	// open a server socket to server smb connections
	//
	result = tcp_listen_on_port(local_port, &server_socket, max_connections);
	if (result)
	{
		butil_log(0, "Can't listen on port %u\n", local_port);
		return -1;
	}

	butil_log(1, "Listening on port %u\n", local_port);

	// accept connections and hand them off to a handling thread
	//
	do
	{
		result = tcp_accept_connection(server_socket, &client_socket, 10000);
		if (!result)
		{
			if (client_socket != INVALID_SOCKET)
			{
				// got a live one
				//
				result = smbgw_proxy_start(client_socket, passthrough);
				if (result)
				{
					butil_log(0, "Can't proxy connection\n");
				}
			}
		}
	}
	while (!result);

	return 0;
}

