#ifndef TREE
#define TREE

extern int tempLength;

extern void freeTree(struct node *branch);

extern struct node *makeTreeEncode(void);

extern void makeTreeDecodeHelper(struct node *branch);
extern struct node *makeTreeDecode(void);

#endif
