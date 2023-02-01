#include "main.h"

// --- BIT READ FUNCTIONS --- //
static uint8_t refill_buffer(void)
{
	if(buffer_size){
		return 0;
	}
	static uint8_t eof_flag;
	if(eof_flag){
		return 1;
	}
	buffer = next_buffer;
	if(fread(&next_buffer, 1, 1, input_file)){
		buffer_size = 8;
		return 0;
	}
	if(buffer == high_mask){
		return 1;
	}

	eof_flag = 1;
	next_buffer = buffer;
	uint8_t ending = 0, padding_size = 0;
	while((ending = next_buffer & low_mask) == 0){
		++padding_size;
		next_buffer >>= 1;
	}
	buffer_size = 8 - (padding_size + 1);

	return 0;
}

static uint8_t read_bit(void)
{
	if(refill_buffer()){
		return 2;
	}

	uint8_t bit = (high_mask & buffer ? 1 : 0);
	buffer <<= 1;
	--buffer_size;
	return bit;
}

static uint8_t read_char(void)
{
	uint8_t chr = 0;
	for(uint8_t i = 0, bit = 0; i < 8; ++i){
		if((bit = read_bit()) == 2){
			file_error_message("cannot decompress, incomplete file header");
		}
		chr <<= 1;
		chr |= bit;
	}
	return chr;
}

static void read_tree(Node *branch)
{
	if(read_bit()){
		uint16_t left_index = append_new_node(read_char(), 0, NULL, NULL);
		branch->left = &heap[left_index];
	}else{
		branch->left = &heap[heap_size++];
		read_tree(branch->left);
	}
	if(read_bit()){
		uint16_t right_index = append_new_node(read_char(), 0, NULL, NULL);
		branch->right = &heap[right_index];
	}else{
		branch->right = &heap[heap_size++];
		read_tree(branch->right);
	}
}

// --- DECOMPRESS --- //
void decompress(void)
{
	uint8_t i = 0;
	while(magic_numbers[i]){
		if(!fread(&buffer, 1, 1, input_file) || buffer != magic_numbers[i++]){
			file_error_message("cannot decompress, magic numbers do not match");
		}
	}
	if(!fread(&buffer, 1, 1, input_file) || !fread(&next_buffer, 1, 1, input_file)){
		file_error_message("cannot decompress, file too small");
	}
	buffer_size = 8;
	if(read_bit()){
		file_error_message("cannot decompress, nonsense file header");
	}

	++heap_size;
	read_tree(heap);

	uint8_t bit;
	Node *current_node = heap;
	while((bit = read_bit()) != 2){
		current_node = (bit ? current_node->right : current_node->left);
		if(current_node->left == NULL && current_node->right == NULL){
			if(!fwrite(&current_node->character, 1, 1, output_file)){
				file_error_message("unable to complete a write to the output file");
			}
			current_node = heap;
		}
	}

	if(current_node != heap){
		file_error_message("cannot decompress, nonsense file body");
	}
}
