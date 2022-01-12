CC = gcc

all: huff test

huff:
	$(CC) -o huffman global.c options.c node.c list.c tree.c fileio.c table.c encode.c decode.c main.c 

test:
	./huffman test.txt -o test.huf # Encode test
	./huffman -d test.huf -o testDecode.txt # Decode test
