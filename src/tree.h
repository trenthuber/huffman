#ifndef TREE
#define TREE

extern int tempLength;

extern void makeTreeEncodeHelper(struct node ***nodePointers);
extern struct node *makeTreeEncode(void);
extern void makeTreeDecodeHelper(struct node *branch);
extern struct node *makeTreeDecode(void);

#endif
