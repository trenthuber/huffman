#ifndef NODE
#define NODE

extern struct node *makeNode(unsigned char symbol, int weight, unsigned char type, struct node *left, struct node *right, struct node *parent);
extern void swapNodes(struct node *one, struct node *two);
extern int partitionNodes(struct node **nodes, int start, int stop);
extern void quicksortNodes(struct node **nodes, int start, int stop);
extern struct node **makeNodes(int *ints);

#endif
