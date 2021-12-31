#include <stdio.h>

#include "global.h"
#include "nodeDef.h"

FILE *out;

unsigned char buffer;
int bufferSize = 0;

void checkBuffer(void){
    if(bufferSize == 8){
        fwrite(&buffer, 1, 1, out);
        bufferSize = 0;
    }
}

void writeBit(int bit){
    buffer = buffer << 1;
    if(bit == 1){
        buffer++;
    }
    bufferSize++;
    checkBuffer();
}

void writeChar(unsigned char uchar){
    int begin = bufferSize;
    
    unsigned char mask = 0b10000000;
    do{
        unsigned char leading = mask & uchar;

        if(leading == '\0'){
            writeBit(0);
        }else{
            writeBit(1);
        }

        uchar = uchar << 1;
    }while(bufferSize != begin);
}

void writeHeader(struct node *root){

}

void writeBody(char **codes){

}

void writeCloser(void){

}

void writeFile(struct node *root, char **codes, FILE *output){
    out = output;
    writeHeader(root);
    writeBody(codes);
    writeCloser();
}