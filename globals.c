#include <stdio.h>
#include <stdlib.h> // For exit() function

int charBit;
int asciiSize;
int length;

void mallocError(int num){
	printf("huffman: Couldn't allocate memory (%d)\n", num);
	exit(-1);
}
