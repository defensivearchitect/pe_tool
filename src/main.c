/* main.c -- command-line entry point for pe_tool. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Read the whole file at path into memory. On success, return the
   buffer and store its length in *out_len; the caller must free the
   buffer. On failure, print the reason and return NULL. */
static uint8_t *read_file(const char *path, size_t *out_len) {
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

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    size_t len = 0;
    uint8_t *data = read_file(path, &len);
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
