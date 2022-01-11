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
    writeChar((unsigned char) length); // First byte is the number of leaf nodes
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
                // printf("Chars: %c, %c\n", current, codes[i][0]);
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
    writeChar((unsigned char) (8 - numZeros));
}

void encode(void){
	struct node *root = makeTreeEncode();

    encodeTree(root);
    encodeFile(root);
}
