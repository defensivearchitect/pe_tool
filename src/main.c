/* main.c -- command-line entry point for pe_tool. */
#include <stdio.h>

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

    fclose(fp);
    return 0;
}