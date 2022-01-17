#include <stdio.h>
#include <stdlib.h>

#include "global/global.h"

/* Finds the index of a character in an array of characters
 * (I know strchr() exists, but I want an int to return)
 */
int searchArray(char array[], char symbol){
	for(int i = 0; i < ASCII_SIZE; i++){
		if(symbol == array[i]){
			return i;
		}
	}
	return -1;
}

/* Reads characters from the input file and creates a list
 * of the total unique characters and the frequency those
 * characters appear in the file
 */
void makeLists(char **chrs, int **ints){
	char next;
	int newIndex = 0;
	int lineNumber = 1;
	while((next = fgetc(input)) != EOF){
		if((unsigned char) next >= (int) ASCII_SIZE){
			printf("huffman: This file contains non-standard ASCII characters at line %d\n", lineNumber);
			exit(-1);
		}
		if(next == '\n'){lineNumber++;}
		int oldIndex;
		if((oldIndex = searchArray(*chrs, next)) != -1){
			(*ints)[oldIndex]++;
		}else{
			(*chrs)[newIndex] = next;
			(*ints)[newIndex] = 1;
			newIndex++;
		}
	}

	// Saves the total unique characters in the file in length
	length = newIndex;
}

/* Selection sort algorithm that sorts both lists based on
 * the array of integers from largest to smallest (lists 
 * will never be longer than 128 elements, so selection sort
 * works fine, in my inexperience)
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
