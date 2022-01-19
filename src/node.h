#ifndef NODE
#define NODE

extern struct node makeNode(unsigned char symbol, int weight, unsigned char type, struct node *left, struct node *right, struct node *parent);
extern void sortNodes(struct node **nodePointers);
extern struct node **makeNodes(int *ints);

#endif
