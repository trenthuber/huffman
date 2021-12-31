#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "global.h"

int iflag, oflag, dflag; // Automatically set to 0 since they're global

void printHelpMessage(void){
    printf(
        "huffman, version 1.0\n\n"
        "usage: huffman [-hd] file [-o pathname]\n\n"
        "h : show help message\n"
        "d : decode the given file\n"
        "o [pathname] : output file (default is \"out.huf\" / \"out.txt\")\n"
    );
}

int handleOptions(int argc, char *argv[]){
    int opt;
    while(optind < argc){
        if((opt = getopt(argc, argv, "hdo:")) != -1){
            switch(opt){
                case 'h':
                    printHelpMessage();
                    exit(0);
                case 'o':
                    if(oflag == 0){
                        if((output = fopen(optarg, "w")) == NULL){
                            printf("huffman: output file could not be created\n");
                            exit(-1);
                        }
                        oflag = 1;
                    }else{
                        printf("huffman: can only output to one file at a time\n");
                        exit(-1);
                    }
                    break;
                case 'd':
                    dflag = 1;
                    break;
            }
        }else{
            if(iflag == 0){
                if((input = fopen(argv[optind], "r")) == NULL){
                    printf("huffman: input file does not exist\n");
                    exit(-1);
                }
                iflag = 1;
                optind++;
            }else{
                printf("huffman: can only take input from one file at a time\n");
                exit(-1);
            }
        }
    }

    if(iflag == 0){
        printf("huffman: No input file provided\n");
        exit(-1);
    }
    if(oflag == 0){
        if(dflag == 0){
            if((output = fopen("out.txt", "w")) == NULL){
                printf("huffman: output file could not be created\n");
                exit(-1);
            }
        }else{
            if((output = fopen("out.huf", "w")) == NULL){
                printf("huffman: output file could not be created\n");
                exit(-1);
            }
        }
    }

    return dflag;
}