CC=gcc
CFLAGS=-std=c99 -g

BIN=decrypt
OBJS=main.o ../util/ftok.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
