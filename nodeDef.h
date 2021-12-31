#ifndef NODEDEF
#define NODEDEF

struct node{
	int weight;
	char symbol;
	struct node *left;
	struct node *right;
};

#endif
