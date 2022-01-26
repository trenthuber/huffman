#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 256 // Number of different types of bytes

struct node{
	unsigned char symbol;
	int weight;

	// 1 for left, 2 for right (0 for nodes with no type)
	unsigned char type;

	struct node *left;
	struct node *right;

	// Used to traverse up the tree when finding the codes
	struct node *parent;
};

extern int length;
extern int fileSize;
extern int codeLength;
extern FILE *input;
extern FILE *output;

extern void mallocError(char *filename, int num);
extern void closeAll(void);

#endif
