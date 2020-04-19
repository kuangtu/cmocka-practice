#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "recv.h"
#include "packet.h"
#include "proc.h"

int
main(int argc, char *argv[]) 
{

	pthread_t tRecvThd;
	int iRet = 0;

	printf("the pkt header size is:%zu\n", sizeof(HQ_PKT_HEADER_T));
	printf("the statc size is:%zu\n", sizeof(MSG_STATS_T));

	//InitData
	iRet = LoadSecData();	
	if(iRet != 0)
	{
		printf("load data error.\n");

		exit(EXIT_FAILURE);
	}
	
	//创建行情接收线程
	if (pthread_create(&tRecvThd, NULL, (void*(*)(void*))RecvHqThd, NULL))
	{
		//LOG ERR
	}

	//解析、发送等线程

	pthread_join(tRecvThd, NULL);

	printf("main finished.\n");


	return 0;

}
