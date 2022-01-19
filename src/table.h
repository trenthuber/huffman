#ifndef TABLE
#define TABLE

extern char *string;
extern char **codes;
extern unsigned char currentChar;

extern void prepend(char *string, char chr);
extern void makeCode(struct node *node);
extern char **makeTable(struct node *root);

#endif
