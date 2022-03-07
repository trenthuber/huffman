#ifndef FILEIO
#define FILEIO

// Encode functions
extern int getBufferSize(void);
extern void writeBit(int bit);
extern void writeChar(unsigned char uchar);

// Decode functions
extern void checkFileRead(void);
extern int readBit(void);
extern unsigned char readChar(void);

#endif
