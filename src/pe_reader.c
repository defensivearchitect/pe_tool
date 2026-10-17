/* pe_reader.c -- read an input file into memory. */
#include "pe_reader.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t *pe_read_file(const char *path, size_t *out_len) {
    /* Binary mode: on Windows, text mode turns CR LF into LF and
       treats byte 0x1A as end of file, which corrupts binary data. */
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror(path);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        perror(path);
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);
    if (size < 0) {
        perror(path);
        fclose(fp);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        perror(path);
        fclose(fp);
        return NULL;
    }

    printf("size: %ld bytes\n", size);

    if (size == 0) {
        fprintf(stderr, "%s: file is empty\n", path);
        fclose(fp);
        return NULL;
    }

    size_t len = (size_t)size;
    uint8_t *data = malloc(len);
    if (data == NULL) {
        fprintf(stderr, "%s: out of memory\n", path);
        fclose(fp);
        return NULL;
    }

    size_t got = fread(data, 1, len, fp);
    if (got != len) {
        fprintf(stderr, "%s: could not read the whole file\n", path);
        free(data);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    *out_len = len;
    return data;
}
