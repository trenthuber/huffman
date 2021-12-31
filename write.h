#ifndef WRITE
#define WRITE

extern unsigned char buffer;
extern int bufferSize;

extern void checkBuffer(void);
extern void writeBit(int bit);
extern void writeChar(char chr);
extern void writeHeader(struct node *root);
extern void writeBody(char **codes);
extern void writeCloser(void);
extern void writeFile(struct node *root, char **codes, FILE *output);

#endif