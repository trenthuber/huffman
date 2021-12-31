#include <stdio.h>
#include <string.h>

#include "global.h"
#include "nodeDef.h"

unsigned char buffer;
int bufferSize = 0;

void checkBuffer(void){
    if(bufferSize == 8){
        fwrite(&buffer, 1, 1, output);
        bufferSize = 0;
    }
}

void encodeBit(int bit){
    buffer = buffer << 1;
    if(bit == 1){
        buffer++;
    }
    bufferSize++;
    checkBuffer();
}

void encodeChar(unsigned char uchar){
    int begin = bufferSize;
    
    unsigned char mask = 0b10000000;
    do{
        unsigned char leading = mask & uchar;

        if(leading == '\0'){
            encodeBit(0);
        }else{
            encodeBit(1);
        }

        uchar = uchar << 1;
    }while(bufferSize != begin);
}

void encodeHeaderHelper(struct node *branch){
    if(branch->symbol == '\0'){
        encodeBit(0);
        encodeHeaderHelper(branch->left);
        encodeHeaderHelper(branch->right);
    }else{
        encodeBit(1);
        encodeChar((unsigned char) branch->symbol);
    }   
}

void encodeHeader(struct node *root){
    encodeChar((unsigned char) length); // First byte is the number of leaf nodes
    encodeHeaderHelper(root);
}

void encodeString(char *code){
    for(int i = 1; i < strlen(code); i++){
        (code[i] == '0') ? encodeBit(0) : encodeBit(1);
    }
}

void encodeBody(char **codes){
    char current;
    rewind(input); // File pointer to the next char was already moved in list.c
    while((current = fgetc(input)) != EOF){
        for(int i = 0; i < length; i++){
            if(current == codes[i][0]){
                encodeString(codes[i]);
                // printf("Chars: %c, %c\n", current, codes[i][0]);
            }
        }
    }
}

void encodeCloser(void){
    int numZeros = 8 - bufferSize;
    if(bufferSize != 0){
        for(int i = 0; i < numZeros; i++){
            encodeBit(0);
        }
    }else{
        numZeros = 0;
    }
    encodeChar((unsigned char) (8 - numZeros));
}

void encodeFile(struct node *root, char **codes){
    encodeHeader(root);
    encodeBody(codes);
    encodeCloser();
}
