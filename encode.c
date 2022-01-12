#include <stdio.h>
#include <string.h>

#include "global.h"
#include "tree.h"
#include "fileio.h"
#include "table.h"

void encodeTreeHelper(struct node *branch){
    if(branch->symbol == '\0'){
        writeBit(0);
        encodeTreeHelper(branch->left);
        encodeTreeHelper(branch->right);
    }else{
        writeBit(1);
        writeChar((unsigned char) branch->symbol);
    }   
}

void encodeTree(struct node *root){
    bufferSize = 0; // There is nothing important in the buffer to start

	/* The first byte written to the file is how many leaves the tree has
	 * minus 1, since 256 (the maximum possible leaves in an 8-bit alphabet)
	 * can't be written with a single byte
	 */
    writeChar((unsigned char) (length - 1)); 
    encodeTreeHelper(root);
}

void encodeFile(struct node *root){
	char **codes = makeTable(root);

	// Body
    char current;
    rewind(input); // File pointer to the next char was already moved in list.c
    while((current = fgetc(input)) != EOF){
        for(int i = 0; i < length; i++){
            if(current == codes[i][0]){
				for(int j = 1; j < strlen(codes[i]); j++){
					(codes[i][j] == '0') ? writeBit(0) : writeBit(1);
				}
            }
        }
    }

	// Closer
    int numZeros = 8 - bufferSize;
    if(bufferSize != 0){
        for(int i = 0; i < numZeros; i++){
            writeBit(0);
        }
    }else{
        numZeros = 0;
    }

	/* The last byte written to the file contains the buffer size of the
	 * previous byte (which is the last byte that contains encoded information)
	 */
    writeChar((unsigned char) (8 - numZeros));
}

void encode(void){
	struct node *root = makeTreeEncode();

    encodeTree(root);
    encodeFile(root);
}
