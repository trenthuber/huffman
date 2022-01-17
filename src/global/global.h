#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 256 // Number of different types of bytes

struct node{
	int weight;
	char symbol;
	struct node *left;
	struct node *right;
};

extern int length;
extern int fileSize;
extern FILE *input;
extern FILE *output;

extern void mallocError(int num);
extern void closeAll(void);

#endif
