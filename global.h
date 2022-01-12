#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 128 // Only works with files with normal ASCII characters

struct node{
	int weight;
	char symbol;
	struct node *left;
	struct node *right;
};

extern int length;
extern FILE *input;
extern FILE *output;

extern void mallocError(int num);

#endif
