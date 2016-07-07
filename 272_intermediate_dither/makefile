CC=gcc
CFLAGS=-std=c99 -g

BIN=dither
OBJS=main.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
