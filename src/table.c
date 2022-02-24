#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global/global.h"

char **codes;
char *prefix;

void makeTableHelper(struct node *branch){
	if(branch->left == NULL){
		strcpy(codes[(int) branch->symbol], prefix);
		return;
	}

	int index = strlen(prefix);
	prefix[index] = '0';
	prefix[index + 1] = '\0';
	makeTableHelper(branch->left);

	prefix[index] = '1';
	prefix[index + 1] = '\0';
	makeTableHelper(branch->right);
}

/* Takes the root node of the Huffman tree as input. Produces an array of strings
 * where the first character of each string is a character from a leaf node and the
 * rest of the string is the Huffman code for the character
 */
char **makeTable(struct node *root){

	codeLength = (int) ceil(log2((double) fileSize + 1)) + 1; // +1 for the null character at the end
	prefix = (char *) malloc(codeLength * sizeof(char));
	prefix[0] = '\0'; // Make sure the prefix is empty before using it

	// Allocate memory for pointers to strings
	codes = (char **) calloc(ASCII_SIZE, sizeof(char *));
	if(codes == NULL){mallocError("table.c", 0);}

	// Allocate memory for strings
	for(int i = 0; i < ASCII_SIZE; i++){
		codes[i] = (char *) malloc(codeLength * sizeof(char));
		if(codes[i] == NULL){mallocError("table.c", 1);}
		codes[i][0] = '\0';
	}

	makeTableHelper(root);

	return codes;
}

void freeTable(char **codes){
	for(int i = 0; i < length; i++){
		free(codes[i]);
	}
	free(codes);
}
