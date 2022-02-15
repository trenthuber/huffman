#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 256 // Number of different types of bytes

struct node{
	unsigned char symbol;
	int weight;

	// 0 for left, 1 for right
	unsigned char type;

	struct node *left;
	struct node *right;

	// Used to traverse up the tree when finding the codes
	struct node *parent;
};

// Used only in the encode process for a nice overall O(nlogn) time
struct heap{
	struct node **array;
	int size;
};

extern int length;
extern int fileSize;
extern int codeLength;
extern FILE *input;
extern FILE *output;

extern void mallocError(char *filename, int num);
extern void closeAll(void);

#endif
