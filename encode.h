#ifndef ENCODE
#define ENCODE

extern unsigned char buffer;
extern int bufferSize;

extern void checkBuffer(void);
extern void encodeBit(int bit);
extern void encodeChar(char chr);
extern void encodeHeaderHelper(struct node *branch);
extern void encodeHeader(struct node *root);
extern void encodeString(char *code);
extern void encodeBody(char **codes);
extern void encodeCloser(void);
extern void encodeFile(struct node *root, char **codes);

#endif