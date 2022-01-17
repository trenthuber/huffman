#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> // For CHAR_BIT
#include <math.h> // For calculating codeLength

#include "global/global.h"

char **codes;
char *string;
int currentCode = 0;
char ***leftPointers;
int currentLeft = 0;

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

/* Getting the max tree depth (i.e. how long the longest possible Huffman
 * code will be) is not as trivial as I first thought. There is no definitive
 * formula for calculating the max tree depth of a given file, but many 
 * heuristics can be put in place of a formula. This function finds the
 * minimum of two heuristics
 */
int getMaxTreeDepth(void){

	/* The first heuristic is based on the file size (calculated in list.c).
	 * This formula usually wins for larger file sizes
	 */
	int maxTreeDepth = (int) ceil(log2((double) fileSize + 1) - 1);

	if(length < maxTreeDepth){
		
		/* The second heuristic is simply based on the number of unique
		 * characters the file has. Since we only repeat the algorithm
		 * length - 1 times, we never will have more than depth of 
		 * length - 1
		 */
		return length - 1;
	}
	return maxTreeDepth;
}

/* In all honesty, this function took quite a long time to create and I
 * consider it to be somewhat magical; I don't really feel like explaining
 * this one in much detail. It essentially consists of a list and several
 * pointers pointing to various items in the list. It writes "0"s and "1"s
 * to certain ranges of values between pointers recursively depending on
 * where we are in the tree
 */
char **makeTableHelper(struct node *branch){
	if(branch->symbol != '\0'){
		prepend(codes[currentCode], branch->symbol);
		return &codes[currentCode++]; 
	}else{

		// Recursively adds to the string for the left branch
		leftPointers[currentLeft] = makeTableHelper(branch->left);
		currentLeft++;

		// Recursively adds to the string for the right branch
		char **right = makeTableHelper(branch->right);
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
 * where the first character of each string is a character from a leaf node and the
 * rest of the string is the Huffman code for the character
 */
char **makeTable(struct node *root){

	/*   character being encoded (1)
	 * + maximum path length encoding (see getMaxTreeDepth())
	 * + null character (1)
	 */
	int codeLength = 1 + getMaxTreeDepth() + 1;

	char *tempString = malloc(codeLength * length * sizeof(char));
	char **tempCodes = malloc(length * sizeof(char *));
	char ***tempLeft = malloc(CHAR_BIT * sizeof(char **));
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
	makeTableHelper(root);
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
