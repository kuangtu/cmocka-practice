#ifndef __RECV_H
#define __RECV_H
#include <stdbool.h>

#define RECV_BUF_SIZE 		64 * 1024
#define SERVER_IP 			"127.0.0.1"
#define SERVER_PORT			10240

#define RECV_SOCKET_OK		1
#define CONNECT_OK			0
#define CONNECT_ERR			-1

#define SELECT_ERR			-2
#define SELECT_TIMEOUT		-3

#define READ_PEER_CLOSE		-4
#define READ_ERR			-5


#define BUF_SIZE_ERR		-9
#define COPY_BUF_OK			1
#define NOT_FULL_PKT		-1



typedef struct TAG_NET_CONTEXT
{
	int iErr;
	int iSockFd;
	int iSockFlag;
	int iTimeOut;
	size_t iPos;
	size_t iFree;
	char szBuf[RECV_BUF_SIZE];
} NET_CONTEXT_T;

void*
RecvHqThd(void *arg);

void
SetRecvFlg(bool bFlg);

#endif //__RECV_H
