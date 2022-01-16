#include <stdio.h>
#include <stdlib.h>

int length = 0;
unsigned long long fileSize = 0;
FILE *input;
FILE *output;

void mallocError(int num){
	printf("huffman: Couldn't allocate memory (%d)\n", num);
	exit(-1);
}

void closeAll(void){
	fclose(input);
	fclose(output);
}
