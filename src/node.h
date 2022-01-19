#ifndef NODE
#define NODE

extern struct node makeNode(unsigned char symbol, unsigned long weight, unsigned char type, struct node *left, struct node *right, struct node *parent);
extern void sortNodes(struct node *nodeList);
extern struct node **makeNodes(unsigned long *longs);

#endif
