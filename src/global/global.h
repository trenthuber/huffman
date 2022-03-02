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

extern int length;
extern int fileSize;
extern int codeLength;

extern FILE *input;
extern FILE *output;

extern void mallocError(char *filename, int num);
extern void closeAll(void);

#endif
