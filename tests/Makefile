#通过Make.defines定义对于gcc编译选项
include ../Make.defines
CFLAGS+= -I../src/
PROG=testrecv testproc
#直接将源代码中的.c文件包含进来
MODINC=-I. -I../src
MODLIBS=-lpthread -lrt -ldl
TESTRECVOBJ= testutil.o
TESTPROCOBJ= testutil.o
CMOCKAOBJ=cmocka.o
OBJ1=testrecv.o testproc.o
all:${PROG}
# 包含编译依赖关系
DEPS := ${OBJ1:.o=.d}
-include ${DEPS}
%.o: %.c
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -c $<
# 编译测试模块
testrecv: ${CMOCKAOBJ} ${TESTRECVOBJ} testrecv.o
	${CC} -Wl,--wrap=ProcHqPkt -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
testproc: ${CMOCKAOBJ} ${TESTPROCOBJ} testproc.o
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf testrecv
	rm -rf testproc
