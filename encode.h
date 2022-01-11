#ifndef ENCODE
#define ENCODE

extern unsigned char buffer;
extern int bufferSize;

extern void encodeTreeHelper(struct node *branch);
extern void encodeTree(struct node *root);
extern void encodeFile(char **codes);
extern void encode(void);

#endif
