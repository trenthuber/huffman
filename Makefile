CC = gcc

all: huff test

huff:
	$(CC) -o huffman main.c global.c options.c node.c list.c tree.c table.c encode.c

test:
	./huffman test.txt -o test.huf # Encode test
	./huffman -d test.huf -o testDecode.txt # Decode test