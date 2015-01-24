CC = gcc

CFLAGS = -Wall -Werror -ansi -pedantic

CFLAGS += -D_XOPEN_SOURCE=500

CFLAGS += -g

all:do

%:%.c makeargv.o
	chmod 744 $<
	$(CC) $(CFLAGS) makeargv.o -o $@ $<

makeargv.o:makeargv.c
	chmod 744 $<
	$(CC) $(CFLAGS) -c makeargv.c

clean:
	rm -f -v *~ *.o
	if test -f do; then rm -f -v do; fi
