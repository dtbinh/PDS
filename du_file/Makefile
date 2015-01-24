CC = gcc

CFLAGS = -Wall -Werror -ansi -pedantic

CFLAGS += -D_XOPEN_SOURCE=500

CFLAGS += -g

all:du_file

%:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o