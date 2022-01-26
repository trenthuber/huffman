#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"
#include "global/fileio.h"
#include "node.h"

int tempLength;

/* Removes the last two nodes from the list, creates a new node
 * that points to those two nodes with their combined weight, 
 * and then inserts this node back into the sorted list.
 */
void makeTreeEncodeHelper(struct node ***nodePointers){

	// Setting up the new node
	int newWeight = (*nodePointers)[tempLength - 1]->weight + (*nodePointers)[tempLength - 2]->weight;
	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	if(newNode == NULL){mallocError("tree.c", 0);}

	*newNode = makeNode('\0', newWeight, 0, (*nodePointers)[tempLength - 1], (*nodePointers)[tempLength - 2], NULL);

	// Setting up the rest of the nodes (parent nodes and which side they're on)

	// Left node set up
	(*nodePointers)[tempLength - 1]->type = 1;
	(*nodePointers)[tempLength - 1]->parent = newNode;

	// Right node set up
	(*nodePointers)[tempLength - 2]->type = 2;
	(*nodePointers)[tempLength - 2]->parent = newNode;

	// Have the last pointer point to nothing since it's no longer used (for good measure)
	(*nodePointers)[tempLength - 1] = NULL;

	// Place the new node in the sorted list
	int end = 0;
	if(tempLength > 1){
		end = tempLength - 2;
		while((end > 0) && ((*nodePointers)[end - 1]->weight < newNode->weight)){
			(*nodePointers)[end] = (*nodePointers)[end - 1]; // Actual shifting of the pointers
			end--;
		}
	}

	(*nodePointers)[end] = newNode;
	tempLength--; // Overall, the length of the list has been decreased by one
}

/* Converts list of characters and integers to a Huffman tree of nodes
 * that consist of these characters and their frequency
 */
struct node *makeTreeEncode(void){
	int *ints = (int *) malloc(ASCII_SIZE * sizeof(int));
	if(ints == NULL){mallocError("tree.c", 1);}
	for(int i = 0; i < ASCII_SIZE; i++){
		ints[i] = 0;
	}

	// Also sets the length (total number of unique characters in the input file)
	int current;
	while((current = fgetc(input)) != EOF){
		ints[current]++;
	}

	rewind(input);

	// Also converts lists to a list of nodes
	struct node **nodePointers = makeNodes(ints);

	free(ints);

	// Creating Huffman tree from nodeList
	tempLength = length;
	while(tempLength > 1){
		makeTreeEncodeHelper(&nodePointers); // Pass by reference
	}

	// Returns the root node of the tree
	return nodePointers[0];
}

/* Does the actual decoding by reading the file and recursively
 * constructing the tree in the same way it was encoded
 */
void makeTreeDecodeHelper(struct node *branch){
	struct node *left = malloc(sizeof(struct node));
	struct node *right = malloc(sizeof(struct node));
	if(left == NULL || right == NULL){mallocError("tree.c", 2);}

	// Decode the LEFT branch
	int nextBit = readBit();

	// Case when left node is an internal node
	if(nextBit == 0){
		*left = makeNode('\0', 0, 1, NULL, NULL, branch);
		branch->left = left;
		makeTreeDecodeHelper(branch->left);
	
	// Case when left node is a leaf node
	}else{
		unsigned char nextChar = readChar();
		*left = makeNode(nextChar, 0, 1, NULL, NULL, branch);
		branch->left = left;
	}

	// Decode the RIGHT branch
	nextBit = readBit();

	// Case when right node is an internal node
	if(nextBit == 0){
		*right = makeNode('\0', 0, 2, NULL, NULL, branch);
		branch->right = right;
		makeTreeDecodeHelper(branch->right);

	// Case when right node is a leaf node
	}else{
		unsigned char nextChar = readChar();
		*right = makeNode(nextChar, 0, 2, NULL, NULL, branch);
		branch->right = right;
	}
}

/* Reads in the bits from the input and constructs the
 * Huffman tree to be used to decode the output file
 */
struct node *makeTreeDecode(void){
	setUpBuffersRead();
	if(readBit() != 0){ // First bit should always be 0 since every tree starts with a root node
		fprintf(stderr, "huffman: Can't decode, invalid first byte\n");
		exit(-1);
	}
	
	// Making the root node for the tree
	struct node *root = malloc(sizeof(struct node));
	if(root == NULL){mallocError("tree.c", 3);}
	*root = makeNode('\0', 0, 0, NULL, NULL, NULL);

	// Making the rest of the tree
	makeTreeDecodeHelper(root);
	
	return root;
}
