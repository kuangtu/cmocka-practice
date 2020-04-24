#ifndef __PROC_H
#define __PROC_H
#include <inttypes.h>

#define DATA_FILE_PATH	"../data/"
#define DATA_FILE_NAME	"secdata.txt"
#define OPEN_DATA_FILE_ERR	-1

#define SECDATA_LINE_LEN	512

#define SECDATA_COL_NUM	6
#define SECDATA_COL_SEP "|"
#define SEQ_SECCODE		0
#define SEQ_LSTPRICE	1
#define SEQ_CLSPRICE	2
#define SEQ_SHARES		3
#define SEQ_TURNOVER	4
#define SEQ_TIME		5


typedef struct TAG_SEC_MEM
{
	uint32_t u32SecurityCode; //证券代码
	uint32_t u32LstPrice;	  //最近成交价格
	uint32_t u32ClsPrice;	  //收盘价格
	uint64_t ullSharesTraded; //成交量
	int64_t llTurnover;       //成交金额
	uint64_t ullSendTime; //发送时间
}SEC_MEM_T;


uint32_t
ProcHqPkt(char *szBuf);

void
InitSecMem();

int
LoadSecData(char *szFileName);

#endif //__PROC_H
