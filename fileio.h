#ifndef FILEIO
#define FILEIO

extern unsigned char mask;
extern int bufferSize;
extern unsigned char buffer;
extern unsigned char n1buffer;
extern unsigned char n2buffer;
extern int endOfFile;

extern void setUpBuffersRead(void);
extern void checkBufferWrite(void);
extern int checkBufferRead(void);
extern void writeBit(int bit);
extern int readBit(void);
extern void writeChar(unsigned char uchar);
extern unsigned char readChar(void);

#endif
