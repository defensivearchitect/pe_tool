/* main.c -- command-line entry point for pe_tool. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pe_headers.h"
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

    size_t pe_offset = 0;
    pe_header_status header_status = pe_find_pe_header(data, len, &pe_offset);
    if (header_status != PE_HEADER_OK) {
        fprintf(stderr, "%s: %s\n", path, pe_header_status_str(header_status));
        free(data);
        return 1;
    }

    printf("PE header at: 0x%zx\n", pe_offset);

    free(data);
    return 0;
}
