#include <stdio.h>
#include <stdlib.h>

#include "global/global.h"
#include "options.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[]){
	
	// Explicitly closes files when exiting
	atexit(closeAll);

	// This function also handles file IO
	handleOptions(argc, argv) == 0 ? encode() : decode();

	return 0;
}
