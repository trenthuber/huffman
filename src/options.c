#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "global/global.h"

// Flags for input, output, and decode (true = 1)
int iflag, oflag, dflag; // Automatically set to 0 since they're global

// Strings of the filenames (for getting filename extentions later)
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

char *getFileExt(char *filename){
	char *ext = strrchr(filename, '.');
	if(ext == NULL || ext == filename){
		return "";
	}
	return ext + 1;
}

int handleOptions(int argc, char *argv[]){
    int opt;
    while(optind < argc){
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
                    }else{
                        printf("huffman: can only output to one file at a time\n");
                        exit(-1);
                    }
                    break;

                // Sets the decode flag to true
                case 'd':
                    dflag = 1;
                    break;
            }
        
        /* In this case, we have an argument that is not an option or a parameter for
         * an option, so it must be the input filename
         */
        }else{

            // No input file given yet
            if(iflag == 0){
				inputFN = argv[optind];
                iflag = 1;
                optind++;
            
            // Too many input files given
            }else{
                printf("huffman: can only take input from one file at a time\n");
                exit(-1);
            }
        }
    }

    /* After parsing all the options, we check to make sure we at least were given 
     * an input file
     */
    if(iflag == 0){
        printf("huffman: No input file provided\n");
        exit(-1);
    }else{

        // If we were given an input file, we check that it has the correct filename extention
		char *inputExt = getFileExt(inputFN);
		char *normalExt;

		normalExt = (dflag == 0 ? "txt" : "huf");
		
		if(strcmp(inputExt, normalExt) != 0){
			printf("huffman: please use the correct filename extentions for the input file (.%s)\n", normalExt);
			exit(-1);
		}
	}

    /* If an output file was not provided we create a new one based on whether
     * we're encoding or decoding
     */
    if(oflag == 0){
        if(dflag == 0){
            if((output = fopen("out.huf", "w")) == NULL){
                printf("huffman: output file could not be created\n");
                exit(-1);
            }
			outputFN = "out.huf";
        }else{
            if((output = fopen("out.txt", "w")) == NULL){
                printf("huffman: output file could not be created\n");
                exit(-1);
            }
			outputFN = "out.txt";
        }
    
    // If an output file was given, we make sure it has the correct filename extention
    }else{
		char *outputExt = getFileExt(outputFN);
		char *normalExt;

		normalExt = (dflag == 0 ? "huf" : "txt");

		if(strcmp(outputExt, normalExt) != 0){
			printf("huffman: please use the correct filename extentions for the output file (.%s)\n", normalExt);
			exit(-1);
		}
	}

	// Once the filenames have been checked, we can open the files
	if((input = fopen(inputFN, "r")) == NULL){
		printf("huffman: input file does not exist\n");
		exit(-1);
	}
	if((output = fopen(outputFN, "w")) == NULL){
		printf("huffman: output file could not be created\n");
		exit(-1);
	}

    // We return the decode flag so the main can know what function to call
    return dflag;
}
