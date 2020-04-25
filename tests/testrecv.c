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
#include "testutil.h"

extern bool gs_bRunFlag;

//Mock socket op
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	UNUSED(sockfd);
	UNUSED(addr);
	UNUSED(addrlen);

	int iRet;

	iRet = mock_type(int);

	return iRet;
}

int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
	UNUSED(nfds);
	UNUSED(readfds);
	UNUSED(writefds);
	UNUSED(exceptfds);
	UNUSED(timeout);

	int iRet;

	iRet = mock_type(int);

	return iRet;


}

ssize_t 
read(int fd, void *buf, size_t count)
{
	UNUSED(fd);
	UNUSED(count);

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

//清空NetContext中的内容
void
test_recv_setup(void **state)
{
	UNUSED(state);
	memset(&gs_tNetContext, 0, sizeof(NET_CONTEXT_T));

}

//清空NetContext中的内容
void
test_recv_down(void **state)
{

	UNUSED(state);
	memset(&gs_tNetContext, 0, sizeof(NET_CONTEXT_T));
}

uint32_t
__wrap_ProcHqPkt(char *szBuf)
{
	UNUSED(szBuf);

	return 0;
}

void
test_ConnSrv_err(void **state)
{

	UNUSED(state);

	int iRet = 0;

	//设置connect返回
	will_return(connect, -1);

	iRet = ConnectSrv();

	assert_int_equal(iRet, CONNECT_ERR);

}

void
test_ConnSrv_ok(void **state)
{

	UNUSED(state);

	int iRet = 0;

	//设置connect返回
	will_return(connect, 0);

	iRet = ConnectSrv();

	assert_int_equal(iRet, CONNECT_OK);

}

void
test_RecvFromSrv_out(void **state)
{

	UNUSED(state);

	int iRet = 0;

	//设置connect返回
	will_return(select, 0);

	iRet = ReadFromSrv();

	assert_int_equal(iRet, SELECT_TIMEOUT);
}

void 
test_RecvFromSrv_err(void **state)
{
	UNUSED(state);

	int iRet = 0;

	//设置connect返回
	will_return(select, -1);

	iRet = ReadFromSrv();

	assert_int_equal(iRet, SELECT_ERR);

}

void 
test_RecvFromSrv_timeout(void **state)
{
	UNUSED(state);
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
	UNUSED(state);
	int iRet = 0;

	//设置select返回正常
	will_return(select, 1);

	//模拟创建一个消息
	HQ_PKT_HEADER_T *ptPktHeader = (HQ_PKT_HEADER_T*)CreateHqPkt();
	will_return(read, ptPktHeader);
	will_return(read, PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE);
	will_return(read, PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE);
	will_return(read, false);

	//设置read读取的buf
	iRet = ReadFromSrv();

	assert_int_equal(iRet, COPY_BUF_OK);
	FreeHqPkt(&ptPktHeader);

}

void
test_CopyBuf(void **state)
{

	UNUSED(state);
	int iRet = 0;
	HQ_PKT_HEADER_T *ptPktHeader = (HQ_PKT_HEADER_T*)CreateHqPkt();
	size_t iLen = ptPktHeader->usPktSize;

	iRet = CopyBuf((char*)ptPktHeader, iLen);	
	
	assert_int_equal(iRet, COPY_BUF_OK);

	FreeHqPkt(&ptPktHeader);

}

void
test_CopyBuf_NOT_FULL(void **state)
{
	UNUSED(state);
	int iRet = 0;
	HQ_PKT_HEADER_T *ptPktHeader = NULL;

	//包头数据
	ptPktHeader = malloc(sizeof(HQ_PKT_HEADER_T));
	memset(ptPktHeader, 0, sizeof(HQ_PKT_HEADER_T));
	//数据长度为100
	ptPktHeader->usPktSize = 100;
	size_t iLen = 20;

	//接收到了数据为20，数据包中的长度100
	iRet = CopyBuf((char*)ptPktHeader, iLen);	
	
	assert_int_equal(iRet, NOT_FULL_PKT);

	FreeHqPkt(&ptPktHeader);

}


int
main(int argc, char *argv[]) 
{

	UNUSED(argc);
	UNUSED(argv);

	const UnitTest tests[] = {
		unit_test_setup_teardown(test_ConnSrv_err, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_ConnSrv_ok, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_out, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_err, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_RecvFromSrv_ok, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_CopyBuf, test_recv_setup, test_recv_down),
		unit_test_setup_teardown(test_CopyBuf_NOT_FULL, test_recv_setup, test_recv_down),
	};

	run_tests(tests);

	return 0;
}

