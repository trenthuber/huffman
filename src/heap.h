#ifndef HEAP
#define HEAP

extern struct heap *makeHeap(void);
extern void freeHeap(struct heap *heap);

extern void swap(struct heap *heap, int parentIndex, int childIndex);
extern void insert(struct heap *heap, struct node *node);
extern struct node *delMin(struct heap *heap);

#endif