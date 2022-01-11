#include <stdio.h>

#include "global.h"
#include "options.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[]){

	// The options function also deals with file IO
	handleOptions(argc, argv) == 0 ? encode() : decode();

	// Closing thoughts
	fclose(input);
	fclose(output);
	return 0;
}
