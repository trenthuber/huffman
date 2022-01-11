#include <stdio.h>
#include <stdlib.h> // For exit() function

int length;
int charBit;
int asciiSize;
FILE *input;
FILE *output;

unsigned char buffer;
int bufferSize = 0;

void mallocError(int num){
	printf("huffman: Couldn't allocate memory (%d)\n", num);
	exit(-1);
}
