#include <stdio.h>
#include <stdlib.h>

int length = 0;
int fileSize = 0;
int codeLength = 0;
FILE *input = NULL;
FILE *output = NULL;

void mallocError(char *filename, int num){
	fprintf(stderr, "huffman: Couldn't allocate memory (%s, %d)\n", filename, num);
	exit(-1);
}

void closeAll(void){
	if(input != NULL){
		fclose(input);
	}
	if(output != NULL){
		fclose(output);
	}
}
