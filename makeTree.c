#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "global.h"
#include "node.h"
#include "list.h"

int tempLength;

/* Removes the last two nodes from the list, creates a new node
 * that points to those two nodes with their combined weight, 
 * and then inserts this node back into the sorted list.
 */
void huffmanAlg(struct node ***nodeList){
	int newWeight = (*nodeList)[tempLength - 1]->weight + (*nodeList)[tempLength - 2]->weight;

	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	if(newNode == NULL){mallocError(4);}

	*newNode = createNode(newWeight, '\0', (*nodeList)[tempLength - 1], (*nodeList)[tempLength - 2]);
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

struct node *makeTree(void){
	charBit = CHAR_BIT;
	asciiSize = (int) pow(2.0, (double) charBit);

	// Creating sorted list of characters and their frequency from input file
	char *symb = (char *) malloc(asciiSize * sizeof(char));
	int *freq = (int *) malloc(asciiSize * sizeof(int));
	if(symb == NULL || freq == NULL){mallocError(1);}

	length = createLists(&symb, &freq);
	sort(&symb, &freq);

	// Converting lists to list of nodes
	struct node **nodeList = makeNodes(symb, freq);

	free(symb);
	free(freq);

	// Creating Huffman tree from nodeList
	tempLength = length;
	while(tempLength > 1){
		huffmanAlg(&nodeList);
	}

	return nodeList[0];
}
