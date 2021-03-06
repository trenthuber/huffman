#include <stdio.h>
#include <stdlib.h>
#if __MACH__ || __linux__
#include <unistd.h>
#endif
#include <string.h>

#include "global/global.h"

#include "options.h"

void printHelpMessage(void){
    printf(
        "\n"
        "huffman - File compressor using Huffman Coding, version 2.0.1\n\n"
        "Usage: huffman [-dh] file [-o pathname]\n\n"
        "Options:\n"
        "d : Decompress the given file\n"
        "h : Show help message\n"
        "o [pathname] : Output file (default is \"out.huf\" / \"out.txt\")\n\n"
    );
}

int handleOptions(int argc, char **argv){

    int dflag = 0;

    // The OS is Windows
    #ifdef _WIN64

    // These parameters aren't used in the Windows options code
    (void) argc;
    (void) argv;
    
    char inputFN[260], outputFN[260];

    printf("Huffman Coding Text Compression Utility\n\n");

    char mode = 'a';
    char overflow[260];
    printf("Operation modes:\n\t(c) compress a file\n\t(d) decompress a file\n\n");
    do{
        printf("Select a mode: ");
        mode = fgetc(stdin);

        // Gets rid of the rest of whatever they would have typed in
        if(mode != (char) 10){
            fgets(overflow, 260, stdin);
        }
    }while(mode != 'c' && mode != 'd');
    dflag = (mode == 'c' ? 0 : 1);

    // Input file

    // Checks that file exists
    char *prefix = (dflag == 1 ? "de" : "");
    printf("\nFile to %scompress: ", prefix);
    fgets(inputFN, 260, stdin);
    inputFN[strlen(inputFN) - 1] = '\0';
    while(fopen_s(&input, inputFN, "rb") != 0){
        if(strcmp(inputFN, "") != 0){
            printf("Couldn't open file, try again\n");
        }
        printf("File to %scompress: ", prefix);
        fgets(inputFN, 260, stdin);
        inputFN[strlen(inputFN) - 1] = '\0';
    }

    // Checks that the file isn't too big
    fseek(input, 0, SEEK_END);
    while(ftell(input) > 2147483647){ // 2147483647 = (2^31) - 1 = longest number for a signed int
        printf("File too large to encode, try again\n");
        rewind(input);
        printf("File to %scompress: ", prefix);
        fgets(inputFN, 260, stdin);
        inputFN[strlen(inputFN) - 1] = '\0';
        fseek(input, 0, SEEK_END);
    }
    fileSize = (int) ftell(input);
    rewind(input);

    // Output file
    printf("\nFile to output to (press enter for default): ");
    fgets(outputFN, 260, stdin);
    outputFN[strlen(outputFN) - 1] = '\0';
    if(strcmp(outputFN, "") == 0){
        strcpy_s(outputFN, 260, (dflag == 0 ? "out.huf" : "out.txt"));
    }

    // User can't compress/decompress a file onto itself
    while(strcmp(inputFN, outputFN) == 0){
        printf("Can not %scompress a file onto itself\n", prefix);
        printf("File to output to (press enter for default): ");
        fgets(outputFN, 260, stdin);
        outputFN[strlen(outputFN) - 1] = '\0';
        if(strcmp(outputFN, "") == 0){
            strcpy_s(outputFN, 260, (dflag == 0 ? "out.huf" : "out.txt"));
        }
    }

    // Checks if the output file can be opened
    while(fopen_s(&output, outputFN, "wb") != 0){
        printf("Could not open output file, try again\n");
        printf("File to output to (press enter for default): ");
        fgets(outputFN, 260, stdin);
        outputFN[strlen(outputFN) - 1] = '\0';
        if(strcmp(outputFN, "") == 0){
            strcpy_s(outputFN, 260, (dflag == 0 ? "out.huf" : "out.txt"));
        }
    }

    // The OS is Mac or Linux
    #elif __MACH__ || __linux__

    // Flags for input and output (default is false)
    int iflag, oflag;
    iflag = oflag = 0;
    int opt = 0;

    // Strings of the filenames (for filename error detection)
    char *inputFN, *outputFN;
	inputFN = outputFN = "";
    
    // Display help message if no arguments are provided
    if(argc == 1){
        printHelpMessage();
        exit(0);
    }
    
    /* As far as I can understand, the getopt() function returns -1 both in the case 
     * of getting an option not in the optstring and when it finishes the options. 
     * For this reason, I'm manually keeping track of the index of the argv array with 
     * the argvIndex variable
     */
    int argvIndex = 1;

    while(argvIndex < argc){
        opt = getopt(argc, argv, ":hdo:");
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
            case -1:
                if(iflag == 0){
                    iflag = 1;
                    inputFN = argv[optind];
                }else{
                    fprintf(stderr, "huffman: can only take input from one file\n");
                    exit(-1);
                }
                optind++;
            break;
            default:
                fprintf(stderr, "huffman: non-valid option\n");
                exit(-1);
        }
        argvIndex++;
    }

    // Checks if there are (still) no input files provided
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

    // The OS is unsupported
    #else
    printf("huffman: supported operating system not detected (options.c, 0)\n");
    exit(-1);
    #endif

    // We return the decode flag so main() can know what function to call
    return dflag;
}
