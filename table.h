#ifndef TABLE
#define TABLE

extern char **codes;
extern char *string;
extern int currentCode;
extern char ***leftPointers;
extern int currentLeft;

extern void prepend(char *string, char chr);
extern char **generateTableHelper(struct node *branch);
extern char **generateTable(struct node *root);

#endif
