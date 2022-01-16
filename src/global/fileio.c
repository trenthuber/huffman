#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "global.h"

unsigned char mask = 0b10000000; // Read buffer from left to right
int bufferSize = 0; // Assume buffer is empty to start
unsigned char buffer;

// Just for the read functions
unsigned char n1buffer;
unsigned char n2buffer;
int endOfFile = 0;

void setUpBuffersRead(void){
    int intBuffer = fgetc(input);
    int intN1buffer = fgetc(input); // Character after buffer
    int intN2buffer = fgetc(input); // Character after n1buffer

    // Making sure file isn't too small to decode
    if(intN2buffer == EOF){
        printf("huffman: Can't decode, file is too small\n");
        exit(-1);
    }

    buffer = (unsigned char) intBuffer;
    n1buffer = (unsigned char) intN1buffer;
    n2buffer = (unsigned char) intN2buffer;

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
        buffer = n1buffer;
        n1buffer = n2buffer;
        int current;
        if((current = fgetc(input)) != EOF){
            n2buffer = (unsigned char) current;
            bufferSize = 8; // Buffer is now full
        }else{

            // Saves the last byte of the file as a number
            int newBufferSize = (int) n1buffer;
            if(newBufferSize > 7){

                /* The last byte of the encoded file represents what the buffer 
                 * size should be for the second to last byte (which is the last
                 * byte of encoded information). If this number is larger that the
                 * buffer, we can't decode
                 */
                printf("huffman: Can't decode, invalid last byte\n");
                exit(-1);
            }

            // Assigns the aforementioned last bufferSize
            bufferSize = newBufferSize;
            endOfFile = 1; // Want to exit the NEXT time bufferSize is 0
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
            printf("huffman: Can't decode, file only contains a header\n");
            exit(-1);
        }
    }
    return uchar;
}
