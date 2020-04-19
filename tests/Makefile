#对于gcc编译选项，应该包含O2优化指令
include ../Make.defines
CFLAGS+= -I../src/
PROG=test
MODINC=-I. -I../src
MODLIBS=-lpthread -lrt -ldl
TESTOMDPARSEOBJ=
TESTRECVPARSEOBJ=util.o ../src/network.o ../src/omdparse.o 
CMOCKAOBJ=cmocka.o
OBJ1=testmktdata.o testparam.o
all:${PROG}
DEPS := ${OBJ1:.o=.d}
-include ${DEPS}
%.o: %.c
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -c $<

testmktdata: ${CMOCKAOBJ} testmktdata.o
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
testparam: ${CMOCKAOBJ} testparam.o
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODINC} ${MODLIBS} 
clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf testmktdata
	rm -rf testparam
