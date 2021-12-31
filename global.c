#include <stdio.h>
#include <stdlib.h> // For exit() function

int length;
int charBit;
int asciiSize;
FILE *input;
FILE *output;

void mallocError(int num){
	printf("huffman: Couldn't allocate memory (%d)\n", num);
	exit(-1);
}
