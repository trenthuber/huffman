#ifndef GLOBAL
#define GLOBAL

struct node{
	int weight;
	char symbol;
	struct node *left;
	struct node *right;
};

extern int length;
extern int charBit;
extern int asciiSize;
extern FILE *input;
extern FILE *output;

extern unsigned char buffer;
extern int bufferSize;

extern void mallocError(int num);

#endif
