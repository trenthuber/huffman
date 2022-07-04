.PHONY: clean uninstall

CC = cc
CFLAGS = -O3 -Wall -Wextra

SRC = $(wildcard src/*.c) $(wildcard src/global/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin
LOCAL = /usr/local/bin

all: bin huffman

ifeq ($(OS), Windows_NT)

# Used to supress the main
CFLAGS := $(CFLAGS) -Wno-unused-parameter

OBJWIN = $(subst /,\,$(OBJ))

bin:
	mkdir $(BIN)

huffman: $(OBJ)
	$(CC) -o $(BIN)\huffman.exe $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rmdir /s /q $(BIN)
	del /s /q $(OBJWIN)

install: all
	mkdir "$(LOCAL)\Huffman"
	copy .\$(BIN)\huffman.exe "$(LOCAL)\Huffman\huffman.exe"
	setx PATH %PATH%;"$(LOCAL)\Huffman"

uninstall:
	rmdir /s /q "$(LOCAL)\Huffman"
	setx PATH %PATH%

else
LDFLAGS = -lm

bin:
	mkdir -p ./$(BIN)

huffman: $(OBJ)
	$(CC) -o $(BIN)/huffman $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)

install: all
	cp $(BIN)/huffman $(LOCAL)/huffman
	chmod 755 $(LOCAL)/huffman

uninstall:
	rm -f $(LOCAL)/huffman

endif
