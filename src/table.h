#ifndef TABLE
#define TABLE

extern char **codes;
extern char *string;
extern int currentCode;
extern char ***leftPointers;
extern int currentLeft;

extern void prepend(char *string, char chr);
extern char **makeTableHelper(struct node *branch);
extern char **makeTable(struct node *root);

#endif
