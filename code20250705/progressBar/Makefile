BIN=progressBar
SRC=$(wildcard *.c)
CC=gcc
OUTFLAGS=-o
RM=rm -f

$(BIN):$(SRC)
	$(CC) $(OUTFLAGS) $@ $^
.PHONY:clean
clean:
	$(RM) $(BIN)