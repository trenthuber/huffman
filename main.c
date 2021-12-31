#include <stdio.h>
#include <stdlib.h> // For exit() function
#include <unistd.h> // For option handling

#include "options.h"
#include "global.h" // For input and output
#include "nodeDef.h"
#include "tree.h"
#include "table.h"
#include "encode.h"

int main(int argc, char *argv[]){
	if(handleOptions(argc, argv) == 0){
		// Encode option
		struct node *root = generateEncodeTree(input);
		char **codes = generateEncodeTable(root);
		encodeFile(root, codes);
	}else{
		// Decode option
	}
	// Closing thoughts
	fclose(input);
	fclose(output);
	return 0;
}