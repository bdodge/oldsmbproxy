#ifndef SMBGW_H
#define SMBGW_H 1

#include "butil.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SMB_URL (256)

int smbgw_proxy_init(
					const char *server_host,
					uint16_t    server_port,
					int max_connections,
					int iobuffer_size
					);
int smbgw_proxy_start(socket_t client, bool passthrough);
int smbgw_proxy_exit(void);

#ifdef __cplusplus
}
#endif

#endif
