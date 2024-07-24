#ifndef SMBPROTO_H
#define SMBPROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smbcontext.h"

#include <stdint.h>
#include <stdbool.h>

/// SMB Commands

#define SMB_COM_CREATE_DIRECTORY              (0x00)
#define SMB_COM_DELETE_DIRECTORY              (0x01)
#define SMB_COM_OPEN                          (0x02)
#define SMB_COM_CREATE                        (0x03)
#define SMB_COM_CLOSE                         (0x04)
#define SMB_COM_FLUSH                         (0x05)
#define SMB_COM_DELETE                        (0x06)
#define SMB_COM_RENAME                        (0x07)
#define SMB_COM_QUERY_INFORMATION             (0x08)
#define SMB_COM_SET_INFORMATION               (0x09)
#define SMB_COM_READ                          (0x0A)
#define SMB_COM_WRITE                         (0x0B)
#define SMB_COM_LOCK_BYTE_RANGE               (0x0C)
#define SMB_COM_UNLOCK_BYTE_RANGE             (0x0D)
#define SMB_COM_CREATE_TEMPORARY              (0x0E)
#define SMB_COM_CREATE_NEW                    (0x0F)
#define SMB_COM_CHECK_DIRECTORY               (0x10)
#define SMB_COM_PROCESS_EXIT                  (0x11)
#define SMB_COM_SEEK                          (0x12)
#define SMB_COM_LOCK_AND_READ                 (0x13)
#define SMB_COM_WRITE_AND_UNLOCK              (0x14)
#define SMB_COM_READ_RAW                      (0x1A)
#define SMB_COM_READ_MPX                      (0x1B)
#define SMB_COM_READ_MPX_SECONDARY            (0x1C)
#define SMB_COM_WRITE_RAW                     (0x1D)
#define SMB_COM_WRITE_MPX                     (0x1E)
#define SMB_COM_WRITE_MPX_SECONDARY           (0x1F)
#define SMB_COM_WRITE_COMPLETE                (0x20)
#define SMB_COM_QUERY_SERVER                  (0x21)
#define SMB_COM_SET_INFORMATION2              (0x22)
#define SMB_COM_QUERY_INFORMATION2            (0x23)
#define SMB_COM_LOCKING_ANDX                  (0x24)
#define SMB_COM_TRANSACTION                   (0x25)
#define SMB_COM_TRANSACTION_SECONDARY         (0x26)
#define SMB_COM_IOCTL                         (0x27)
#define SMB_COM_IOCTL_SECONDARY               (0x28)
#define SMB_COM_COPY                          (0x29)
#define SMB_COM_MOVE                          (0x2A)
#define SMB_COM_ECHO                          (0x2B)
#define SMB_COM_WRITE_AND_CLOSE               (0x2C)
#define SMB_COM_OPEN_ANDX                     (0x2D)
#define SMB_COM_READ_ANDX                     (0x2E)
#define SMB_COM_WRITE_ANDX                    (0x2F)
#define SMB_COM_NEW_FILE_SIZE                 (0x30)
#define SMB_COM_CLOSE_AND_TREE_DISC           (0x31)
#define SMB_COM_TRANSACTION2                  (0x32)
#define SMB_COM_TRANSACTION2_SECONDARY        (0x33)
#define SMB_COM_FIND_CLOSE2                   (0x34)
#define SMB_COM_FIND_NOTIFY_CLOSE             (0x35)
#define SMB_COM_TREE_CONNECT                  (0x70)
#define SMB_COM_TREE_DISCONNECT               (0x71)
#define SMB_COM_NEGOTIATE                     (0x72)
#define SMB_COM_SESSION_SETUP_ANDX            (0x73)
#define SMB_COM_LOGOFF_ANDX                   (0x74)
#define SMB_COM_TREE_CONNECT_ANDX             (0x75)
#define SMB_COM_QUERY_INFORMATION_DISK        (0x80)
#define SMB_COM_SEARCH                        (0x81)
#define SMB_COM_FIND                          (0x82)
#define SMB_COM_FIND_UNIQUE                   (0x83)
#define SMB_COM_FIND_CLOSE                    (0x84)
#define SMB_COM_NT_TRANSACT                   (0xA0)
#define SMB_COM_NT_TRANSACT_SECONDARY         (0xA1)
#define SMB_COM_NT_CREATE_ANDX                (0xA2)
#define SMB_COM_NT_CANCEL                     (0xA4)
#define SMB_COM_NT_RENAME                     (0xA5)
#define SMB_COM_OPEN_PRINT_FILE               (0xC0)
#define SMB_COM_WRITE_PRINT_FILE              (0xC1)
#define SMB_COM_CLOSE_PRINT_FILE              (0xC2)
#define SMB_COM_GET_PRINT_QUEUE               (0xC3)
#define SMB_COM_READ_BULK                     (0xD8)
#define SMB_COM_WRITE_BULK                    (0xD9)
#define SMB_COM_WRITE_BULK_DATA               (0xDA)

/// FLAGS field bitmasks.

#define SMB_FLAGS_SERVER_TO_REDIR       0x80
#define SMB_FLAGS_REQUEST_BATCH_OPLOCK  0x40
#define SMB_FLAGS_REQUEST_OPLOCK        0x20
#define SMB_FLAGS_CANONICAL_PATHNAMES   0x10
#define SMB_FLAGS_CASELESS_PATHNAMES    0x08
#define SMB_FLAGS_RESERVED              0x04
#define SMB_FLAGS_CLIENT_BUF_AVAIL      0x02
#define SMB_FLAGS_SUPPORT_LOCKREAD      0x01
#define SMB_FLAGS_MASK                  0xFB

/// FLAGS2 field bitmasks.

#define SMB_FLAGS2_UNICODE_STRINGS      0x8000
#define SMB_FLAGS2_32BIT_STATUS         0x4000
#define SMB_FLAGS2_READ_IF_EXECUTE      0x2000
#define SMB_FLAGS2_DFS_PATHNAME         0x1000
#define SMB_FLAGS2_EXTENDED_SECURITY    0x0800
#define SMB_FLAGS2_RESERVED_01          0x0400
#define SMB_FLAGS2_RESERVED_02          0x0200
#define SMB_FLAGS2_RESERVED_03          0x0100
#define SMB_FLAGS2_RESERVED_04          0x0080
#define SMB_FLAGS2_IS_LONG_NAME         0x0040
#define SMB_FLAGS2_RESERVED_05          0x0020
#define SMB_FLAGS2_RESERVED_06          0x0010
#define SMB_FLAGS2_RESERVED_07          0x0008
#define SMB_FLAGS2_SECURITY_SIGNATURE   0x0004
#define SMB_FLAGS2_EAS                  0x0002
#define SMB_FLAGS2_KNOWS_LONG_NAMES     0x0001
#define SMB_FLAGS2_MASK                 0xF847

/// SMB2 Command codes we understand

#define SMB2_NEGOTIATE                 	     (0)
#define SMB2_SESSION_SETUP                   (1)
#define SMB2_LOGOFF                          (2)
#define SMB2_TREE_CONNECT                    (3)
#define SMB2_TREE_DISCONNECT                 (4)
#define SMB2_CREATE                          (5)
#define SMB2_CLOSE                           (6)
#define SMB2_FLUSH                           (7)
#define SMB2_READ                            (8)
#define SMB2_WRITE                           (9)
#define SMB2_LOCK                            (10)
#define SMB2_IOCTL                           (11)
#define SMB2_CANCEL                          (12)
#define SMB2_ECHO                            (13)
#define SMB2_QUERY_DIRECTORY                 (14)
#define SMB2_CHANGE_NOTIFY                   (15)
#define SMB2_QUERY_INFO                      (16)
#define SMB2_SET_INFO                        (17)
#define SMB2_OPLOCK_BREAK                    (18)

/// SMB header structure

struct smb_header
{
	uint8_t     protocol_id[4];
	uint8_t     command;
	uint32_t    status;
	uint8_t     flags;
	uint16_t    flags2;
	uint16_t    PIDhigh;
	uint8_t     securityFeatures[8];
	uint16_t    _reserved_;
	uint16_t    TID;
	uint16_t    PIDlow;
	uint16_t    UID;
	uint16_t    MID;
} __attribute__((packed));

struct smb2_async
{
	uint64_t async_id;
} __attribute__((packed));

struct smb2_sync
{
	uint32_t process_id;
	uint32_t tree_id;
} __attribute__((packed));

struct smb2_header
{
	uint8_t protocol_id[4];
	uint16_t struct_size;
	uint16_t credit_charge;
	uint32_t status;
	uint16_t command;
	uint16_t credit_request_response;
	uint32_t flags;
	uint32_t next_command;
	uint64_t message_id;
	union {
		struct smb2_async async;
		struct smb2_sync sync;
	};
	uint64_t session_id;
	uint8_t signature[16];
} __attribute__((packed));

struct smb_session_nonext_setup_request_params
{
	uint16_t max_buffer_size;
	uint16_t max_mpx_count;
	uint16_t vc_number;
	uint32_t session_key;
	uint16_t length1;
	uint16_t length2;
	uint32_t reserved;
	uint32_t caps;
} __attribute__((packed));

struct smb_session_ext_setup_request_params
{
	uint16_t max_buffer_size;
	uint16_t max_mpx_count;
	uint16_t vc_number;
	uint32_t session_key;
	uint16_t length1;
	uint32_t reserved;
	uint32_t caps;
} __attribute__((packed));

const char *smbproto_smb_command_name(uint8_t command);
const char *smbproto_smb2_command_name(uint16_t command);

int smbproto_packet(smb_ctx_t *ctx, bool isclient, uint8_t *pkt, int bytes);

#ifdef __cplusplus
}
#endif

#endif
