#对于gcc编译选项，应该包含O2优化指令
include ../Make.defines
DEST=../bin/
PROG=cmocka_demo
MODINC=-I.
MODFLGS= -DREENTRANT
MODLIBS=-lpthread 
#依赖的库
OBJ1= main.o  recv.o proc.o util.o

all: ${PROG}
# 依赖关系
DEPS := ${OBJ1:.o=.d}
-include ${DEPS}
#编译
%.o: %.c
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	${CC}  ${CFLAGS} ${MODFLGS} ${MODINC} -c $<
cmocka_demo:${OBJ1}
	${CC}  -o ${DEST}$@ $^ ${CFLAGS} ${MODLIBS} 

clean:
	rm -rf *.o  
	rm -rf *.d
	rm -rf ../bin/cmocka_demo
