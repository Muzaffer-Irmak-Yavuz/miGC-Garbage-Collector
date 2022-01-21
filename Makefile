CC = gcc
CFLAGS = -g -Wall
SRC_FILE =  src/vm.c 


all :

	${CC} ${CFLAGS} src/main.c ${SRC_FILE} -o main 