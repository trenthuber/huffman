#ifndef FILEIO
#define FILEIO

extern void checkEncodeBuffer(void);
extern void checkDecodeBuffer(void);
extern void writeBit(int bit);
extern int readBit(void);
extern void writeChar(unsigned char uchar);
extern unsigned char readChar(void);

#endif
