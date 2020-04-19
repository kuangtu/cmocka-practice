#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "packet.h"
#include "proc.h"
#include "util.h"

#define MAX_SEC_CODE 100

static SEC_MEM_T tSecMem[MAX_SEC_CODE];


void
InitSecMem()
{
	memset(&tSecMem, 0, sizeof(SEC_MEM_T) * MAX_SEC_CODE);
}

int
LoadSecData()
{
	char szFileName[PATH_MAX + 1];
    char szReadLine[SECDATA_LINE_LEN];
	LINE_COLUMNS_T *ptLineColumns = NULL;

    memset(szFileName, 0, PATH_MAX + 1);
	strncat(szFileName, DATA_FILE_PATH, strlen(DATA_FILE_PATH));
	strncat(szFileName, DATA_FILE_NAME, strlen(DATA_FILE_NAME));
	printf("the szFileName is:%s\n", szFileName);

	FILE *fpFile = fopen(szFileName, "w+");
	//Check FILE
	
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
		SEC_MEM_T secTmp = tSecMem[code];

		char *szLstPrice = ptLineColumns->szColumns[SEQ_LSTPRICE];
		unsigned long int price = strtoul(szLstPrice, &pEnd, 10);
		secTmp.u32LstPrice = (uint32_t)price;

		char *szClsPrice = ptLineColumns->szColumns[SEQ_CLSPRICE];
		price = strtoul(szClsPrice, &pEnd, 10);
		secTmp.u32ClsPrice = (uint32_t)price;

		char *szshare = ptLineColumns->szColumns[SEQ_SHARES];
		unsigned long long shares = strtoull(szshare, &pEnd, 10);
		secTmp.ullSharesTraded = shares;

		char *szturnover = ptLineColumns->szColumns[SEQ_TURNOVER];
		long long turnover = strtoll(szturnover, &pEnd, 10);
		secTmp.llTurnover = turnover;

		char *sztime = ptLineColumns->szColumns[SEQ_TIME];
		unsigned long long time = strtoull(sztime, &pEnd, 10);
		secTmp.ullSendTime = time;

		memset(szReadLine, 0, SECDATA_LINE_LEN);
		FreeLineColumns(ptLineColumns);
	}


}

int
ProcHqPkt(const char *szBuf)
{
	uint8_t ucMsgCount = 0;
	uint16_t usPktSize = 0;
	HQ_PKT_HEADER_T *ptPktHeader = (HQ_PKT_HEADER_T*)szBuf;

	usPktSize = ptPktHeader->usPktSize;
	ucMsgCount = ptPktHeader->ucMsgCount;

	return 0;
			
}


static int
UpdateMsg()
{

}

