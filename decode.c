#include <stdio.h>

#include "global.h"
#include "fileio.h"
#include "tree.h"

void decodeFile(struct node *root){
	// YEP.
}

void decode(void){
	struct node *root = makeTreeDecode();

	decodeFile(root);
}
