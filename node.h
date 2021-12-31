#ifndef NODE
#define NODE

struct node{
	int weight;
	char symbol;
	struct node *left;
	struct node *right;
};

#endif
