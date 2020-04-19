#include <stdio.h>
#include <pthread.h>
#include "recv.h"
#include "packet.h"

int
main(int argc, char *argv[]) 
{

	pthread_t tRecvThd;
	
	//创建行情接收线程
	if (pthread_create(&tRecvThd, NULL, (void*(*)(void*))RecvHqThd, NULL))
	{
		//LOG ERR
	}

	pthread_join(tRecvThd, NULL);

	printf("the pkt header size is:%zu\n", sizeof(HQ_PKT_HEADER_T));
	printf("the statc size is:%zu\n", sizeof(MSG_STATS_T));

	return 0;

}
