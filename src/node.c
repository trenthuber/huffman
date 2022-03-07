#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "global/global.h"

#include "node.h"

struct node *makeNode(unsigned char symbol, int weight, struct node *left, struct node *right){
	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	if(newNode == NULL){mallocError("node.c", 0);}
	newNode->symbol = symbol;
	newNode->weight = weight;
	newNode->left = left;
	newNode->right = right;
	return newNode;
}

// Creates a sorted array of pointers to nodes based on lists of chars and ints
struct node **makeNodes(int *ints){
	struct node **nodes = (struct node **) calloc(ASCII_SIZE, sizeof(struct node *));
	if(nodes == NULL){mallocError("node.c", 1);}
	
	for(int i = 0; i < ASCII_SIZE; i++){
		if(ints[i] != 0){

			/* Killing two birds with one stone: using length to signify the
			 * end of the list so far, and also leaving it as it's value for
			 * the rest of the encode process
			 */
			nodes[length] = makeNode((unsigned char) i, ints[i], NULL, NULL);
			length++;
		}
	}
	
	// Check we have enough unique symbols to make a tree
	if(length <= 1){
		fprintf(stderr, "huffman: File does not contain enough text to encode\n");
		exit(-1);
	}

	// Reallocating the memory to fit the length of the code
	nodes = realloc(nodes, length * sizeof(struct node *));
	if(nodes == NULL){mallocError("node.c", 2);}
	
	return nodes;
}
