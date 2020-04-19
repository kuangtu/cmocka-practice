#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "recv.h"

static NET_CONTEXT_T gs_tNetContext;

static int 
ConnectSrv();

static int
ReadFromSrv();

int
CopyBuf(char *szBuf, size_t iRead);

void*
RecvHqThd(void *arg)
{
	printf("recv hq\n");
	int iRet = 0;

	iRet = ConnectSrv();
	printf("the connect srv ret is %d\n", iRet);

	iRet = ReadFromSrv();
	printf("read from srv ret is %d\n", iRet);

	return NULL;
}

static int
ConnectSrv()
{
	struct sockaddr_in tSockAddr;

	//初始化NET_CONTEXT_T
	memset(&gs_tNetContext, 0, sizeof(NET_CONTEXT_T));
	memset(&tSockAddr, 0, sizeof(struct sockaddr_in));

	//建立socket,采用IP地址方式进行连接
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &(tSockAddr.sin_addr));

	gs_tNetContext.iSockFd = socket(AF_INET, SOCK_STREAM, 0);
	
	int ret = connect(gs_tNetContext.iSockFd, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr));

	if (ret != 0)
	{
		return CONNECT_ERR;
	}
	else
	{
		return CONNECT_OK;
	}

}

static int
ReadFromSrv()
{

	char szBuf[RECV_BUF_SIZE];  //socket读取buffer
	ssize_t iRead = 0;       //读取到的数据大小
	int iRet = 0;
	fd_set tReadSet;	//读取文件描述符集合
	int iMaxFp;
	int iFeedRet = 0;
	struct timeval tTimeout;

	while(true)
	{

		//清空read集合
		FD_ZERO(&tReadSet);

		memset(&tTimeout, 0, sizeof(struct timeval));

		//设置读取超时时间
		tTimeout.tv_sec = gs_tNetContext.iTimeOut;
		tTimeout.tv_usec = 0;

		//将socket描述符增加到集合中
		FD_SET(gs_tNetContext.iSockFd, &tReadSet);

		//设置select等待读取数据
		iRet = select(gs_tNetContext.iSockFd + 1, &tReadSet, NULL, NULL, &tTimeout);

		if (iRet < 0)
		{
			//此时需要处理EINTR错误，该错误为信号中断错误，继续进行处理
			if (errno == EINTR)
			{
				continue;
			}

			iRet = SELECT_ERR;
			break;
		}

		//select超时，服务端在配置时间内没有发送数据
		if (iRet == 0)
		{
			iRet = SELECT_TIMEOUT;
			break;
		}

		//socket读取
		if (FD_ISSET(gs_tNetContext.iSockFd, &tReadSet))
		{
			iRead = read(gs_tNetContext.iSockFd, szBuf, sizeof(szBuf));
			//服务器对端关闭
			if (iRead == 0)
			{
				iRet = READ_PEER_CLOSE;
				break;
			}
			else if (iRead < 0)
			{
				iRet = READ_ERR;
				break;
			}
			else
			{
				//copy到缓存中，进行解析
				CopyBuf(szBuf, (size_t)iRead);
			}
		}

	}

	return iRet;
}

bool
IsRemain()
{

	return true;
}

bool
IsFullPkt()
{
	return false;
}

int
CopyBuf(char *szBuf, size_t iRead)
{

	bool bBuf = true;
	uint32_t u32SeqNum = 0;

	//判断剩余buffer大小
	if (!IsRemain())
	{
		return BUF_SIZE_ERR;
	}


	//将接收到的数据放置到buffer中
	memcpy(gs_tNetContext.szBuf + gs_tNetContext.iPos, szBuf, iRead);
	gs_tNetContext.iFree -= iRead;
	gs_tNetContext.iPos += iRead;

	//完整数据包的检查
	if(IsFullPkt())	
	{

		//更新到内存中
		u32SeqNum = ProcHqPkt(gs_tNetContext.szBuf);

		//TODO根据完整包的大小还需要调整buffer中的长度
		
		
	}
	

}
