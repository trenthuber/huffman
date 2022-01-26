#ifndef TABLE
#define TABLE

extern char *codes;
extern unsigned char currentChar;

extern void freeTable(char **codes);
extern void prepend(char *string, char chr);
extern void makeCode(struct node *node);
extern char **makeTable(struct node *root);

#endif
