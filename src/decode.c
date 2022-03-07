#include <stdio.h>

#include "global/global.h"
#include "global/fileio.h"
#include "tree.h"

#include "decode.h"

void decode(void){
	struct node *root = makeTreeDecode();
	struct node *nodeP = root; // Used as a pointer to various nodes
	
	int bit = readBit();
	while(bit != -1){
		if(bit == 0){
			nodeP = nodeP->left;
		}else{
			nodeP = nodeP->right;
		}

		/* If the current node is a leaf node, write the 
		 * character to the output file and reset the node 
		 * pointer to the root node
		 */
		if(nodeP->left == NULL && nodeP->right == NULL){
			fputc(nodeP->symbol, output);
			nodeP = root;
		}

		// Get the next bit
		bit = readBit();
	}
}
