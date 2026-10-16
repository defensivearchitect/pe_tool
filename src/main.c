/* main.c -- command-line entry point for pe_tool. */
#include <stdio.h>

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    printf("pe_tool: %s\n", argv[1]);
    return 0;
}