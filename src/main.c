/* main.c -- command-line entry point for pe_tool. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror(path);
        return 1;
    }

    if (fseek(fp, 0, SEEK_END) !=0) {
        perror(path);
        fclose(fp);
        return 1;
    }

    long size = ftell(fp);
    if (size < 0) {
        perror(path);
        fclose(fp);
        return 1;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        perror(path);
        fclose(fp);
        return 1;
    }

    printf("size: %ld bytes\n", size);

    if (size == 0) {
        fprintf(stderr, "%s: file is empty\n", path);
        fclose(fp);
        return 1;
    }

    size_t len = (size_t)size;
    uint8_t *data = malloc(len);
    if (data == NULL) {
        fprintf(stderr, "%s: out of memory\n", path);
        fclose(fp);
        return 1;
    }

    size_t got = fread(data, 1, len, fp);
    if (got != len) {
        fprintf(stderr, "%s: could not read the whole file\n", path);
        free(data);
        fclose(fp);
        return 1;
    }

    printf("read: %zu bytes\n", got);

    free(data);

    fclose(fp);
    return 0;
}
