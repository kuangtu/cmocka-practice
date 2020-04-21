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
#include "proc.c"
#include "util.c"


void
test_recv_setup(void **state)
{
	
}

void
test_recv_down(void **state)
{

}

void
testLoadSecData(void **state)
{

}

int
main(int argc, char *argv[]) 
{

	const UnitTest tests[] = {
		unit_test_setup_teardown(testLoadSecData, test_recv_setup, test_recv_down),
	};

	run_tests(tests);

	return 0;
}

