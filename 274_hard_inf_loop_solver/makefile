CC=gcc
CFLAGS=-std=c99 -g

BIN=infloop
OBJS=main.o print.o infloop.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
