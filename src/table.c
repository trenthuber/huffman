#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"

char *string;
char **codes;
unsigned char currentChar;

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
	if(node->type == 0){
		return;
	}

	// Node is a left node
	if(node->type == 1 || node->type == 3){
		prepend(codes[(int) currentChar], '0');

	// Node is a right node
	}else{
		prepend(codes[(int) currentChar], '1');
	}

	makeCode(node->parent);
}

void traverseTree(struct node *branch){
	if(branch->type == 3 || branch->type == 4){
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

	int codeLength = (int) ceil(log2((double) fileSize + 1)) + 1; // +1 for the null character at the end

	/* The location of each code represents what character 
	 * it's a code for, hence the use of ASCII_SIZE and not length
	 */
	string = (char *) malloc(ASCII_SIZE * codeLength * sizeof(char));
	codes = (char **) malloc(ASCII_SIZE * sizeof(char *));
	if(string == NULL || codes == NULL){mallocError("table.c", 0);}

	// Prepare the code table and pointers
	for(int i = 0; i < ASCII_SIZE; i++){
		*(string + (codeLength * i)) = '\0';
		*(codes + i) = &string[codeLength * i];
	}

	traverseTree(root);

	return codes;
}
