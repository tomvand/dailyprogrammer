CC=gcc
CFLAGS=-std=c99 -g

BIN=parse
OBJS=main.o vector.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
