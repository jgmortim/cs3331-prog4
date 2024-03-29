## ----------------------------------------------------------- ##
## NAME : John Mortimore                     User ID: jgmortim ##
## DUE DATE : 04/05/2019                                       ##
## PROGRAM ASSIGNMENT #4                                       ##
## FILE NAME : Makefile                                        ##
## PROGRAM PURPOSE :                                           ##
##    The make file for program 4.                             ##
## ----------------------------------------------------------- ##
CC       = c++
FLAGS    = 
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp -W
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/local/eit-linux/apps/ThreadMentor/include
TMLIB    = /local/eit-linux/apps/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = /local/eit-linux/apps/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = thread.o thread-main.o thread-support.o
EXE_FILE = prog4

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

thread.o: thread.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp

thread-main.o: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp

thread-support.o: thread-support.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-support.cpp

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

visual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread
		                                        
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
