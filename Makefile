CC = gcc

all: huff test

huff:
	$(CC) -o huffman main.c globals.c nodeFuncs.c listFuncs.c treeFuncs.c tableFuncs.c

test:
	./huffman test.txt
