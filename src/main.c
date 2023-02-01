#include "main.h"

// --- FILE I/O --- //
FILE *input_file, *output_file;
char *output_file_name = "";
const uint8_t magic_numbers[] = "\xDE\xAD\xBE\xEF";

void file_error_message(const char *message){
	fprintf(stderr, "huffman: %s\n", message);
	if(input_file){
		fclose(input_file);
	}
	if(output_file){
		fclose(output_file);
	}
	remove(output_file_name);
	exit(1);
}

// --- DATA STRUCTURES --- //
uint16_t heap_size;
Node heap[2 * NUM_ASCII];

uint16_t append_new_node(uint8_t character, uint64_t freq, Node *left, Node *right)
{
	heap[heap_size].character = character;
	heap[heap_size].freq = freq;
	heap[heap_size].left = left;
	heap[heap_size].right = right;
	return heap_size++;
}

// --- BIT READ/WRITE --- //
const uint8_t high_mask = 0x80, low_mask = 0x01;
uint8_t buffer, next_buffer, buffer_size;

// --- MAIN --- //
void compress(void);
void decompress(void);

static void usage(void)
{
	fprintf(stdout,
		"usage: huffman [-dh] source_file [-o target_file]\n"
		"   The default action taken is to compress `source_file' and\n"
		"   output `source_file.huf'. For decompression, the default\n"
		"   output is `source_file.orig'.\n"
		"   -d\tdecompress the input file\n"
		"   -h\tshow this help message\n"
		"   -o\tset target file\n"
	);
	return;
}

int main(int argc, char **argv)
{
	if(argc == 1){
		usage();
		exit(0);
	}

	char *input_file_name = "";
	uint8_t decompress_flag = 0, input_flag = 0, output_flag = 0;
	for(--argc, ++argv; argc; --argc, ++argv){
		if((*argv)[0] == '-'){
			if((*argv)[2] != '\0'){
				file_error_message("illegal option, use -h for help");
			}
			switch((*argv)[1]){
				case 'd':
					decompress_flag = 1;
					break;
				case 'h':
					usage();
					exit(0);
				case 'o':
					if(output_flag){
						file_error_message("can only output to one file");
					}
					--argc;
					++argv;
					if(argc == 0 || (*argv)[0] == '-'){
						file_error_message("output file path not provided, use -h for help");
					}
					output_flag = 1;
					output_file_name = *argv;
					break;
				default:
					file_error_message("illegal option, use -h for help");
			}
		}else{
			if(input_flag){
				file_error_message("can only take input from one file");
			}
			input_flag = 1;
			input_file_name = *argv;
		}
	}

	if(!input_flag){
		file_error_message("input file path not provided, use -h for help");
	}
	if(strcmp(input_file_name, output_file_name) == 0){
		file_error_message("cannot use the same file path for input and output");
	}
	if((input_file = fopen(input_file_name, "rb")) == NULL){
		file_error_message("could not open input file");
	}
	if(!output_flag){
		output_file_name = strcat(input_file_name, (decompress_flag ? ".orig" : ".huf"));
	}
	if((output_file = fopen(output_file_name, "wb")) == NULL){
		file_error_message("could not open output file");
	}

	decompress_flag ? decompress() : compress();

	fclose(input_file);
	fclose(output_file);
	return 0;
}
