#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"
#include "global/fileio.h"
#include "node.h"
#include "list.h"

int tempLength;

/* Removes the last two nodes from the list, creates a new node
 * that points to those two nodes with their combined weight, 
 * and then inserts this node back into the sorted list.
 */
void makeTreeEncodeHelper(struct node ***nodeList){
	int newWeight = (*nodeList)[tempLength - 1]->weight + (*nodeList)[tempLength - 2]->weight;

	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	if(newNode == NULL){mallocError(4);}

	*newNode = makeNode(newWeight, '\0', (*nodeList)[tempLength - 1], (*nodeList)[tempLength - 2]);
	(*nodeList)[tempLength - 1] = NULL;

	int end;
	if(tempLength > 2){
		end = tempLength - 2;
		while((end > 0) && ((*nodeList)[end - 1]->weight < newNode->weight)){
			(*nodeList)[end] = (*nodeList)[end - 1];
			end--;
		}
	}else{
		end = 0;
	}
	(*nodeList)[end] = newNode;
	tempLength--; // Overall, the length of the list has been decreased by one
}

/* Converts list of characters and integers to a Huffman tree of nodes
 * that consist of these characters and their frequency
 */
struct node *makeTreeEncode(void){

	// Creating sorted list of characters and their frequency from input file
	char *symb = (char *) malloc(ASCII_SIZE * sizeof(char));
	int *freq = (int *) malloc(ASCII_SIZE * sizeof(int));
	if(symb == NULL || freq == NULL){mallocError(1);}

	// Also sets the length (total number of unique characters in the input file)
	makeLists(&symb, &freq);

	// Edge cases - need at least two chars for the algorithm to work
	if(length <= 1){
		printf("huffman: File does not contain enough text to encode\n");
		exit(-1);
	}

	// Organize by highest frequency
	sort(&symb, &freq);

	// Converting lists to list of nodes
	struct node **nodeList = makeNodes(symb, freq);

	free(symb);
	free(freq);

	// Creating Huffman tree from nodeList
	tempLength = length;
	while(tempLength > 1){
		makeTreeEncodeHelper(&nodeList);
	}

	// The sum of all the weights is the file size itself
	fileSize = nodeList[0]->weight;

	// Returns the root node of the tree
	return nodeList[0];
}

/* Does the actual decoding by reading the file and recursively
 * constructing the tree in the same way it was encoded
 */
void makeTreeDecodeHelper(struct node *branch){
	struct node *left = malloc(sizeof(struct node));
	struct node *right = malloc(sizeof(struct node));
	if(left == NULL || right == NULL){mallocError(5);}

	// Decode the LEFT branch
	int nextBit = readBit();

	// Case when left node is a parent node
	if(nextBit == 0){
		*left = makeNode(0, '\0', NULL, NULL);
		branch->left = left;
		makeTreeDecodeHelper(branch->left);
	
	// Case when left node is a leaf node
	}else{
		char nextChar = readChar();
		*left = makeNode(0, nextChar, NULL, NULL);
		branch->left = left;
	}

	// Decode the RIGHT branch
	nextBit = readBit();

	// Case when right node is a parent node
	if(nextBit == 0){
		*right = makeNode(0, '\0', NULL, NULL);
		branch->right = right;
		makeTreeDecodeHelper(branch->right);

	// Case when right node is a leaf node
	}else{
		char nextChar = readChar();
		*right = makeNode(0, nextChar, NULL, NULL);
		branch->right = right;
	}
}

/* Reads in the bits from the input and constructs the
 * Huffman tree to be used to decode the output file
 */
struct node *makeTreeDecode(void){
	setUpBuffersRead();
	if(readBit() != 0){ // First bit should always be 0 since every tree starts with a root node
		printf("huffman: Can't decode, invalid first byte\n");
		exit(-1);
	}
	
	// Making the root node for the tree
	struct node *root = malloc(sizeof(struct node));
	if(root == NULL){mallocError(6);}
	*root = makeNode(0, '\0', NULL, NULL);

	// Making the rest of the tree
	makeTreeDecodeHelper(root);
	
	return root;
}
