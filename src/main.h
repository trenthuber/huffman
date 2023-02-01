#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- FILE I/O --- //
extern FILE *input_file, *output_file;
extern char *output_file_name;
extern const uint8_t magic_numbers[];

void file_error_message(const char *message);

// --- DATA STRUCTURES --- //
#define NUM_ASCII 256

typedef struct Node Node;
struct Node {
	Node *left;
	Node *right;
	uint64_t freq;
	uint8_t character;
};

extern uint16_t heap_size;
extern Node heap[];

uint16_t append_new_node(uint8_t character, uint64_t freq, Node *left, Node *right);

// --- BIT READ/WRITE --- //
extern const uint8_t high_mask, low_mask;
extern uint8_t buffer, next_buffer, buffer_size;
