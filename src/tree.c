#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"
#include "global/fileio.h"
#include "node.h"
#include "heap.h"

#include "tree.h"

/* Converts list of characters and integers to a Huffman tree of nodes
 * that consist of these characters and their frequency
 */
struct node *makeTreeEncode(void){
	int *ints = (int *) calloc(ASCII_SIZE, sizeof(int));
	if(ints == NULL){mallocError("tree.c", 0);}
	
	// Also sets the length (total number of unique characters in the input file)
	int current;
	while((current = fgetc(input)) != EOF){
		ints[current]++;
	}

	rewind(input);

	// Converts a lists of ints to a list of pointers to nodes
	struct node **nodes = makeNodes(ints);

	free(ints);

	// Adds every node to a heap
	struct heap *heap = makeHeap();
	for(int i = 0; i < length; i++){
		insert(heap, nodes[i]);
	}

	/* Removes the two smallest elements of the heap, creates a new 
	 * tree with those elements, and adds it back to the heap
	 */
	for(int i = 0; i < length - 1; i++){
		struct node *first = delMin(heap);
		struct node *second = delMin(heap);
		struct node *newNode = makeNode('\0', first->weight + second->weight, first, second);

		insert(heap, newNode);
	}

	// Returns the root node of the tree while also freeing the heap
	struct node *root = heap->array[0];
	freeHeap(heap);
	return root;
}

/* Does the actual decoding by reading the file and recursively
 * constructing the tree in the same way it was encoded
 */
void makeTreeDecodeHelper(struct node *branch){
	struct node *left;
	struct node *right;

	// Decode the LEFT branch
	int nextBit = readBit();

	// Case when left node is an internal node
	if(nextBit == 0){
		left = makeNode('\0', 0, NULL, NULL);
		branch->left = left;
		makeTreeDecodeHelper(branch->left);
	
	// Case when left node is a leaf node
	}else{
		unsigned char nextChar = readChar();
		left = makeNode(nextChar, 0, NULL, NULL);
		branch->left = left;
	}

	// Decode the RIGHT branch
	nextBit = readBit();

	// Case when right node is an internal node
	if(nextBit == 0){
		right = makeNode('\0', 0, NULL, NULL);
		branch->right = right;
		makeTreeDecodeHelper(branch->right);

	// Case when right node is a leaf node
	}else{
		unsigned char nextChar = readChar();
		right = makeNode(nextChar, 0, NULL, NULL);
		branch->right = right;
	}
}

/* Reads in the bits from the input and constructs the
 * Huffman tree to be used to decode the output file
 */
struct node *makeTreeDecode(void){
	checkFileRead();
	if(readBit() != 0){ // First bit should always be 0 since every tree starts with a root node
		fprintf(stderr, "huffman: Can't decode, invalid first byte\n");
		exit(-1);
	}
	
	// Making the root node for the tree
	struct node *root = makeNode('\0', 0, NULL, NULL);

	// Making the rest of the tree
	makeTreeDecodeHelper(root);
	
	return root;
}

void freeTree(struct node *branch){
	if(branch != NULL){
		freeTree(branch->left);
		freeTree(branch->right);
		free(branch);
	}
}
