CC=gcc
CFLAGS=-std=c99 -g

BIN=rekt
OBJS=main.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
