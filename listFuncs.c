#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

int searchArray(char array[], char symbol){
	for(int i = 0; i < asciiSize; i++){
		if(symbol == array[i]){
			return i;
		}
	}
	return -1;
}

int createLists(char **chrs, int **ints, FILE *input){
	char next;
	int newIndex = 0;
	while((next = fgetc(input)) != EOF){
		if((unsigned char) next >= asciiSize){
			printf("huffman: This file contains non-standard ASCII characters.\n");
			exit(-1);
		}
		int oldIndex;
		if((oldIndex = searchArray(*chrs, next)) != -1){
			(*ints)[oldIndex]++;
		}else{
			(*chrs)[newIndex] = next;
			(*ints)[newIndex] = 1;
			newIndex++;
		}
	}
	return newIndex; // Returns the length of the arrays
}

/* List will never be longer than 256 (asciiSize) elements, so
 * selection sort works fine in my opinion.
 */
void sort(char **chrs, int **ints){
	for(int i = 0; i < length; i++){
		int max = 0;
		int maxIndex = 0;
		for(int j = i; j < length; j++){
			if((*ints)[j] > max){
				max = (*ints)[j];
				maxIndex = j;
			}
		}
		if(maxIndex != i){
			char tempChar = (*chrs)[maxIndex];
			(*chrs)[maxIndex] = (*chrs)[i];
			(*chrs)[i] = tempChar;
			int tempInts = (*ints)[maxIndex];
			(*ints)[maxIndex] = (*ints)[i];
			(*ints)[i] = tempInts;
		}
	}
}
