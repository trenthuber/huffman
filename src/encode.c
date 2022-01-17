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
    if(branch->symbol == '\0'){
        writeBit(0);
        encodeTree(branch->left);
        encodeTree(branch->right);
    }else{
        writeBit(1);
        writeChar((unsigned char) branch->symbol);
    }   
}

void encodeFile(struct node *root){
	char **codes = makeTable(root);
    char current;

    rewind(input); // Resets file pointer

    while((current = fgetc(input)) != EOF){
        for(int i = 0; i < length; i++){
            if(current == codes[i][0]){
				for(int j = 1; j < (int) strlen(codes[i]); j++){
					(codes[i][j] == '0') ? writeBit(0) : writeBit(1);
				}
            }
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
	struct node *root = makeTreeEncode(); // Makes the tree

    encodeTree(root); // Encodes the tree in the header of the file
    encodeFile(root);
}
