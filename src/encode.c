#include <stdio.h>
#include <string.h>

#include "global/global.h"
#include "global/fileio.h"
#include "tree.h"
#include "table.h"

/* Recursively traverses the Huffman tree, writing 
 * a 0 for a parent node and a 1 for a leaf node
 */
void encodeTree(struct node *branch){

    // If current node is an internal node
    if(branch->type == 0 || branch->type == 1 || branch->type == 2){
        writeBit(0);
        encodeTree(branch->left);
        encodeTree(branch->right);
    
    // If current node is a leaf node
    }else{
        writeBit(1);
        writeChar(branch->symbol);
    }
}

void encodeFile(struct node *root){
	char *codes = makeTable(root);
    int current;

    while((current = fgetc(input)) != EOF){
        for(int i = 0; i < (int) strlen(codes + (codeLength * current)); i++){
            (codes + (codeLength * (int) current))[i] == '0' ? writeBit(0) : writeBit(1);
        }
    }

	// Padding for the last byte
    writeBit(1);
    int numZeros = (8 - bufferSize) % 8;
    for(int i = 0; i < numZeros; i++){
        writeBit(0);
    }
}

void encode(void){
	struct node *root = makeTreeEncode();

    encodeTree(root);
    encodeFile(root);
}
