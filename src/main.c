/* main.c -- command-line entry point for pe_tool. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pe_reader.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    size_t len = 0;
    uint8_t *data = pe_read_file(path, &len);
    if (data == NULL) {
        return 1;
    }

    printf("read: %zu bytes\n", len);

    if (len < 2) {
        fprintf(stderr, "%s: file too small\n", path);
        free(data);
        return 1;
    }

    printf("first bytes: %02x %02x\n", data[0], data[1]);

    free(data);
    return 0;
}
