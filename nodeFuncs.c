#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "node.h"

struct node createNode(int weight, char symbol, struct node *left, struct node *right){
	struct node newNode;
	newNode.weight = weight;
	newNode.symbol = symbol;
	newNode.left = left;
	newNode.right = right;
	return newNode;
}

struct node **makeNodes(char chrs[], int ints[]){
	struct node **nodeList = (struct node **) malloc(length * sizeof(struct node *));
	if(nodeList == NULL){mallocError(2);}

	for(int i = 0; i < length; i++){ 
		struct node *ptemp = (struct node *) malloc(sizeof(struct node)); // Grabbing a UNIQUE space in memory for each node
		if(ptemp == NULL){mallocError(3);}

		*ptemp = createNode(ints[i], chrs[i], NULL, NULL); // Makes that space point to a UNIQUE node
		nodeList[i] = ptemp; // Makes the pointer in nodeList[i] point to this UNIQUE space
		// DEBUG
		// printf("%p -> %p\n", &nodeList[i], nodeList[i]);
	}
	return nodeList;
}
