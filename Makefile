CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wmissing-prototypes -g -Iinclude
SRC    = $(wildcard src/*.c)
HDR    = $(wildcard include/*.h)
OUT    = build/pe_tool.exe

TEST_SRC = tests/test_bytes.c src/pe_bytes.c
TEST_OUT = build/test_bytes.exe

all: $(OUT)

$(OUT): $(SRC) $(HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: $(TEST_OUT)
	./$(TEST_OUT)

$(TEST_OUT): $(TEST_SRC) $(HDR)
	mkdir -p build
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_OUT)

clean:
	rm -rf build

.PHONY: all clean test
