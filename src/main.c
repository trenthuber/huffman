#include <stdio.h>
#include <stdlib.h>

/* Checks the length of unsigned ints to make sure 
 * the operating system is ultimately 64-bit
 */
#if UINTPTR_MAX != 0xffffffffffffffff
	fprintf(stderr, "huffman: 32-bit operating system not supported (main.c, 0)\n");
	exit(-1);
#endif

#include "global/global.h"
#include "options.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char **argv){
	
	// Explicitly closes files when exiting
	atexit(closeAll);

	// This function also handles file IO
	handleOptions(argc, argv) == 0 ? encode() : decode();

	return 0;
}