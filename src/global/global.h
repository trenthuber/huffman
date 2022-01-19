#ifndef GLOBAL
#define GLOBAL

#define ASCII_SIZE 256 // Number of different types of bytes

struct node{
	unsigned char symbol;
	unsigned long weight;

	/* 0 for root node (or generic internal node),
	 * 1 for left internal node, 2 for right internal node,
	 * 3 for left leaf node, 4 for right leaf node,
	 * 5 for generic leaf node
	 */
	unsigned char type;

	struct node *left;
	struct node *right;

	// Used to traverse up the tree when finding the codes
	struct node *parent;
};

extern int length;
extern unsigned long fileSize;
extern FILE *input;
extern FILE *output;

extern void mallocError(char *filename, int num);
extern void closeAll(void);

#endif
