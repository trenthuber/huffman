#include <stdio.h>
#include <stdlib.h>

#include "global/global.h"

struct node makeNode(unsigned char symbol, int weight, unsigned char type, struct node *left, struct node *right, struct node *parent){
	struct node newNode;
	newNode.symbol = symbol;
	newNode.weight = weight;
	newNode.type = type;
	newNode.left = left;
	newNode.right = right;
	newNode.parent = parent;
	return newNode;
}

/* The algorithm sorts an array of pointers to nodes
 * (because this list will never be longer than 256
 * nodes, I figured a selection sort algorithm should
 * be fine)
 */
void sortNodes(struct node **nodePointers){
	struct node *temp;

	for(int i = 0; i < length; i++){
		int maxFreq = 0;
		int maxIndex = 0;
		for(int j = i; j < length; j++){
			if(nodePointers[j]->weight > maxFreq){
				maxFreq = nodePointers[j]->weight;
				maxIndex = j;
			}
		}
		if(maxIndex != i){
			temp = nodePointers[maxIndex];
			nodePointers[maxIndex] = nodePointers[i];
			nodePointers[i] = temp;
		}
	}
}

// Creates a sorted array of pointers to nodes based on lists of chars and ints
struct node **makeNodes(int *ints){
	struct node *nodes = (struct node *) malloc(ASCII_SIZE * sizeof(struct node));
	if(nodes == NULL){mallocError("node.c", 0);}
	
	for(int i = 0; i < ASCII_SIZE; i++){
		if(ints[i] != 0){

			/* Killing two birds with one stone: using length to signify the
			 * end of the list, and also leaving it as it's value for the
			 * rest of the encode process
			 */
			nodes[length] = makeNode((unsigned char) i, ints[i], (unsigned char) 5, NULL, NULL, NULL);
			length++;
		}
	}

	// Check we have enough unique symbols to make a tree
	if(length <= 1){
		printf("huffman: File does not contain enough text to encode\n");
		exit(-1);
	}

	/* Now that we have the length variable, we can create the array of 
	 * pointers to nodes used for the rest of encode process
	 */
	struct node **nodePointers = (struct node **) malloc(length * sizeof(struct node *));
	if(nodePointers == NULL){mallocError("node.c", 1);}

	for(int i = 0; i < length; i++){
		*(nodePointers + i) = &nodes[i]; // Pointer arithmetic to the rescue!!!
	}

	sortNodes(nodePointers);

	return nodePointers;
}
