#ifndef TABLE
#define TABLE

extern char *string;
extern char **codes;
extern unsigned char currentChar;

extern void prepend(char *string, char chr);
extern char **makeTableHelper(struct node *branch);
extern char **makeTable(struct node *root);

#endif
