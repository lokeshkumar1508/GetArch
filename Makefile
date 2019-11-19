ifdef DEBUG
CFLAGS += -DDEBUG -g
endif
CC := gcc
DIR := ./
objs := parse.o read_lib.o file_names.o
INC += ./inc

CFLAGS += -I$(INC)
APP := Getarch


.PHONY: all debug clean


.c.o:
	$(CC) $(CFLAGS) -c $*.c

all:$(objs)
	$(CC) $(CFLAGS) -o $(APP) $(objs)
	
clean:
	rm -rf *.o  Getarch
