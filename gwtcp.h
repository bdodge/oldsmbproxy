#ifndef GWTCP_H_
#define GWTCP_H_

#include "butil.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef Windows
#define SOCKET_ERROR (-1)
#define closesocket close
#define ioctlsocket ioctl
#else
    typedef int socklen_t;
#endif

int tcp_listen_on_port(uint16_t port, socket_t *socket_ptr, int max_connections);
int tcp_accept_connection(socket_t serversock, socket_t *clientsock, int timeoutms);
int tcp_connect(const char *host, uint16_t port, socket_t *socket_ptr);
int tcp_write(socket_t sock, uint8_t *buffer, int count);
int tcp_read(socket_t sock, uint8_t *buffer, int count, int timeoutms);

#ifdef __cplusplus
}
#endif

#endif
