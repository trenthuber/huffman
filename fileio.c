#include <stdio.h>

#include "global.h"

void checkEncodeBuffer(void){
    if(bufferSize == 8){
        fwrite(&buffer, 1, 1, output);
        bufferSize = 0;
    }
}

void checkDecodeBuffer(void){

}

void writeBit(int bit){
    buffer = buffer << 1;
    if(bit == 1){
        buffer++;
    }
    bufferSize++;
    checkEncodeBuffer();
}

int readBit(void){
	return -1;
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

unsigned char readChar(void){
	return 0;
}
