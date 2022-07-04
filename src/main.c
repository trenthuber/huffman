#include <stdio.h>
#include <stdlib.h>

#include "global/global.h"
#include "options.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char **argv){

	/* Checks the length of unsigned ints to make sure 
	* the operating system is ultimately 64-bit
	*/
	#if ! __x86_64__ || _WIN64
	fprintf(stderr, "huffman: 32-bit operating system not supported (main.c, 0)\n");
	exit(-1);
	#endif
	
	// Explicitly closes files when exiting
	atexit(closeAll);

	// This function also handles file IO
	handleOptions(argc, argv) == 0 ? encode() : decode();

	return 0;
}