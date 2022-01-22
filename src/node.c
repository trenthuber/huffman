#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void swapNodes(struct node *one, struct node *two){
	struct node *temp = one;
	one = two;
	two = temp;
}

int partitionNodes(struct node **nodePointers, int start, int stop){
	int randInt = (rand() % (stop - start + 1)) + start;
	swapNodes(nodePointers[randInt], nodePointers[stop]);
	struct node *pivot = nodePointers[stop];

	int i = start;
	for(int j = i; j < stop; j++){
		if(nodePointers[j]->weight > pivot->weight){
			swapNodes(nodePointers[i], nodePointers[j]);
			i++;
		}
	}
	swapNodes(nodePointers[i], pivot);
	return i;
}

void quicksortNodes(struct node **nodePointers, int start, int stop){
	if(start < stop){
		int pivot = partitionNodes(nodePointers, start, stop);
		quicksortNodes(nodePointers, start, pivot - 1);
		quicksortNodes(nodePointers, pivot + 1, stop);
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
		fprintf(stderr, "huffman: File does not contain enough text to encode\n");
		exit(-1);
	}

	/* It would be easier to sort and shift 8 byte pointers to nodes rather than
	 * sorting and shifting 30 byte nodes themselves. At the expense of what is
	 * relatively little memory, we increase runtime for this section almost
	 * fourfold
	 */
	struct node **nodePointers = (struct node **) malloc(length * sizeof(struct node *));
	if(nodePointers == NULL){mallocError("node.c", 1);}

	for(int i = 0; i < length; i++){
		*(nodePointers + i) = &nodes[i]; // Pointer arithmetic to the rescue!!!
	}

	srand(time(NULL));
	quicksortNodes(nodePointers, 0, length - 1);
	
	return nodePointers;
}
