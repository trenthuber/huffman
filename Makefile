.PHONY: bin clean uninstall

CC = gcc
CFLAGS = -O3 -Wall -Wextra
LDFLAGS = -lm

SRC = $(wildcard src/*.c) $(wildcard src/global/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin
LOCAL = /usr/local/bin

all: bin huffman

bin:
	mkdir -p ./$(BIN)

huffman: $(OBJ)
	$(CC) -o $(BIN)/huffman $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

install: all
	cp $(BIN)/huffman $(LOCAL)/huffman
	chmod 755 $(LOCAL)/huffman

uninstall:
	rm -f $(LOCAL)/huffman