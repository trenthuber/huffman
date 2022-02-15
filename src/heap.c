#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global/global.h"

struct heap *makeHeap(void){
    struct heap *newHeap = (struct heap *) malloc(sizeof(struct heap));
    newHeap->array = (struct node **) calloc(length, sizeof(struct node *));
    newHeap->size = 0;
    return newHeap;
}

void swap(struct heap *heap, int parentIndex, int childIndex){
    struct node *temp = (heap->array)[parentIndex];
    (heap->array)[parentIndex] = (heap->array)[childIndex];
    (heap->array)[childIndex] = temp;
}

void insert(struct heap *heap, struct node *node){
    (heap->array)[heap->size] = node;
    int childIndex = heap->size;
    int parentIndex = floor((childIndex - 1) / 2);
    while(parentIndex >= 0){
        if((heap->array)[parentIndex]->weight > (heap->array)[childIndex]->weight){
            swap(heap, parentIndex, childIndex);
            childIndex = parentIndex;
            parentIndex = floor((childIndex - 1) / 2);
        }else{
            break;
        }
    }
    (heap->size)++;
}

struct node *delMin(struct heap *heap){
    if(heap->size == 0){
        return NULL;
    }
    (heap->size)--;

    // Swaps the min with the last node and removes the min
    swap(heap, 0, heap->size);
    struct node *minNode = (heap->array)[heap->size];

    // Downheaps the new root node
    int parentIndex = 0;
    int leftIndex = 1;
    int rightIndex = 2;
    while(rightIndex < heap->size){
        if(((heap->array)[parentIndex]->weight > (heap->array)[leftIndex]->weight) || ((heap->array)[parentIndex]->weight > (heap->array)[rightIndex]->weight)){
            if((heap->array)[leftIndex]->weight < (heap->array)[rightIndex]->weight){
                swap(heap, parentIndex, leftIndex);
                parentIndex = leftIndex;
            }else{
                swap(heap, parentIndex, rightIndex);
                parentIndex = rightIndex;
            }
            leftIndex = (parentIndex * 2) + 1;
            rightIndex = (parentIndex * 2) + 2;
        }else{
            break;
        }
    }

    // Edge case when only the right node is NULL but the left node isn't
    if((rightIndex == heap->size) && ((heap->array)[parentIndex]->weight > (heap->array)[leftIndex]->weight)){
        swap(heap, parentIndex, leftIndex);
    }

    return minNode;
}