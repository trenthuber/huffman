#ifndef TREE
#define TREE

extern struct node *makeTreeEncode(void);
extern struct node *makeTreeDecode(void);
extern void freeTree(struct node *branch);

#endif
