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

    printf("opened: %s\n", path);

    fclose(fp);
    return 0;
}