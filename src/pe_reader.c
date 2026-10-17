/* pe_reader.c -- read an input file into memory. */
#include "pe_reader.h"

#include <stdio.h>
#include <stdlib.h>

/* Read the whole content of an already open file. The caller opens
   and closes the file, so this function can return eraly on any
   error without leaving the file open. */
static pe_read_status read_open_file(FILE *fp, uint8_t **out_data, size_t *out_len) {
    if (fseek(fp, 0, SEEK_END) != 0) {
        return PE_READ_SEEK_FAILED;
    }

    long size = ftell(fp);
    if (size < 0) {
        return PE_READ_SEEK_FAILED;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        return PE_READ_SEEK_FAILED;
    }

    if (size == 0) {
        return PE_READ_EMPTY;
    }

    size_t len = (size_t)size;
    uint8_t *data = malloc(len);
    if (data == NULL) {
        return PE_READ_NO_MEMORY;
    }

    size_t got = fread(data, 1, len, fp);
    if (got != len) {
        free(data);
        return PE_READ_SHORT_READ;
    }

    *out_data = data;
    *out_len = len;
    return PE_READ_OK;
}

pe_read_status pe_read_file(const char *path, uint8_t **out_data, size_t *out_len) {
    *out_data = NULL;
    *out_len = 0;

    /* Binary mode: on Windows, text mode turns CR LF into LF and
       treats byte 0x1A as end of file, which corrupts binary data. */
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return PE_READ_OPEN_FAILED;
    }

    pe_read_status status = read_open_file(fp, out_data, out_len);
    fclose(fp);
    return status;
}

const char *pe_read_status_str(pe_read_status status) {
    switch (status) {
    case PE_READ_OK:
        return "ok";
    case PE_READ_OPEN_FAILED:
        return "cannot open file";
    case PE_READ_SEEK_FAILED:
        return "cannot determine file size";
    case PE_READ_EMPTY:
        return "file is empty";
    case PE_READ_NO_MEMORY:
        return "out of memory";
    case PE_READ_SHORT_READ:
        return "could not read the whole file";
    }
    return "unknown error";
}
