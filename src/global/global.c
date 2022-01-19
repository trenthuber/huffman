#include <stdio.h>
#include <stdlib.h>

int length = 0;
unsigned long fileSize = 0;
FILE *input;
FILE *output;

void mallocError(char *filename, int num){
	printf("huffman: Couldn't allocate memory (%s, %d)\n", filename, num);
	exit(-1);
}

void closeAll(void){
	fclose(input);
	fclose(output);
}
