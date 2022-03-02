#ifndef TABLE
#define TABLE

extern char *codes;
extern char *prefix;

extern void makeTableHelper(struct node *branch);
extern char **makeTable(struct node *root);
extern void freeTable(char **codes);

#endif
