CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -Iinclude
SRC    = $(wildcard src/*.c)
HDR    = $(wildcard include/*.h)
OUT    = build/pe_tool.exe

all: $(OUT)

$(OUT): $(SRC) $(HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build

.PHONY: all clean
