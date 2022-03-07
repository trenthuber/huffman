#include <stdio.h>
#include <string.h>

#include "global/global.h"
#include "global/fileio.h"
#include "tree.h"
#include "table.h"

#include "encode.h"

/* Recursively traverses the Huffman tree, writing 
 * a 0 for a parent node and a 1 for a leaf node
 */
void encodeTree(struct node *branch){

    // If current node is a leaf
    if(branch->left == NULL && branch->right == NULL){
        writeBit(1);
        writeChar(branch->symbol);
    
    // If current node is internal
    }else{
        writeBit(0);
        encodeTree(branch->left);
        encodeTree(branch->right);
    }
}

void encodeFile(struct node *root){
	char **codes = makeTable(root);
    freeTree(root);
    
    int current;
    while((current = fgetc(input)) != EOF){
        for(int i = 0; i < (int) strlen(codes[(int) current]); i++){
            codes[(int) current][i] == '0' ? writeBit(0) : writeBit(1);
        }
    }
    freeTable(codes);

	// Padding for the last byte
    writeBit(1);
    int numZeros = (8 - getBufferSize()) % 8;
    for(int i = 0; i < numZeros; i++){
        writeBit(0);
    }
}

void encode(void){
	struct node *root = makeTreeEncode();

    encodeTree(root);
    encodeFile(root);
}
