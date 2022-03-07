#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 256 // Number of unique bytes

struct node{
	unsigned char symbol;
	int weight;

	struct node *left;
	struct node *right;
};

struct heap{
	struct node **array;
	int size;
};

extern int codeLength;

// File variables
extern int length;
extern int fileSize;

// File pointers (for either encode and decode process)
extern FILE *input;
extern FILE *output;

// Error handling functions
extern void mallocError(char *filename, int num);
extern void closeAll(void);

#endif
