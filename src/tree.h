#ifndef TREE
#define TREE

extern int tempLength;

extern void freeTree(struct node *branch);
extern void makeTreeEncodeHelper(struct node ***nodesP);
extern struct node *makeTreeEncode(void);
extern void makeTreeDecodeHelper(struct node *branch);
extern struct node *makeTreeDecode(void);

#endif
