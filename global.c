#include <stdio.h>
#include <stdlib.h>

int length;
FILE *input;
FILE *output;

void mallocError(int num){
	printf("huffman: Couldn't allocate memory (%d)\n", num);
	exit(-1);
}
