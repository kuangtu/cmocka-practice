#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "testutil.h"

char*
CreateHqPkt()
{
	char *szBuf = NULL;
	HQ_PKT_HEADER_T *ptPktHeader = NULL;
	size_t iPktSize = 0;

	//假设一共包含了2个消息
	iPktSize = PKT_HEADER_SIZE + 2 * MSG_STATS_SIZE;

	szBuf = malloc(sizeof(char) * iPktSize);
	memset(szBuf, 0, sizeof(char) * iPktSize);

	//设置包头
	ptPktHeader = (HQ_PKT_HEADER_T*)szBuf;
	ptPktHeader->usPktSize = iPktSize;
	ptPktHeader->ucMsgCount = 2;
	ptPktHeader->u32SeqNum = 10;
	ptPktHeader->ullSendTime = 103000;

	MSG_STATS_T  *ptMsg = (MSG_STATS_T*)(szBuf +  PKT_HEADER_SIZE);
	ptMsg->usMsgSize = MSG_STATS_SIZE;
	ptMsg->usMsgType = 10;
	ptMsg->u32SecurityCode = 1;
	ptMsg->u32LstPrice = 1200;
	ptMsg->u32ClsPrice = 0;
	ptMsg->ullSharesTraded = 200;
	ptMsg->llTurnover = 3000000;

	ptMsg = (MSG_STATS_T*)(szBuf + PKT_HEADER_SIZE + MSG_STATS_SIZE);
	ptMsg->usMsgSize = MSG_STATS_SIZE;
	ptMsg->usMsgType = 10;
	ptMsg->u32SecurityCode = 2;
	ptMsg->u32LstPrice = 200;
	ptMsg->u32ClsPrice = 0;
	ptMsg->ullSharesTraded = 100;
	ptMsg->llTurnover = 300000;
	
	return szBuf;

}


void
FreeHqPkt(HQ_PKT_HEADER_T **ptPktHeader)
{
	free(*ptPktHeader);
	*ptPktHeader = NULL;

}
