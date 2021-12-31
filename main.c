#include <stdio.h>
#include <stdlib.h> // For exit() function
#include <unistd.h> // For option handling

#include "global.h" 
#include "nodeDef.h"
#include "tree.h"
#include "table.h"
#include "write.h"

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

	FILE *output;
	if((output = fopen("out.huf", "w")) == NULL){
		printf("huffman: Out file could not be created\n");
		exit(-1);
	}

	struct node *root = generateTree(input);

	char **codes = generateTable(root);

	// DEBUGGING
	/*
	for(int i = 0; i < length; i++){
		printf("%d - %s - %d\n", i, codes[i], codes[i][0]);
	}
	*/
	writeFile(root, codes, output);

	// Closing thoughts
	fclose(input);
	fclose(output);
	return 0;
}
