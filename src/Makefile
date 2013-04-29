CROSS = mipsel-linux-
#CROSS =
CC = $(CROSS)gcc
CPP = $(CROSS)g++
LD = $(CROSS)gcc

OBJECTS = main.o uinput.o

CFLAGS += -I../include

all: server getevent

getevent: getevent.o
	$(LD) getevent.o $(LDFLAGS) -o getevent

server: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o server

getevent.o: getevent.c
	$(CC) $(CFLAGS) -c getevent.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o *~ server
