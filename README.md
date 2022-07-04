# Text Compression Command Line Utility

A command line utility using the Huffman coding algorithm for compressing and decompressing text files.

## Installation Steps

### For Mac/Linux:
```
git clone https://github.com/trenthuber/huffman.git
cd huffman
```
The `make` command will make the binary in a newly created `bin` directory.

The command `sudo make install clean` will install the binary to `/usr/local/bin`. To uninstall the command, use `sudo make clean uninstall`.

### For Windows:
Check out the `windows` branch of this project for code that has been tweaked to run on Windows. There is also a `huffman.exe` executable file under the release for this project that should be plug and play on Windows.

You could also try cloning the main branch into WSL.

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

All source code is in the `src/` directory. Within `src/` is a `global/` directory containing source code used by the majority of the other files (just to better distinguish them). `options.c` is only used by `main.c` to handle command line arguments. The compression and decompression source code is split into seperate files (`encode.c` and `decode.c` respectively), but both files use `global/` and `tree.c`.`table.c` is used exclusively by `encode.c`. `node.c` and `heap.c` are used exclusively by `tree.c`.

## Scope of the Program

This program is intended to compress `.txt` files using [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding). The **file size is capped at 2.1GB** (2GiB) for a 64-bit system, but I think it's reasonable to assume you'll never have a single text file that exceeds that memory constraint. For a 32-bit system, this max file size is actually 32.8KB (32KiB) which is barely worth compressing. For this reason, **32-bit systems are not officially supported**.

Although this was intended to be used exclusively with text files, you can *technically* compress **any** file type since the program treats all files as binary files. And yes, this also means you can recursively compress files, feeding the output of one compression as the input of another. However, in both practice (and theory) this won't do much. Huffman coding works best with files that have a wide variety of frequencies in their data set, files that have a very clear most used character, second most used character, etc. In a sense, Huffman coding assigns the shortest sequence of bits to the characters that are used the most. The issue with the average binary file is that it's bytes are relatively random. This means two things:
1. It will probably use almost all possible bytes
2. It will probably have an even distribution of those bytes

If most of the bytes happen with the same frequency, all the bytes will have (more or less) the same code length. And if all bytes happen with similar frequency, then all these bytes end up getting a code length of pretty much the same length they were before. For this reason, while it's possible to compress binary files, it isn't at all practical. The largest binary file I tested was compressed by 1/100th of a percent.

## How the Program Works (for those that are interested)

### Compressing (Encoding)
The compression algorithm is significantly more complicated than the decompression algorithm. There are four steps:
1. Create a [heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) of nodes based on their frequencies for every character used in the file (`heap.c`)
2. Make a Huffman tree from the nodes in the heap (`tree.c`)
3. From that tree, create a table of values that contains the Huffman codes for each character (`table.c`)
4. Write to a new file replacing the characters of the input file with their respective Huffman codes (`encode.c`)

To create the heap, I started by going through the file and counting how many times each character appeared (by character, I mean *any* byte). This list is then converted into a list of nodes (using `node.c`) and then added to the heap with the element with the smallest weight at the top (weight being frequency in the input file).

From this, I took the top two elements of the heap (the two smallest elements in the current list) and created a new node with those two nodes as left and right children. I also summed their weights which became the new weight of the new node. I then inserted the new node back into the heap and repeated this until there was only one node left. This final node in the heap is the completed Huffman tree.

Taking this tree and turning it into a table of values was actually easy with recursion. I started by creating an array of strings that would contain a code for each character. One of the issues I was having at first was that I couldn't store the code for the null character in a string since strings always end with the null terminating character. So instead of including the character in the string of 1's and 0's, I placed the code at that character's corresponding number position in the array (so the code corresponding to `'a'` goes in `codes[97]`). 

After creating the array, I used recursion to take the code for the internal node above the current node and turn that into the current node's code. Once we get down to a leaf node, we copy that final code over to the corresponding position in the string array for the codes.

While encoding the file (now that I had a list of codes) was the conceptually easier part, I found it to be the more technically challenging part. In C, you can't write single bits to a file (in fact, I don't know of a single programming language that allows you to do that, probably some archaic ones). In fact, the smallest unit of data you can write to a file is bytes (with the fputc() function, or something of the sort). My solution to this was to create an `unsigned char` variable to act as a "buffer". In `fileio.c`, I made functions that would write these bits to the buffer instead and then (when the buffer was full) write the buffer to the file. I utilized bit shifting and bit masking to accomplish this goal, and the end result worked surprisingly well with few bugs.

The final thing I had to figure out was how to format the compressed file. By this I mean, it's not good enough to just have the converted bits, you need to also encode the tree itself so the decompressor can rebuild the `.txt` file. I chose to go with a method outlined in the (aforementioned) [Wikipedia article](https://en.wikipedia.org/wiki/Huffman_coding#Decompression) which describes traversing the tree and writing "0"s for internal nodes and "1"s for leaf nodes (along with the character that the leaf node represents directly after the "1"). Once the tree is constructed (and, as it turns out, the tree will always know when it's done being constructed since it's a binary tree), the decompression algorithm knows that the rest of the bits are the actual encoded text. With `fileio.c` abstracting away all the technical file writing, this part was fairly easy to implement.

### Decompressing (Decoding)

The easier of the two, decompressing the data simply consists of two steps:
1. Reconstructing the tree (`tree.c`)
2. Decoding the rest of the message (`decode.c`)

To reconstruct the tree, I mirrored to the method I used to encode the tree in `encode.c` in which I simply read in the bits (which also required writing functions to **read** bits as well as write them in `fileio.c`) and reconstruct the tree as I see the bits come in. Once the tree is built and the function is returned, I simply traverse the tree as the rest of the file tells me to ("0" for left, "1" for right). Every time I hit a leaf node, I write it to the file and set the node pointer to the root node again.

The only part that took some thought was the very last byte. It's not likely that the encoded bits will be a multiple of 8, so you're going to have to add some padding to the end of the last byte written. So, how do I tell the decoder what's padding and what's not? I decided to use a bit change to indicate that. After the last bit is written, I write a "1" and then the rest of the byte is "0"s padding. When decoding, the decoder reads the last byte from the end until it encounters a "1". Then it knows the bit before that one is the last one. The only edge case is when we actually don't need padding at all: then we have to write an entire byte to the end of the file. Although slightly unfortunate, I couldn't think of any better methods.
