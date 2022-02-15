#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"

char **codes;
unsigned char currentChar;

void freeTable(char **codes){
	for(int i = 0; i < length; i++){
		free(codes[i]);
	}
	free(codes);
}

// Adds a char to the beginning of a string
void prepend(char *string, char chr){
	char *temp = string;

	// Find null terminating character
	while(*temp != '\0'){
		temp++;
	}

	// Shift each character to the right
	while(temp != string){
		char *current = temp + 1;
		*current = *temp;
		temp--;
	}
	char *current = temp + 1;
	*current = *temp;

	// Insert chr at beginning
	*temp = chr;
}

/* Given a node in a Huffman tree, this function uses the
 * parent nodes of each node to traverse it's way back up
 * tree to the root node. It takes this information as well
 * as the node type identifier to insert a string of '1's
 * and '0's into an array of strings.
 */
void makeCode(struct node *node){

	// We must be at the root node, thus we're done
	if(node->parent == NULL){
		return;
	}

	// Node is a left node
	if(node->type == 0){
		prepend(codes[(int) currentChar], '0');

	// Node is a right node
	}else{
		prepend(codes[(int) currentChar], '1');
	}

	makeCode(node->parent);
}

void traverseTree(struct node *branch){
	if(branch->left == NULL && branch->right == NULL){
		currentChar = branch->symbol;
		makeCode(branch);
	}else{
		traverseTree(branch->left);
		traverseTree(branch->right);
	}
}

/* Takes the root node of the Huffman tree as input. Produces an array of strings
 * where the first character of each string is a character from a leaf node and the
 * rest of the string is the Huffman code for the character
 */
char **makeTable(struct node *root){

	codeLength = (int) ceil(log2((double) fileSize + 1)) + 1; // +1 for the null character at the end

	// Allocate memory for pointers to strings
	codes = (char **) calloc(ASCII_SIZE, sizeof(char *));
	if(codes == NULL){mallocError("table.c", 0);}

	// Allocate memory for strings
	for(int i = 0; i < ASCII_SIZE; i++){
		codes[i] = (char *) malloc(codeLength * sizeof(char));
		if(codes[i] == NULL){mallocError("table.c", 1);}
		codes[i][0] = '\0';
	}

	traverseTree(root);

	return codes;
}
