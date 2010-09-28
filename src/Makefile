# Makefile for entire system

# For NeXT
#CC = cc
#CFLAGS = -g -O2
#LIBS = 

# For SGI
CC = gcc
CFLAGS = -O2
LIBS = -lgcc -lcurses

# standard stuff

INC = include
SRCS = *.cpp
OBJS = *.o

all: ptest
	./inc .build

*.o: *.cpp
	${CC} -c ${SRCS} ${LIBS}

ptest: *.o
	${CC} -o go ${OBJS} ${LIBS}

clean:
	rm *.o
