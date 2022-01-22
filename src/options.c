#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "global/global.h"

// Flags for input, output, and decode (true = 1)
int iflag, oflag, dflag; // Automatically set to 0 since they're global

// Strings of the filenames (for filename error detection)
char *inputFN, *outputFN;

void printHelpMessage(void){
    printf(
        "huffman, version 1.0\n\n"
        "usage: huffman [-hd] file [-o pathname]\n\n"
        "h : show help message\n"
        "d : decompress the given file\n"
        "o [pathname] : output file (default is \"out.huf\" / \"out.txt\")\n"
    );
}

int handleOptions(int argc, char **argv){
    int opt;

    /* This variable is here to make sure this works both with the BSD (macOS) 
     * and the GNU (Linux) getopt() function. They both use the optind variable slightly 
     * differently, so I just implemented by own that always works the way it's 
     * suppose to.
     */
    int argvIndex = 1;

    while(argvIndex < argc){
        if((opt = getopt(argc, argv, "hdo:")) != -1){
            switch(opt){

                // Print help message
                case 'h':
                    printHelpMessage();
                    exit(0);

                // Assign output file
                case 'o':

                    // Checks to see if it has already assigned an output file
                    if(oflag == 0){
                        outputFN = optarg;
                        oflag = 1;
                        argvIndex++;
                    }else{
                        fprintf(stderr, "huffman: can only output to one file at a time\n");
                        exit(-1);
                    }
                    break;

                // Sets the decode flag to true
                case 'd':
                    dflag = 1;
                    break;
                
                // Something went wrong
                case '?':
                default:
                    fprintf(stderr, "huffman: non-valid option\n");
                    exit(-1);
            }
        }else{
            if(iflag == 0){
                iflag = 1;
                inputFN = argv[optind];
            }else{
                fprintf(stderr, "huffman: can only take input from one file\n");
                exit(-1);
            }
            optind++;
        }
        argvIndex++;
    }
    
    // Checks if there are no input files provided
    if(iflag == 0){
        fprintf(stderr, "huffman: no input file provided\n");
        exit(-1);
    }

    /* If an output file was not provided we create a new one based on whether
     * we're encoding or decoding
     */
    if(oflag == 0){
        if(dflag == 0){
            if((output = fopen("out.huf", "wb")) == NULL){
                fprintf(stderr, "huffman: output file could not be created\n");
                exit(-1);
            }
			outputFN = "out.huf";
        }else{
            if((output = fopen("out.txt", "wb")) == NULL){
                fprintf(stderr, "huffman: output file could not be created\n");
                exit(-1);
            }
			outputFN = "out.txt";
        }
    }

    // User can't compress/decompress a file onto itself
    if(strcmp(inputFN, outputFN) == 0){
        fprintf(stderr, "huffman: can not use the same file for input and output\n");
        exit(-1);
    }

	// Opening the input file
	if((input = fopen(inputFN, "rb")) == NULL){
		fprintf(stderr, "huffman: input file does not exist\n");
		exit(-1);
	}

    // Checks for file size for encoding process
    if(dflag == 0){
        fseek(input, 0, SEEK_END);
        if(ftell(input) > 2147483647){ // 2147483647 = (2^31) - 1 = longest number for a signed int
            fprintf(stderr, "huffman: file too large to encode\n");
            exit(-1);
        }
        fileSize = (int) ftell(input);
        rewind(input);
    }

    // Opening the output file
	if((output = fopen(outputFN, "wb")) == NULL){
		fprintf(stderr, "huffman: output file could not be created\n");
		exit(-1);
	}

    // We return the decode flag so main() can know what function to call
    return dflag;
}
