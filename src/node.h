#ifndef NODE
#define NODE

extern struct node *makeNode(unsigned char symbol, int weight, unsigned char type, struct node *left, struct node *right);
extern struct node **makeNodes(int *ints);

#endif
