#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "packet.h"
#include "proc.h"
#include "util.h"

#define MAX_SEC_CODE 10000

static SEC_MEM_T gs_tSecMem[MAX_SEC_CODE];

static void
UpdateMsg(MSG_STATS_T *ptMsg)
{
	uint32_t u32Code = ptMsg->u32SecurityCode;
	SEC_MEM_T *ptSecMem = &gs_tSecMem[u32Code];

	uint32_t u32LstPrice = ptMsg->u32LstPrice;
	ptSecMem->u32LstPrice = u32LstPrice;

	uint32_t u32ClsPrice = ptMsg->u32ClsPrice;
	ptSecMem->u32ClsPrice = u32ClsPrice;

	uint64_t u64Shares = ptMsg->ullSharesTraded;
	ptSecMem->ullSharesTraded = u64Shares;

	int64_t llTurnover = ptMsg->llTurnover;
	ptSecMem->llTurnover = llTurnover;

	return;

}

void
InitSecMem()
{
	memset(&gs_tSecMem, 0, sizeof(SEC_MEM_T) * MAX_SEC_CODE);
}

int
LoadSecData(char *szFileName)
{
    char szReadLine[SECDATA_LINE_LEN];
	LINE_COLUMNS_T *ptLineColumns = NULL;

	FILE *fpFile = fopen(szFileName, "r");
	//Check FILE
	if (!fpFile)
	{
		//TODO strerror
		return OPEN_DATA_FILE_ERR;
	}
	
	memset(szReadLine, 0, SECDATA_LINE_LEN);
	while(fgets(szReadLine, 256, fpFile) != NULL)
	{

        ptLineColumns = GetLineColumns(szReadLine, SECDATA_COL_NUM, SECDATA_COL_SEP);
		if (ptLineColumns->u32Num != SECDATA_COL_NUM)
		{
            memset(szReadLine, 0, SECDATA_LINE_LEN);
            FreeLineColumns(ptLineColumns);
			continue;
		}

		//TODO:NUMBER CHECK and number range check
		char *pEnd;
		char *szCode = ptLineColumns->szColumns[SEQ_SECCODE];
		unsigned long int code = strtoul(szCode, &pEnd, 10);
		SEC_MEM_T *secTmp = &gs_tSecMem[code];

		char *szLstPrice = ptLineColumns->szColumns[SEQ_LSTPRICE];
		unsigned long int price = strtoul(szLstPrice, &pEnd, 10);
		secTmp->u32LstPrice = (uint32_t)price;

		char *szClsPrice = ptLineColumns->szColumns[SEQ_CLSPRICE];
		price = strtoul(szClsPrice, &pEnd, 10);
		secTmp->u32ClsPrice = (uint32_t)price;

		char *szshare = ptLineColumns->szColumns[SEQ_SHARES];
		unsigned long long shares = strtoull(szshare, &pEnd, 10);
		secTmp->ullSharesTraded = shares;

		char *szturnover = ptLineColumns->szColumns[SEQ_TURNOVER];
		long long turnover = strtoll(szturnover, &pEnd, 10);
		secTmp->llTurnover = turnover;

		char *sztime = ptLineColumns->szColumns[SEQ_TIME];
		unsigned long long time = strtoull(sztime, &pEnd, 10);
		secTmp->ullSendTime = time;

		memset(szReadLine, 0, SECDATA_LINE_LEN);
		FreeLineColumns(ptLineColumns);
	}

	return 0;

}

uint32_t
ProcHqPkt(char *szBuf)
{
	uint32_t u32SeqNum = 0;
	uint16_t usPktSize = 0;

	HQ_PKT_HEADER_T *ptPktHeader = (HQ_PKT_HEADER_T*)szBuf;
	usPktSize = ptPktHeader->usPktSize;
	u32SeqNum = ptPktHeader->u32SeqNum;

	szBuf += PKT_HEADER_SIZE;
	usPktSize -= PKT_HEADER_SIZE;

	//跳过包头指向消息
	while(usPktSize > 0)
	{
		MSG_STATS_T *ptMsg = (MSG_STATS_T*)szBuf;
		UpdateMsg(ptMsg);
		szBuf +=  ptMsg->usMsgSize;
		usPktSize -= ptMsg->usMsgSize;
		u32SeqNum++;
	}

	//最新的消息编号
	return u32SeqNum;
		
}


