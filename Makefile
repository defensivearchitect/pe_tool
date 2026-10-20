CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wmissing-prototypes -g -Iinclude
SRC    = $(wildcard src/*.c)
HDR    = $(wildcard include/*.h)
OUT    = build/pe_tool.exe

TEST_HDR = $(wildcard tests/*.h)
TESTS    = build/test_bytes.exe build/test_headers.exe

all: $(OUT)

$(OUT): $(SRC) $(HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: $(TESTS)
	./build/test_bytes.exe
	./build/test_headers.exe

build/test_bytes.exe: tests/test_bytes.c src/pe_bytes.c $(HDR) $(TEST_HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(filter %.c,$^) -o $@

build/test_headers.exe: tests/test_headers.c src/pe_headers.c src/pe_bytes.c $(HDR) $(TEST_HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(filter %.c,$^) -o $@

clean:
	rm -rf build

.PHONY: all clean test
