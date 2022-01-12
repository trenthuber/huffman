#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "node.h"
#include "fileio.h"
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

struct node *makeTreeEncode(void){

	// Creating sorted list of characters and their frequency from input file
	char *symb = (char *) malloc(ASCII_SIZE * sizeof(char));
	int *freq = (int *) malloc(ASCII_SIZE * sizeof(int));
	if(symb == NULL || freq == NULL){mallocError(1);}

	length = createLists(&symb, &freq);

	// Edge cases
	if(length == 0){
		printf("huffman: File is empty\n");
		exit(-1);
	}
	if(length == 1){
		struct node *entry;
		*entry = createNode(1, symb[0], NULL, NULL);
		struct node *root;
		*root = createNode(1, '\0', entry, NULL);
		return root;
	}

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

	return nodeList[0];
}

void makeTreeDecodeHelper(void){

}

struct node *makeTreeDecode(void){

	// Adds one since we encoded it decremented one to catch the edge case
	int numLeaves = (int) (fgetc(input) + 1);
	setUpBuffersRead();
	
	

	struct node *test;
	return test;
}
