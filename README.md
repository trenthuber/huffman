# File Compression Command Line Utility

A command line utility using the Huffman coding algorithm for compressing and decompressing text files.

## Important Notes

**This program is designed to compress a single text file with standard ASCII characters and UTF-8 encoding.** The file being compressed must have
a `.txt` file extention and the output file must have a `.huf` file extension. The reverse is true for decompression.

On a 64-bit system when compressing, the **file size is limited to 2.1GB** (2GiB) which is more than enough for an ASCII text file. For a 32-bit system,
however, the max file size when compressing is 32.8KB (32KiB) which is barely worth compressing. So, for all intents and purposes, **32-bit 
systems are not supported**.

## Installation Steps

### For Mac/Linux:
```
git clone https://github.com/trenthuber/huffman.git
cd huffman
```
The `make` command will make the binary in a newly created `bin` directory.

The command `sudo make install clean` will install the binary to `/usr/local/bin`. To uninstall the command, use `sudo make clean uninstall`.

### For Windows:
WSL?

## Using the Command

###### Note: In order to use this command in any directory, you must `sudo make install` it first.

To compress a file, simply give it a file to compress and an output file to send it too (a lot like `gcc`):
```
huffman input.txt -o compressed.huf
```

To decompress a file the same applies but you must include the `-d` option:
```
huffman -d input.huf -o decompressed.txt
```

## Code Organization

All source code is in the `src` directory. Within `src` is a `global` directory containing source code used by the majority of the other files (just to 
better distinguish them). `options.c` is only used by `main.c` to handle command line arguments. The compression and decompression source code is split 
into seperate files (`encode.c` and `decode.c` respectively), but both files use `global` and `tree.c`.`list.c` and `table.c` are used exclusively by 
`encode.c`. `node.c` is used exclusively by `tree.c`.

## How the Program Works

### Compressing (Encoding)
The compression algorithm is significantly more complicated than the decompression algorithm. There are four steps:
1. Create a sorted list of characters used in the file in order of frequency (`list.c`)
2. Make a [Huffman tree](https://en.wikipedia.org/wiki/Huffman_coding) of the characters in the list (`tree.c`)
3. From the tree, create a table of values that contains the Huffman codes for each character (`table.c`)
4. Write to a new file replacing the characters of the input file with their respective Huffman codes (`encode.c`)

Creating the list (and, thus, the list of nodes) is fairly straight-forward. I implemented that Huffman algorithm by having a list of pointers to nodes and
then changing the pointers without changing the order of the nodes themselves. So, every time I take away two nodes from the bottom, I'm really just having
a new node point to those two nodes, and then having those pointers shift that used to shift to the nodes now point nowhere. Then I insert the new node by
shifting all the pointers below that node up by one instead of actually swapping elements in the list of nodes. 

Creating the table of values was the most challenging part, I had to convert a tree structure into a table structure. Although I could have simply had an
extra pointer to a "parent" `struct node` in the definition of `struct node`, I took a more tedious route of having a list of "`leftPointers`" as I called 
them that were at the last places we had gone left when traversing the tree. This is how I "remembered" my path when traversing a tree. I then had a recursive
function create a table of Huffman codes based on the ranges between these `leftPointers`.

Encoding the file now that I had a list of codes was the conceptually easier part, but the more technically challenging part. In C, you can't write single 
bits to a file (in fact, I don't know of a single programming language that allows you to do that, probably some archaic ones). In fact, the smallest unit 
of data you can write to a fileare bytes (with the fputc() function). My solution to this was to create an `unsigned char` variable to act as a "buffer". 
In `fileio.c`, I made functions that would write these bits to the buffer instead and then (when the buffer was full) write the buffer to the file. I 
utilized bit shifting and bit masking to accomplish this goal, and the end result worked surprisingly well with few bugs.

The final thing I had to figure out was how to format the compressed file. By this I mean, it's not good enough to just have the converted bits, you need to
also encode the tree itself so the decompressor can rebuild the `.txt` file. I chose to go with a method outlined in the (aforementioned)
[Wikipedia article](https://en.wikipedia.org/wiki/Huffman_coding#Decompression) which describes traversing the tree and writing "0"s for internal nodes and
"1"s for leaf nodes (along with the character that the leaf node represents directly after the "1"). Once the tree is constructed (and, as it turns out, the
tree will always know when it's done being constructed since it's a binary tree), the decompression algorithm knows that the rest of the bits are the actual
encoded text. With `fileio.c` abstracting away all the technical file writing, this part was fairly easy to implement.

### Decompressing (Decoding)

The easier of the two, decompressing the data simply consists of two steps:
1. Reconstructing the tree (`tree.c`)
2. Decoding the rest of the message (`decode.c`)

To reconstruct the tree, I used a "mirror" to the method I used to encode the tree in `encode.c` in which I simply read in the bits (which also required writing
functions to **read** bits, as well as write them in `fileio.c`) and construct the tree as I see the bits come in. Once the tree is built and the function is
returned, I simply traverse the tree as the rest of the file tells me to ("0" for left, "1" for right). Every time I hit a leaf node, I write it to the file and
set the node pointer to the root node again.

The only part that took some thought was the very last byte. It's not likely that the encoded bits will be a multiple of 8, so you're going to have to add some
padding to the end of the last byte written. So, how do I tell the decoder what's padding and what's not? I decided to use a bit change to indicate that. After
the last bit is written, I write a "1" and then the rest of the byte is "0"s padding. When decoding, the decoder reads the last byte from the end until it
encounters a "1". Then it knows the bit before that one is the last one. The only edge case is when we actually don't need padding at all: then we have to write
an entire byte to the end of the file. Although it's unfortunate, I couldn't think of any other methods that do better.
