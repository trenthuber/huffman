CC = gcc

all: huff test

huff:
	$(CC) -o huffman main.c global.c node.c list.c tree.c table.c encode.c

test:
	./huffman test.txt
