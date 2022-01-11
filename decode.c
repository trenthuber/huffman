#include <stdio.h>

#include "global.h"
#include "fileio.h"
#include "getTree.h"

void decodeFile(struct node *root){
	// YEP.
}

void decode(void){
	struct node *root = getTree();

	decodeFile(root);
}
