/*########################################################################
 *#                                                                      #
 *#                      Copyright (c) 2020 by                           #
 *#          China Securities Index (CSI), Shanghai, China              #
 *#                       All rights reserved.                           #
 *#                                                                      #
 *########################################################################

 *****************************************************************************
 *
 * Module Name   : 内存行情处理单元测试模块
 * File   Name   : mktdata.c
 * Description   : market data process
 * Corporation   : CSI
 * Author        : Moxuansheng
 * Created Date  : 2020-02-18
 * Memo          :
 *****************************************************************************/
/*****************************************************************************
 *  MODIFICATION HISTORY:
 *
 *    DATE      PROG.
 *  DD-MMM-YYYY INIT.         SIR    Modification Description
 *  ----------- ----------- ------  ------------------------------------------
 *  18-Feb-2020 Moxuansheng           Create
 *****************************************************************************/

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <setjmp.h>
#include "cmocka.h"
#include "cmocka_private.h"
#include "packet.h"
#include "proc.h"
#include "recv.c"

extern bool gs_bRunFlag;

//Mock socket op
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int iRet;

	iRet = mock_type(int);

	return iRet;
}

int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
	int iRet;

	iRet = mock_type(int);

	return iRet;


}

ssize_t 
read(int fd, void *buf, size_t count)
{
	char *szBuf;
	size_t iNum;
	ssize_t iRet;

	//设置buf
	szBuf = mock_type(char*);
	iNum = mock_type(size_t);
	iRet = mock_type(ssize_t);
	gs_bRunFlag = mock_type(bool);
	
	//拷贝到buf中
	memcpy(buf, szBuf, iNum);


	return iRet;

}


void
test_recv_setup(void **state)
{
	
}

void
test_recv_down(void **state)
{

}

HQ_PKT_HEADER_T*
CreateHqPkt()
{
	HQ_PKT_HEADER_T *ptPktHeader = NULL;
	size_t iPktSize = 0;

	//假设一共包含了2个消息
	iPktSize = PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE;

	ptPktHeader = malloc(sizeof(char) * iPktSize);

	//设置包头
	ptPktHeader->usPktSize = iPktSize;
	ptPktHeader->ucMsgCount = 2;
	ptPktHeader->u32SeqNum = 10;
	ptPktHeader->ullSendTime = 103000;

	MSG_STATS_T  *ptMsg = (MSG_STATS_T*)(ptPktHeader + PKT_HEADER_SIZE);
	ptMsg->usMsgSize = MSG_STATS_SIZE;
	ptMsg->usMsgType = 10;
	ptMsg->u32SecurityCode = 1;
	ptMsg->u32LstPrice = 1200;
	ptMsg->u32ClsPrice = 0;
	ptMsg->ullSharesTraded = 200;
	ptMsg->llTurnover = 3000000;

	ptMsg = (MSG_STATS_T*)(ptPktHeader + PKT_HEADER_SIZE + MSG_STATS_SIZE);
	ptMsg->usMsgSize = MSG_STATS_SIZE;
	ptMsg->usMsgType = 10;
	ptMsg->u32SecurityCode = 2;
	ptMsg->u32LstPrice = 200;
	ptMsg->u32ClsPrice = 0;
	ptMsg->ullSharesTraded = 100;
	ptMsg->llTurnover = 300000;
	
	return ptPktHeader;

}

void
FreeHqPkt(HQ_PKT_HEADER_T **ptPktHeader)
{
	free(*ptPktHeader);
	*ptPktHeader = NULL;

}
void
test_ConnSrv_err(void **state)
{

	int iRet = 0;

	//设置connect返回
	will_return(connect, -1);

	iRet = ConnectSrv();

	assert_int_equal(iRet, CONNECT_ERR);

}

void
test_ConnSrv_ok(void **state)
{

	int iRet = 0;

	//设置connect返回
	will_return(connect, 0);

	iRet = ConnectSrv();

	assert_int_equal(iRet, CONNECT_OK);

}

void
test_RecvFromSrv_out(void **state)
{

	int iRet = 0;

	//设置connect返回
	will_return(select, 0);

	iRet = ReadFromSrv();

	assert_int_equal(iRet, SELECT_TIMEOUT);
}

void 
test_RecvFromSrv_err(void **state)
{
	int iRet = 0;

	//设置connect返回
	will_return(select, -1);

	iRet = ReadFromSrv();

	assert_int_equal(iRet, SELECT_ERR);

}

void 
test_RecvFromSrv_timeout(void **state)
{
	int iRet = 0;

	//设置select返回正常
	will_return(select, 1);

	//设置read读取的buf
	iRet = ReadFromSrv();

	assert_int_equal(iRet, SELECT_ERR);

}

void 
test_RecvFromSrv_ok(void **state)
{
	int iRet = 0;

	//设置select返回正常
	will_return(select, 1);

	//模拟创建一个消息
	HQ_PKT_HEADER_T *ptPktHeader = CreateHqPkt();
	will_return(read, ptPktHeader);
	will_return(read, PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE);
	will_return(read, PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE);
	will_return(read, false);

	//设置read读取的buf
	iRet = ReadFromSrv();

	//
	assert_int_equal(iRet, COPY_BUF_OK);

}



int
main(int argc, char *argv[]) 
{

	const UnitTest tests[] = {
		unit_test_setup_teardown(test_ConnSrv_err, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_ConnSrv_ok, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_out, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_err, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_ok, test_recv_setup, test_recv_down),
	};

	run_tests(tests);

	return 0;
}

