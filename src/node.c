#include <stdio.h>
#include <stdlib.h>

#include "global/global.h"

struct node makeNode(int weight, char symbol, struct node *left, struct node *right){
	struct node newNode;
	newNode.weight = weight;
	newNode.symbol = symbol;
	newNode.left = left;
	newNode.right = right;
	return newNode;
}

// Creates a array of pointers to nodes based on lists of chars and ints
struct node **makeNodes(char chrs[], int ints[]){
	struct node **nodeList = (struct node **) malloc(length * sizeof(struct node *));
	if(nodeList == NULL){mallocError(2);}

	for(int i = 0; i < length; i++){

		// Grabbing a UNIQUE space in memory for each node
		struct node *ptemp = (struct node *) malloc(sizeof(struct node));
		if(ptemp == NULL){mallocError(3);}

		*ptemp = makeNode(ints[i], chrs[i], NULL, NULL);
		nodeList[i] = ptemp;
	}
	return nodeList;
}
