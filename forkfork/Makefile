CC = gcc

CFLAGS = -Wall -Werror -ansi -pedantic

CFLAGS += -D_XOPEN_SOURCE=500

CFLAGS += -g

all:multiprocess ff race ff.txt race.txt

ff.txt:ff
	./ff > ff.txt

race.txt:race
	./race > race.txt

%:%.c forkfork.o
	chmod 744 $<
	$(CC) $(CFLAGS) forkfork.o -o $@ $<

forkfork.o:forkfork.c forkfork.h
	chmod 744 forkfork.*
	$(CC) $(CFLAGS) -c forkfork.c

clean:
	rm -f -v *~ *.o *.txt
	if test -f ff; then rm -f -v ff; fi
	if test -f race; then rm -f -v race; fi
	if test -f multiprocess; then rm -f -v multiprocess; fi
