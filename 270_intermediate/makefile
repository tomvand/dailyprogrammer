CC=gcc
CFLAGS=-std=c99 -g


BIN=markov
OBJS=main.o dict.o vector.o ftok.o
CFLAGS+=

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
