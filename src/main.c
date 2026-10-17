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

    uint8_t *data = NULL;
    size_t len = 0;
    pe_read_status status = pe_read_file(path, &data, &len);
    if (status == PE_READ_OPEN_FAILED) {
        perror(path);
        return 1;
    }
    if (status != PE_READ_OK) {
        fprintf(stderr, "%s: %s\n", path, pe_read_status_str(status));
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
