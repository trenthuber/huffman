#include "main.h"

// --- HEAP FUNCTIONS --- //
static void swap(uint8_t parent_index, uint8_t child_index)
{
	Node temp = heap[parent_index];
	heap[parent_index] = heap[child_index];
	heap[child_index] = temp;
}

static void insert(uint8_t character, uint64_t freq, Node *left, Node *right)
{
	uint8_t child_index = append_new_node(character, freq, left, right), parent_index = child_index >> 1;
	while(child_index > 0 && heap[parent_index].freq > heap[child_index].freq){
		swap(parent_index, child_index);
		child_index = parent_index;
		parent_index >>= 1;
	}
}

static void downheap(void)
{
	uint16_t parent_index = 0, left_index = 1, right_index = 2;
	uint64_t parent_freq = heap[0].freq, left_freq = heap[1].freq, right_freq = heap[2].freq;
	while(right_index < heap_size && (parent_freq > left_freq || parent_freq > right_freq)){
		if(left_freq < right_freq){
			swap(parent_index, left_index);
			parent_index = left_index;
		}else{
			swap(parent_index, right_index);
			parent_index = right_index;
		}
		left_index = (parent_index << 1) + 1;
		right_index = (parent_index << 1) + 2;

		parent_freq = heap[parent_index].freq;
		left_freq = heap[left_index].freq;
		right_freq = heap[right_index].freq;
	}
	
	if(right_index == heap_size && parent_freq > left_freq){
		swap(parent_index, left_index);
	}
}

// --- BIT WRITE FUNCTIONS --- //
static void write_bit(uint8_t bit)
{
	buffer <<= 1;
	buffer |= bit;

	if(++buffer_size == 8){
		if(!fwrite(&buffer, 1, 1, output_file)){
			file_error_message("unable to complete a write to the output file");
		}
		buffer_size = 0;
	}
}

static void write_char(uint8_t chr)
{
	for(uint8_t i = 0; i < 8; ++i){
		write_bit(chr & high_mask ? 1 : 0);
		chr <<= 1;
	}
}

static void write_tree(Node *branch)
{
	if(branch->left == NULL && branch->right == NULL){
		write_bit(1);
		write_char(branch->character);
	}else{
		write_bit(0);
		write_tree(branch->left);
		write_tree(branch->right);
	}
}

// --- CODE GENERATION --- //
#define MAX_CODE_LENGTH 256

static char prefix[MAX_CODE_LENGTH];
static char codes[NUM_ASCII][MAX_CODE_LENGTH];

static void generate_codes(Node *branch)
{
	static uint8_t index;
	if(branch->left == NULL){
		prefix[index] = '\0';
		strcpy(codes[branch->character], prefix);
		return;
	}

	prefix[index++] = '0';
	generate_codes(branch->left);
	--index;

	prefix[index++] = '1';
	generate_codes(branch->right);
	--index;
}

// --- COMPRESS --- //
void compress(void)
{
	uint8_t chr;
	uint64_t freqs[NUM_ASCII] = {0};
	while(fread(&chr, 1, 1, input_file)){
		++freqs[chr];
	}
	rewind(input_file);

	chr = 0;
	do{
		if(freqs[chr] != 0){
			insert(chr, freqs[chr], NULL, NULL);
		}
	}while(chr++ < NUM_ASCII - 1);

	if(heap_size <= 1){
		file_error_message("cannot compress, file too small");
	}

	uint16_t stack_index = (NUM_ASCII << 1) - 1;
	while(heap_size > 1){

		heap[stack_index--] = heap[0];
		heap[0] = heap[--heap_size];
		downheap();

		heap[stack_index--] = heap[0];
		heap[0].character = 0;
		heap[0].freq = heap[stack_index + 2].freq + heap[stack_index + 1].freq;
		heap[0].left = &heap[stack_index + 2];
		heap[0].right = &heap[stack_index + 1];
		downheap();
	}

	if(!fwrite(magic_numbers, strlen((char *) magic_numbers), 1, output_file)){
		file_error_message("unable to complete a write to the output file");
	}

	write_tree(heap);
	generate_codes(heap);

	while(fread(&chr, 1, 1, input_file)){
		for(uint8_t i = 0; codes[chr][i] != '\0'; ++i){
			write_bit(codes[chr][i] == '0' ? 0 : 1);
		}
	}

	write_bit(1);
	while(buffer_size > 0){
		write_bit(0);
	}
}
