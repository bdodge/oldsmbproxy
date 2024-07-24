#include "smbproto.h"
#include "smbgw.h"

const char *smbproto_smb_command_name(uint8_t command)
{
	switch (command)
	{
	case SMB_COM_CREATE_DIRECTORY:	return "COM_CREATE_DIRECTORY";
	case SMB_COM_DELETE_DIRECTORY:	return "COM_DELETE_DIRECTORY";
	case SMB_COM_OPEN:	return "COM_OPEN";
	case SMB_COM_CREATE:	return "COM_CREATE";
	case SMB_COM_CLOSE:	return "COM_CLOSE";
	case SMB_COM_FLUSH:	return "COM_FLUSH";
	case SMB_COM_DELETE:	return "COM_DELETE";
	case SMB_COM_RENAME:	return "COM_RENAME";
	case SMB_COM_QUERY_INFORMATION:	return "COM_QUERY_INFORMATION";
	case SMB_COM_SET_INFORMATION:	return "COM_SET_INFORMATION";
	case SMB_COM_READ:	return "COM_READ";
	case SMB_COM_WRITE:	return "COM_WRITE";
	case SMB_COM_LOCK_BYTE_RANGE:	return "COM_LOCK_BYTE_RANGE";
	case SMB_COM_UNLOCK_BYTE_RANGE:	return "COM_UNLOCK_BYTE_RANGE";
	case SMB_COM_CREATE_TEMPORARY:	return "COM_CREATE_TEMPORARY";
	case SMB_COM_CREATE_NEW:	return "COM_CREATE_NEW";
	case SMB_COM_CHECK_DIRECTORY:	return "COM_CHECK_DIRECTORY";
	case SMB_COM_PROCESS_EXIT:	return "COM_PROCESS_EXIT";
	case SMB_COM_SEEK:	return "COM_SEEK";
	case SMB_COM_LOCK_AND_READ:	return "COM_LOCK_AND_READ";
	case SMB_COM_WRITE_AND_UNLOCK:	return "COM_WRITE_AND_UNLOCK";
	case SMB_COM_READ_RAW:	return "COM_READ_RAW";
	case SMB_COM_READ_MPX:	return "COM_READ_MPX";
	case SMB_COM_READ_MPX_SECONDARY:	return "COM_READ_MPX_SECONDARY";
	case SMB_COM_WRITE_RAW:	return "COM_WRITE_RAW";
	case SMB_COM_WRITE_MPX:	return "COM_WRITE_MPX";
	case SMB_COM_WRITE_MPX_SECONDARY:	return "COM_WRITE_MPX_SECONDARY";
	case SMB_COM_WRITE_COMPLETE:	return "COM_WRITE_COMPLETE";
	case SMB_COM_QUERY_SERVER:	return "COM_QUERY_SERVER";
	case SMB_COM_SET_INFORMATION2:	return "COM_SET_INFORMATION2";
	case SMB_COM_QUERY_INFORMATION2:	return "COM_QUERY_INFORMATION2";
	case SMB_COM_LOCKING_ANDX:	return "COM_LOCKING_ANDX";
	case SMB_COM_TRANSACTION:	return "COM_TRANSACTION";
	case SMB_COM_TRANSACTION_SECONDARY:	return "COM_TRANSACTION_SECONDARY";
	case SMB_COM_IOCTL:	return "COM_IOCTL";
	case SMB_COM_IOCTL_SECONDARY:	return "COM_IOCTL_SECONDARY";
	case SMB_COM_COPY:	return "COM_COPY";
	case SMB_COM_MOVE:	return "COM_MOVE";
	case SMB_COM_ECHO:	return "COM_ECHO";
	case SMB_COM_WRITE_AND_CLOSE:	return "COM_WRITE_AND_CLOSE";
	case SMB_COM_OPEN_ANDX:	return "COM_OPEN_ANDX";
	case SMB_COM_READ_ANDX:	return "COM_READ_ANDX";
	case SMB_COM_WRITE_ANDX:	return "COM_WRITE_ANDX";
	case SMB_COM_NEW_FILE_SIZE:	return "COM_NEW_FILE_SIZE";
	case SMB_COM_CLOSE_AND_TREE_DISC:	return "COM_CLOSE_AND_TREE_DISC";
	case SMB_COM_TRANSACTION2:	return "COM_TRANSACTION2";
	case SMB_COM_TRANSACTION2_SECONDARY:	return "COM_TRANSACTION2_SECONDARY";
	case SMB_COM_FIND_CLOSE2:	return "COM_FIND_CLOSE2";
	case SMB_COM_FIND_NOTIFY_CLOSE:	return "COM_FIND_NOTIFY_CLOSE";
	case SMB_COM_TREE_CONNECT:	return "COM_TREE_CONNECT";
	case SMB_COM_TREE_DISCONNECT:	return "COM_TREE_DISCONNECT";
	case SMB_COM_NEGOTIATE:	return "COM_NEGOTIATE";
	case SMB_COM_SESSION_SETUP_ANDX:	return "COM_SESSION_SETUP_ANDX";
	case SMB_COM_LOGOFF_ANDX:	return "COM_LOGOFF_ANDX";
	case SMB_COM_TREE_CONNECT_ANDX:	return "COM_TREE_CONNECT_ANDX";
	case SMB_COM_QUERY_INFORMATION_DISK:	return "COM_QUERY_INFORMATION_DISK";
	case SMB_COM_SEARCH:	return "COM_SEARCH";
	case SMB_COM_FIND:	return "COM_FIND";
	case SMB_COM_FIND_UNIQUE:	return "COM_FIND_UNIQUE";
	case SMB_COM_FIND_CLOSE:	return "COM_FIND_CLOSE";
	case SMB_COM_NT_TRANSACT:	return "COM_NT_TRANSACT";
	case SMB_COM_NT_TRANSACT_SECONDARY:	return "COM_NT_TRANSACT_SECONDARY";
	case SMB_COM_NT_CREATE_ANDX:	return "COM_NT_CREATE_ANDX";
	case SMB_COM_NT_CANCEL:	return "COM_NT_CANCEL";
	case SMB_COM_NT_RENAME:	return "COM_NT_RENAME";
	case SMB_COM_OPEN_PRINT_FILE:	return "COM_OPEN_PRINT_FILE";
	case SMB_COM_WRITE_PRINT_FILE:	return "COM_WRITE_PRINT_FILE";
	case SMB_COM_CLOSE_PRINT_FILE:	return "COM_CLOSE_PRINT_FILE";
	case SMB_COM_GET_PRINT_QUEUE:	return "COM_GET_PRINT_QUEUE";
	case SMB_COM_READ_BULK:	return "COM_READ_BULK";
	case SMB_COM_WRITE_BULK:	return "COM_WRITE_BULK";
	case SMB_COM_WRITE_BULK_DATA:	return "COM_WRITE_BULK_DATA";
	default:	return "???";
	}
}

const bool smbproto_is_andx_command(uint8_t command)
{
	switch (command)
	{
	case SMB_COM_LOCKING_ANDX:
	case SMB_COM_OPEN_ANDX:
	case SMB_COM_READ_ANDX:
	case SMB_COM_WRITE_ANDX:
	case SMB_COM_SESSION_SETUP_ANDX:
	case SMB_COM_LOGOFF_ANDX:
	case SMB_COM_TREE_CONNECT_ANDX:
	case SMB_COM_NT_CREATE_ANDX:
		return true;
	default:
		return false;
	}
}

const char *smbproto_smb2_command_name(uint16_t command)
{
	switch (command)
	{
	case SMB2_NEGOTIATE:	return "NEGOTIATE";
	case SMB2_SESSION_SETUP:return "SESSION_SETUP";
	case SMB2_LOGOFF:		return "LOGOFF";
	case SMB2_TREE_CONNECT:	return "TREE_CONNECT";
	case SMB2_TREE_DISCONNECT:return "TREE_DISCONNECT";
	case SMB2_CREATE:		return "CREATE";
	case SMB2_CLOSE:		return "CLOSE";
	case SMB2_FLUSH:		return "FLUSH";
	case SMB2_READ:			return "READ";
	case SMB2_WRITE:		return "WRITE";
	case SMB2_LOCK:			return "LOCK";
	case SMB2_IOCTL:		return "IOCTL";
	case SMB2_CANCEL:		return "CANCEL";
	case SMB2_ECHO:			return "ECHO";
	case SMB2_QUERY_DIRECTORY:return "QUERY_DIRECTORY";
	case SMB2_CHANGE_NOTIFY:return "CHANGE_NOTIFY";
	case SMB2_QUERY_INFO:	return "QUERY_INFO";
	case SMB2_SET_INFO:		return "SET_INFO";
	case SMB2_OPLOCK_BREAK:	return "OPLOCK_BREAK";
	default: return "????";
	}
}

static int smbproto_session_setup_request(
									smb_ctx_t *ctx,
									uint8_t *params,
									int param_bytes,
									uint8_t *data,
									int data_bytes
									)
{
	struct smb_session_nonext_setup_request_params nsetup_params;
	struct smb_session_ext_setup_request_params    xsetup_params;
	//struct smb_session_setup_request_data          setup_data;
	char domain[128];
	char username[128];
	bool is_extended;
	
	if (param_bytes == sizeof(nsetup_params))
	{
		memcpy(&nsetup_params, params, sizeof(nsetup_params));
		is_extended = false;
	}
	else if (param_bytes == sizeof(xsetup_params))
	{
		memcpy(&xsetup_params, params, sizeof(xsetup_params));
		is_extended = true;		
	}
	else
	{
		butil_log(0, "Unknown session setup param block length\n");
		return -1;
	}
	
	return 0;
}

static int smbproto_session_setup_response(
									smb_ctx_t *ctx,
									uint8_t *param,
									int param_bytes,
									uint8_t *data,
									int data_bytes
									)
{
	return 0;
}

static int smbproto_handle_v1_command(
									smb_ctx_t *ctx,
									bool isclient,
									uint8_t command,
									struct smb_header *hdr,
									uint8_t *params,
									int param_bytes,
									uint8_t *data,
									int data_bytes
									)
{
	int result = 0;
	
	butil_log(4, "%s[%d] SMB bytes:%6d params:%u %s\n",
				isclient ? "C" : "S", ctx->ordinal,
				data_bytes, param_bytes, smbproto_smb_command_name(command));
				
	switch (command)
	{
	case SMB_COM_SESSION_SETUP_ANDX:
		if (isclient)
		{
			result = smbproto_session_setup_request(ctx, params, param_bytes, data, data_bytes);
		}
		else
		{
			result = smbproto_session_setup_response(ctx, params, param_bytes, data, data_bytes);
		}
		break;
	case SMB_COM_CREATE_DIRECTORY:
	case SMB_COM_DELETE_DIRECTORY:
	case SMB_COM_OPEN:
	case SMB_COM_CREATE:
	case SMB_COM_CLOSE:
	case SMB_COM_FLUSH:
	case SMB_COM_DELETE:
	case SMB_COM_RENAME:
	case SMB_COM_QUERY_INFORMATION:
	case SMB_COM_SET_INFORMATION:
	case SMB_COM_READ:
	case SMB_COM_WRITE:
	case SMB_COM_LOCK_BYTE_RANGE:
	case SMB_COM_UNLOCK_BYTE_RANGE:
	case SMB_COM_CREATE_TEMPORARY:
	case SMB_COM_CREATE_NEW:
	case SMB_COM_CHECK_DIRECTORY:
	case SMB_COM_PROCESS_EXIT:
	case SMB_COM_SEEK:
	case SMB_COM_LOCK_AND_READ:
	case SMB_COM_WRITE_AND_UNLOCK:
	case SMB_COM_READ_RAW:
	case SMB_COM_READ_MPX:
	case SMB_COM_READ_MPX_SECONDARY:
	case SMB_COM_WRITE_RAW:
	case SMB_COM_WRITE_MPX:
	case SMB_COM_WRITE_MPX_SECONDARY:
	case SMB_COM_WRITE_COMPLETE:
	case SMB_COM_QUERY_SERVER:
	case SMB_COM_SET_INFORMATION2:
	case SMB_COM_QUERY_INFORMATION2:
	case SMB_COM_LOCKING_ANDX:
	case SMB_COM_TRANSACTION:
	case SMB_COM_TRANSACTION_SECONDARY:
	case SMB_COM_IOCTL:
	case SMB_COM_IOCTL_SECONDARY:
	case SMB_COM_COPY:
	case SMB_COM_MOVE:
	case SMB_COM_ECHO:
	case SMB_COM_WRITE_AND_CLOSE:
	case SMB_COM_OPEN_ANDX:
	case SMB_COM_READ_ANDX:
	case SMB_COM_WRITE_ANDX:
	case SMB_COM_NEW_FILE_SIZE:
	case SMB_COM_CLOSE_AND_TREE_DISC:
	case SMB_COM_TRANSACTION2:
	case SMB_COM_TRANSACTION2_SECONDARY:
	case SMB_COM_FIND_CLOSE2:
	case SMB_COM_FIND_NOTIFY_CLOSE:
	case SMB_COM_TREE_CONNECT:
	case SMB_COM_TREE_DISCONNECT:
	case SMB_COM_NEGOTIATE:
	case SMB_COM_LOGOFF_ANDX:
	case SMB_COM_TREE_CONNECT_ANDX:
	case SMB_COM_QUERY_INFORMATION_DISK:
	case SMB_COM_SEARCH:
	case SMB_COM_FIND:
	case SMB_COM_FIND_UNIQUE:
	case SMB_COM_FIND_CLOSE:
	case SMB_COM_NT_TRANSACT:
	case SMB_COM_NT_TRANSACT_SECONDARY:
	case SMB_COM_NT_CREATE_ANDX:
	case SMB_COM_NT_CANCEL:
	case SMB_COM_NT_RENAME:
	case SMB_COM_OPEN_PRINT_FILE:
	case SMB_COM_WRITE_PRINT_FILE:
	case SMB_COM_CLOSE_PRINT_FILE:
	case SMB_COM_GET_PRINT_QUEUE:
	case SMB_COM_READ_BULK:
	case SMB_COM_WRITE_BULK:
	case SMB_COM_WRITE_BULK_DATA:
	default:
		break;
	}
	return 0;
}

static int smbproto_handle_v2_command(
									smb_ctx_t *ctx,
									bool isclient,
									uint8_t command,
									struct smb2_header *hdr,
									uint8_t *data,
									int data_bytes
									)
{
	int result = 0;
	
	butil_log(4, "%s[%d] SMB bytes:%6d next:%u %s\n",
				isclient ? "C" : "S", ctx->ordinal,
				data_bytes, hdr->next_command, smbproto_smb2_command_name(command));
				
	switch (command)
	{
	case SMB2_READ:
	case SMB2_WRITE:
	case SMB2_NEGOTIATE:
	case SMB2_SESSION_SETUP:
	case SMB2_LOGOFF:
	case SMB2_TREE_CONNECT:
	case SMB2_TREE_DISCONNECT:
	case SMB2_CREATE:
	case SMB2_CLOSE:
	case SMB2_FLUSH:
	case SMB2_LOCK:
	case SMB2_IOCTL:
	case SMB2_CANCEL:
	case SMB2_ECHO:
	case SMB2_QUERY_DIRECTORY:
	case SMB2_CHANGE_NOTIFY:
	case SMB2_QUERY_INFO:
	case SMB2_SET_INFO:
	case SMB2_OPLOCK_BREAK:
	default:
		if (hdr->status)
		{
			butil_log(4, "  with status %04X\n", hdr->status);
		}
		break;
	}
	return 0;
}

int smbproto_v1_packet(smb_ctx_t *ctx, bool isclient, uint8_t *pkt, int bytes)
{
	struct smb_header hdr;
	uint32_t word_count;
	uint32_t byte_count;
	uint8_t *pktptr;
	uint8_t *params;
	uint8_t next_command;
	uint32_t next_offset;
	int hdrz;
	int result;

	if (bytes < sizeof(hdr))
	{
		butil_log(1, "%s[%d]  Short SMB Header\n", isclient ? "C" : "S", ctx->ordinal);
		return -1;
	}
	hdrz = sizeof(hdr);
	if (hdrz > bytes)
	{
		hdrz = bytes;
	}

	byte_count = (uint32_t)bytes;
	pktptr = pkt;
	
	hdr.protocol_id[0] = 0;
	hdr.protocol_id[1] = 0;
	hdr.protocol_id[2] = 0;
	hdr.protocol_id[3] = 0;
	
	// get header
	//
	memcpy(&hdr, pktptr, hdrz);

	if (
			hdr.protocol_id[0] != 0xFF
		||	hdr.protocol_id[1] != 'S'
		||	hdr.protocol_id[2] != 'M'
		||	hdr.protocol_id[3] != 'B'
	)
	{
		butil_log(1, "%s[%d]  Bad SMB Header\n", isclient ? "C" : "S", ctx->ordinal);
		return -1;
	}
	
	pktptr += hdrz;
	byte_count -= hdrz;
	
	if (byte_count == 0)
	{
		butil_log(1, "%s[%d]  No Parameter Block: %s\n", isclient ? "C" : "S", ctx->ordinal, smbproto_smb_command_name(hdr.command));
		params = (uint8_t *)"";
		word_count = 0;
		byte_count = 0;
	}
	else
	{
		// number of parameter words (16 bits each)
		//
		word_count = *pktptr++;
		byte_count--;
		params = pktptr;
		pktptr += word_count * 2;
		byte_count -= word_count * 2;
	}
	
	next_command = hdr.command;
	next_offset  = 0;
	
	do
	{		
		if (smbproto_is_andx_command(next_command))
		{
			result = smbproto_handle_v1_command(ctx, isclient, next_command, &hdr, params  + 4, (word_count - 1) * 2, pktptr, byte_count);
		}
		else
		{
			result = smbproto_handle_v1_command(ctx, isclient, next_command, &hdr, params, word_count * 2, pktptr, byte_count);
		}
		
		if (smbproto_is_andx_command(next_command))
		{
			// andx commands
			next_command = params[0];
			next_offset = params[2];
			next_offset <<= 8;
			next_offset |= params[3];
			
			if (next_command != 0xFF)
			{
				butil_log(1, "ANDX=%02x %u\n", next_command, next_offset);
				params = pkt + next_offset;
			}
		}
		else
		{
			next_command = 0xFF;
		}
	}
	while (next_command != 0xFF);

	return 0;
}


int smbproto_v2_packet(smb_ctx_t *ctx, bool isclient, uint8_t *pkt, int bytes)
{
	struct smb2_header hdr;
	uint32_t word_count;
	uint32_t byte_count;
	uint8_t *pktptr;
	int hdrz;
	int result;

	if (bytes < sizeof(hdr))
	{
		butil_log(1, "%s[%d]  Short SMB Header\n", isclient ? "C" : "S", ctx->ordinal);
		return -1;
	}
	hdrz = sizeof(hdr);
	if (hdrz > bytes)
	{
		hdrz = bytes;
	}

	byte_count = (uint32_t)bytes;
	pktptr = pkt;
	
	do
	{
		hdr.protocol_id[0] = 0;
		hdr.protocol_id[1] = 0;
		hdr.protocol_id[2] = 0;
		hdr.protocol_id[3] = 0;
		
		// get header
		//
		memcpy(&hdr, pktptr, hdrz);
	
		if (
				hdr.protocol_id[0] != 0xFE
			||	hdr.protocol_id[1] != 'S'
			||	hdr.protocol_id[2] != 'M'
			||	hdr.protocol_id[3] != 'B'
		)
		{
			butil_log(1, "%s[%d]  Bad SMB2 Header\n", isclient ? "C" : "S", ctx->ordinal);
			return -1;
		}

		byte_count -= hdrz;
		
		result = smbproto_handle_v2_command(ctx, isclient, hdr.command, &hdr, pktptr, byte_count);
		
		if (hdr.next_command != 0)
		{
			pktptr += hdr.next_command;
		}
	}
	while (!result && (hdr.next_command != 0));

	return 0;
}

int smbproto_packet(smb_ctx_t *ctx, bool isclient, uint8_t *pkt, int bytes)
{
	if (bytes < 1)
	{
		return -1;
	}
	
	uint8_t proto = pkt[0];
	
	if (proto == 0xFF)
	{
		return smbproto_v1_packet(ctx, isclient, pkt, bytes);
	}
	else
	{
		return smbproto_v2_packet(ctx, isclient, pkt, bytes);
	}
}

