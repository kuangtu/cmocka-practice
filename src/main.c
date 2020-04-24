#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include "recv.h"
#include "packet.h"
#include "proc.h"
#include "util.h"

int
main(int argc, char *argv[]) 
{

	UNUSED(argc);
	UNUSED(argv);

	pthread_t tRecvThd;
	int iRet = 0;
	char szFileName[PATH_MAX + 1];

	printf("the pkt header size is:%zu\n", sizeof(HQ_PKT_HEADER_T));
	printf("the statc size is:%zu\n", sizeof(MSG_STATS_T));

    memset(szFileName, 0, PATH_MAX + 1);
	strncat(szFileName, DATA_FILE_PATH, strlen(DATA_FILE_PATH));
	strncat(szFileName, DATA_FILE_NAME, strlen(DATA_FILE_NAME));

	//InitData
	iRet = LoadSecData(szFileName);	
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
