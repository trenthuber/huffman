
# File Compression Command Line Utility

A command line utility using the [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) algorithm for compressing and decompressing files.

## Installation (UNIX and Unix-like operating systems only)

```
git clone https://github.com/trenthuber/huffman.git
cd huffman
```
The `make` command will make the binary in a newly created `./bin/` directory. the `make clean` command will remove the binary and the object files created in `./src/`.

The command `sudo make install clean` will install the binary to `/usr/local/bin`. To uninstall the command, use `sudo make uninstall`.

## Features

 - Unlimited filesize (as big as your operating system can handle)
 - Binary files are supported (although by the nature of Huffman coding, won't always compress that well)
 - All memory is allocated on the stack for efficiency and simplicity

## Other notes

 - You're on your own for getting this to work for Windows
 - I'd one day like to implement an archiver/unarchiver to go alongside this utility so multiple files can be compressed/decompressed simultaneously

## License

See [LICENSE](./LICENSE) file for license rights and limitations.
