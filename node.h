#ifndef NODE
#define NODE

extern struct node createNode(int weight, char symbol, struct node *left, struct node *right);
extern struct node **makeNodes(char chrs[], int ints[]);

#endif
