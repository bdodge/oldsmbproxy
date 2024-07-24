#ifndef SMBCONTEXT_H
#define SMBCONTEXT_H

#include "butil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_smb_connection
{
	socket_t	client_socket;
	socket_t	server_socket;

	ioring_t	client_ring;
	ioring_t	server_ring;

	bool		passthrough;
	int			ordinal;

	pthread_t	thread;
}
smb_ctx_t;


#ifdef __cplusplus
}
#endif

#endif
