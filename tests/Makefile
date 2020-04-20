#对于gcc编译选项，应该包含O2优化指令
include ../Make.defines
CFLAGS+= -I../src/
PROG=testrecv testproc
#可以将源代码中的.c文件包含进来
MODINC=-I. -I../src
MODLIBS=-lpthread -lrt -ldl
TESTRECVOBJ=
TESTPROCOBJ=
CMOCKAOBJ=cmocka.o
OBJ1=testrecv.o testproc.o
all:${PROG}
DEPS := ${OBJ1:.o=.d}
-include ${DEPS}
%.o: %.c
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -c $<

testrecv: ${CMOCKAOBJ} testrecv.o
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
testproc: ${CMOCKAOBJ} testproc.o
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf testrecv
	rm -rf testproc
