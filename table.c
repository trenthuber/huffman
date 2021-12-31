#include <stdio.h>
#include <stdlib.h> 

#include "global.h"
#include "nodeDef.h"

char **codes;
char *string;
int currentCode = 0;
char ***leftPointers;
int currentLeft = 0;

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

/* In all honesty, this function took quite a long time to create and I
 * consider it to be somewhat magical; I don't really feel like explaining
 * this one in much detail.
 */
char **generateEncodeTableHelper(struct node *branch){
	if(branch->symbol != '\0'){
		prepend(codes[currentCode], branch->symbol);
		return &codes[currentCode++]; 
	}else{

		// Recursively adds to the string for the left branch
		leftPointers[currentLeft] = generateEncodeTableHelper(branch->left);
		currentLeft++;

		// Recursively adds to the string for the right branch
		char **right = generateEncodeTableHelper(branch->right);
		currentLeft--;

		// Changes the code(s) for the current branch
		char **temp;

		// Left branch of current branch
		if(currentLeft == 0){
			temp = codes;
		}else{
			temp = (leftPointers[currentLeft - 1] + 1);
		}
		while(temp <= leftPointers[currentLeft]){
			prepend(*temp, '0');
			temp++;
		}

		// Right branch of current branch
		temp = leftPointers[currentLeft] + 1;
		while(temp <= right){
			prepend(*temp, '1');
			temp++;
		}

		return right;
	}
}

/* Takes the root node of the Huffman tree as input. Produces an array of strings
 * where the first character is a character from a leaf and the rest are the
 * Huffman code for the character.
 */
char **generateEncodeTable(struct node *root){
	int codeLength = 1 + charBit + 1; // character being encoded (1) + charBit (8) + null character (1)
	char *tempString = malloc(codeLength * length * sizeof(char));
	char **tempCodes = malloc(length * sizeof(char *));
	char ***tempLeft = malloc(charBit * sizeof(char **));
	if(tempString == NULL || tempCodes == NULL || tempLeft == NULL){mallocError(5);}

	// String preparation
	leftPointers = tempLeft;
	leftPointers[currentLeft] = codes = tempCodes;
	string = tempString;
	for(int i = 0; i < length; i++){
		string[codeLength * i] = '\0';
		codes[i] = &string[codeLength * i];
	}

	// Recursive algorithm to create the codes
	generateEncodeTableHelper(root);
	free(tempLeft);

	// Moves the character for each code to the front
	for(int i = 0; i < length; i++){
		char *temp = codes[i];
		while(*temp != '\0'){
			temp++;
		}
		char *chrP = temp - 1;
		char chr = *chrP;
		*chrP = '\0';
		prepend(codes[i], chr);
	}

	return codes;
}
