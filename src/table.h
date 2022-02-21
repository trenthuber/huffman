#ifndef TABLE
#define TABLE

extern char *codes;
extern char *prefix;

extern void freeTable(char **codes);

extern void makeTableHelper(struct node *branch);
extern char **makeTable(struct node *root);

#endif
