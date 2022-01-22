#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "global.h"

unsigned char mask = 0b10000000; // Read buffer from left to right
int bufferSize = 0; // Assume buffer is empty to start
unsigned char buffer;

// Just for the read functions
unsigned char nextBuffer;
int endOfFile = 0;

void setUpBuffersRead(void){
    int intBuffer = fgetc(input);
    int intNextBuffer = fgetc(input); // Character after buffer

    // Making sure file isn't too small to decode
    if(intNextBuffer == EOF){
        fprintf(stderr, "huffman: Can't decode, file is too small\n");
        exit(-1);
    }

    buffer = (unsigned char) intBuffer;
    nextBuffer = (unsigned char) intNextBuffer;

    bufferSize = 8; // Buffer is now full
}

/* Checks if the buffer has reached the bufferSize. If it has,
 * it "empties" the buffer by writing whatever character is in
 * it to the output file
 */
void checkBufferWrite(void){
    if(bufferSize == 8){
        fwrite(&buffer, 1, 1, output);
        bufferSize = 0; // Buffer is now empty
    }
}

/* Checks if the bufferSize is above 0. If it's not, it reads in
 * the next character from the input file, thus filling up the
 * buffer. If it gets to the end of the file, it does some further
 * decoding of the file to set bufferSize for the last encoded byte
 */
int checkBufferRead(void){
    if(bufferSize == 0){

        // Checks if we're at the end of the file
        if(endOfFile){
            return -1;
        }

        // Shifts the buffers along
        buffer = nextBuffer;
        int current;
        if((current = fgetc(input)) != EOF){
            nextBuffer = (unsigned char) current;
            bufferSize = 8; // Buffer is now full
        }else{
            endOfFile = 1;
            
            bufferSize = 0;

            // Edge case when the last byte IS the padding - nothing else to read
            if(buffer == 0x80){
                return -1;
            }

            nextBuffer = buffer;
            unsigned char endMask = 0b00000001;
            unsigned char ending = nextBuffer & endMask;
            int numZeros = 0;
            while(ending == 0){
                numZeros++;
                nextBuffer = nextBuffer >> 1;
                ending = nextBuffer & endMask;
            }

            /* Last byte ends with a string of "0"s that begin 
             * with a 1. Thus, (numZeros + 1)
             */
            bufferSize = 8 - (numZeros + 1);
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

    // Check if the bit we just wrote pushed the buffer over the bufferSize
    checkBufferWrite();
}

int readBit(void){

    /* Check if we can read anymore bits from the buffer FIRST
     * so we only read a bit if we can
     */
    if(checkBufferRead() == -1){
        return -1;
    }
    unsigned char leading = mask & buffer;
    buffer = buffer << 1;
    bufferSize--;
    if(leading == '\0'){
        return 0;
    }
    return 1;
}

/* Writes 8 bits to the output file. This function doesn't simply
 * write a character to the file because there will most likely 
 * be bits in the buffer that need to be written to the file before 
 * the character
 */
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

/* Reads 8 bits from the file, creating a character that it then 
 * returns. Doesn't simply read a character from the file because 
 * there will most likely be other bits in the buffer that need 
 * to be read as a part of the final character
 */
unsigned char readChar(void){
    unsigned char uchar = '\0';
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
            fprintf(stderr, "huffman: Can't decode, file only contains a header\n");
            exit(-1);
        }
    }
    return uchar;
}
