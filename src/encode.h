#ifndef ENCODE
#define ENCODE

extern unsigned char buffer;
extern int bufferSize;

extern void encodeTree(struct node *branch);
extern void encodeFile(char **codes);
extern void encode(void);

#endif
