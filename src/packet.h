#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>

#define PKT_HEADER_SIZE			16
#define MSG_STATS_SIZE			32

#pragma pack(push, 1)
typedef struct TAG_HQ_PKT_HEADER
{
	uint16_t usPktSize; //数据包的长度，包含头部
	uint8_t ucMsgCount; //数据包中包含的消息数目，财华社特殊处理为1
	char cFiller;
	uint32_t u32SeqNum; //数据包中首个消息的编号
	uint64_t ullSendTime; //发送时间
}HQ_PKT_HEADER_T;

typedef struct TAG_MSG_STATS
{
	uint16_t usMsgSize;  //消息长度
	uint16_t usMsgType;  //消息类型
	uint32_t u32SecurityCode; //证券代码
	uint32_t u32LstPrice;	  //最近成交价格
	uint32_t u32ClsPrice;	  //收盘价格
	uint64_t ullSharesTraded; //成交量
	int64_t llTurnover;       //成交金额
}MSG_STATS_T;

#pragma pack(pop)

#endif //__PACKET_H
