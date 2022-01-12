#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "global.h"

unsigned char mask = 0b10000000;
int bufferSize;
unsigned char buffer;

// For read functions
unsigned char n1buffer; // Next char 1 space away
unsigned char n2buffer; // Next char 2 spaces away
int endOfFile = 0;

void setUpBuffersRead(void){
    int intBuffer = fgetc(input);
    int intN1buffer = fgetc(input);
    int intN2buffer = fgetc(input);

    if(intN2buffer == EOF){
        printf("huffman: Can't decode (1)\n");
        exit(-1);
    }

    buffer = (unsigned char) intBuffer;
    n1buffer = (unsigned char) intN1buffer;
    n2buffer = (unsigned char) intN2buffer;

    bufferSize = 8;
}

void checkBufferWrite(void){
    if(bufferSize == 8){
        fwrite(&buffer, 1, 1, output);
        bufferSize = 0; // Buffer is now empty
    }
}

int checkBufferRead(void){
    if(bufferSize == 0){
        if(endOfFile){
            return -1;
        }
        buffer = n1buffer;
        n1buffer = n2buffer;
        int current;
        if((current = fgetc(input)) != EOF){
            n2buffer = (unsigned char) current;
            bufferSize = 8; // Buffer is now full
        }else{

            // Deals with the final byte of the file
            int newBufferSize = (int) n1buffer;
            if(newBufferSize > 7){
                printf("huffman: Can't decode file (2)\n");
                exit(-1);
            }

            // Last byte what the size of the buffer should be for the last byte
            bufferSize = newBufferSize;
            endOfFile = 1; // Want to exit the NEXT time the bufferSize is
        }
    }
    return 0;
}

void writeBit(int bit){
    buffer = buffer << 1;
    if(bit == 1){
        buffer++;
    }
    bufferSize++;
    checkBufferWrite();
}

int readBit(void){
    unsigned char leading = mask & buffer;
    buffer = buffer << 1;
    bufferSize--;
    if(checkBufferRead() == -1){
        return -1;
    }
    if(leading == '\0'){
        return 0;
    }
    return 1;
}

void writeChar(unsigned char uchar){
    for(int i = 0; i < CHAR_BIT; i++){
        unsigned char leading = mask & uchar;
        if(leading == '\0'){
            writeBit(0);
        }else{
            writeBit(1);
        }
        uchar = uchar << 1;
    }
}

unsigned char readChar(void){
    unsigned char uchar;
	for(int i = 0; i < CHAR_BIT; i++){
        int bit = readBit();
        if(bit != -1){
            uchar = uchar << 1;
            if(bit == 1){
                uchar++;
            }
        }else{

            /* Since we only read characters when constructing the tree from
             * the header, we should never reach the end of the file while 
             * reading characters
             */
            printf("huffman: Can't decode file (3)\n");
            exit(-1);
        }
    }
    return uchar;
}
