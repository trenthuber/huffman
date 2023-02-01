.PHONY: clean uninstall

CC = cc
CFLAGS = -O3 -Wall -Wextra -pedantic

SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)
BIN = ./bin
LOCAL = /usr/local/bin

all: huffman

clean:
	rm -rf $(BIN) $(OBJ)

huffman: $(OBJ)
	mkdir -p $(BIN)
	$(CC) -o $(BIN)/huffman $^

install: huffman
	cp $(BIN)/huffman $(LOCAL)/huffman
	chmod 755 $(LOCAL)/huffman

uninstall:
	rm -f $(LOCAL)/huffman

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
