#ifndef __TEST_UTIL_H
#define __TEST_UTIL_H
#include "packet.h"

#define UNUSED(x)	((void)(x))

char*
CreateHqPkt();

void
FreeHqPkt(HQ_PKT_HEADER_T **ptPktHeader);
#endif //__TEST_UTIL_H
