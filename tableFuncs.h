#ifndef STRINGFUNCTIONS
#define STRINGFUNCTIONS

extern char **codes;
extern char *string;
extern int currentCode;
extern char ***leftPointers;
extern int currentLeft;

extern void prepend(char *string, char chr);
extern char **treeToTableHelper(struct node *branch);
extern char **treeToTable(struct node *root);

#endif
