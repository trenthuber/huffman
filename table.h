#ifndef E_TABLE
#define E_TABLE

extern char **codes;
extern char *string;
extern int currentCode;
extern char ***leftPointers;
extern int currentLeft;

extern void prepend(char *string, char chr);
extern char **generateEncodeTableHelper(struct node *branch);
extern char **generateEncodeTable(struct node *root);

#endif
