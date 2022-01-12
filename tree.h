#ifndef TREE
#define TREE

int tempLength;

extern void makeTreeEncodeHelper(struct node ***nodeList);
extern struct node *makeTreeEncode(void);
extern void makeTreeDecodeHelper(void);
extern struct node *makeTreeDecode(void);

#endif
