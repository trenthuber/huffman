#include <stdio.h>
#include <stdlib.h> // For exit() function
#include <unistd.h> // For option handling

#include "globals.h" 
#include "node.h"
#include "treeFuncs.h"
#include "tableFuncs.h"

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("huffman: File is required\n");
		exit(-1);
	}

	FILE *input;
	if((input = fopen(argv[1], "r")) == NULL){
		printf("huffman: File does not exist or cannot be read from\n");
		exit(-1);
	}

	struct node *root = generateTree(input);

	char **codes = treeToTable(root);

	// DEBUGGING
	/*
	for(int i = 0; i < length; i++){
		printf("%d - %s - %d\n", i, codes[i], codes[i][0]);
	}
	*/

	// Closing thoughts
	fclose(input);
	return 0;
}
