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
#include "proc.c"
#include "util.c"
#include "testutil.h"


void
test_proc_setup(void **state)
{
	UNUSED(state);
}

void
test_proc_down(void **state)
{
	UNUSED(state);

}

void
testLoadSecData(void **state)
{
	UNUSED(state);

	char szFileName[PATH_MAX + 1];
	int iRet = 0;

	//模拟创建一个文件
    memset(szFileName, 0, PATH_MAX + 1);
	strncat(szFileName, DATA_FILE_NAME, strlen(DATA_FILE_NAME));

	FILE *fpData = fopen(szFileName, "w+");
	assert_non_null(fpData);
	char *szLine = 
		"00700|40000|0|100|400000|093400\n"
		"03988|12000|0|200|200000|093300\n";
	fprintf(fpData, "%s", szLine);
	fclose(fpData);

	//初始化内存行情
	InitSecMem();

	iRet = LoadSecData(szFileName);	
	assert_int_equal(iRet, 0);

	uint32_t u32Secode = 700;
	uint32_t u32LstPrice = 40000;

	SEC_MEM_T secMem = gs_tSecMem[u32Secode];
	assert_int_equal(secMem.u32LstPrice, u32LstPrice);

	//删除模拟行情文件
	remove(szFileName);

}

void
testLoadSecData_uncomp(void **state)
{

	UNUSED(state);

	char szFileName[PATH_MAX + 1];
	int iRet = 0;

	//模拟创建一个文件
    memset(szFileName, 0, PATH_MAX + 1);
	strncat(szFileName, DATA_FILE_NAME, strlen(DATA_FILE_NAME));

	FILE *fpData = fopen(szFileName, "w+");
	assert_non_null(fpData);
	char *szLine = 
		"00700|40000|0|100|400000|093400\n"
		"03988|12000|0";
	fprintf(fpData, "%s", szLine);
	fclose(fpData);

	//初始化内存行情
	InitSecMem();

	iRet = LoadSecData(szFileName);	
	assert_int_equal(iRet, 0);

	uint32_t u32Secode = 3988;
	uint32_t u32LstPrice = 12000;

	SEC_MEM_T secMem = gs_tSecMem[u32Secode];
	//不完整的记录，不会更新
	assert_int_not_equal(secMem.u32LstPrice, u32LstPrice);
	assert_int_equal(secMem.u32LstPrice, 0);

	//删除模拟行情文件
	remove(szFileName);
}

void
testLoadSecData_fileErr(void **state)
{

	UNUSED(state);

	char szFileName[PATH_MAX + 1];
	int iRet = 0;

	//模拟文件不存在
    memset(szFileName, 0, PATH_MAX + 1);
	strncat(szFileName, "filenotfind.txt", PATH_MAX);


	//初始化内存行情
	InitSecMem();

	iRet = LoadSecData(szFileName);	
	assert_int_equal(iRet, OPEN_DATA_FILE_ERR);

}
void 
testProcHqPkt(void **state)
{
	UNUSED(state);

	//初始化内存行情
	InitSecMem();

	uint32_t u32SeqNum = 0;
	uint16_t usPktSize = 0;
	//创建一个数据包其中包含了2个消息
	char *szBuf = CreateHqPkt();
	HQ_PKT_HEADER_T *ptPktHeader;
	//消息中的证券代码为:
	//code-1,LstPrice=1200, ullShares=200
	//code-2,LstPrice=200, Turnover=300000
	ptPktHeader = (HQ_PKT_HEADER_T*)szBuf;
	usPktSize = ptPktHeader->usPktSize;
	u32SeqNum = ptPktHeader->u32SeqNum;

	u32SeqNum = ProcHqPkt((char*)ptPktHeader);
	//下一个期望的消息序号
	assert_int_equal(u32SeqNum, 12);
	
	SEC_MEM_T secMem = gs_tSecMem[1];
	assert_int_equal(secMem.u32LstPrice, 1200);
	assert_int_equal(secMem.ullSharesTraded, 200);
	assert_int_equal(secMem.llTurnover, 3000000);

	secMem = gs_tSecMem[2];
	assert_int_equal(secMem.u32LstPrice, 200);
	assert_int_equal(secMem.ullSharesTraded, 100);
	assert_int_equal(secMem.llTurnover, 300000);
	FreeHqPkt(&ptPktHeader);

}

static void
testUpdateMsg(void **state)
{
	UNUSED(state);

	MSG_STATS_T tMsg;

	memset(&tMsg, 0, sizeof(MSG_STATS_T));

	tMsg.u32Code = 3988;
	tMsg.u32LstPrice = 1000;
	tMsg.u32ClsPrice = 1010;
	tMsg.ullSharesTraded = 200;
	tMsg.llTurnover = 180000;

	UpdateMsg(&tMsg);

	SEC_MEM_T *ptSecMem = &gs_tSecMem[3988];

	assert_int_equal(ptSecMem->u32LstPrice, 1000);
	assert_int_equal(ptSecMem->llTurnover, 180000);

}

int
main(int argc, char *argv[]) 
{

	UNUSED(argc);
	UNUSED(argv);

	const UnitTest tests[] = {
		unit_test_setup_teardown(testLoadSecData, test_proc_setup, test_proc_down),
		unit_test_setup_teardown(testLoadSecData_uncomp, test_proc_setup, test_proc_down),
		unit_test_setup_teardown(testLoadSecData_fileErr, test_proc_setup, test_proc_down),
		unit_test_setup_teardown(testProcHqPkt, test_proc_setup, test_proc_down),
		unit_test_setup_teardown(testUpdateMsg, test_proc_setup, test_proc_down),
	};

	run_tests(tests);

	return 0;
}

