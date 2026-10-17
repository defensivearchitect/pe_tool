/* pe_reader.c -- read an input file into memory. */
#include "pe_reader.h"

#include <stdio.h>
#include <stdlib.h>

pe_read_status pe_read_file(const char *path, uint8_t **out_data, size_t *out_len) {
    *out_data = NULL;
    *out_len = 0;

    /* Binary mode: on Windows, text mode turns CR LF into LF and
       treats byte 0x1A as end of file, which corrupts binary data. */
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return PE_READ_OPEN_FAILED;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return PE_READ_SEEK_FAILED;
    }

    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return PE_READ_SEEK_FAILED;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return PE_READ_SEEK_FAILED;
    }

    if (size == 0) {
        fclose(fp);
        return PE_READ_EMPTY;
    }

    size_t len = (size_t)size;
    uint8_t *data = malloc(len);
    if (data == NULL) {
        fclose(fp);
        return PE_READ_NO_MEMORY;
    }

    size_t got = fread(data, 1, len, fp);
    if (got != len) {
        free(data);
        fclose(fp);
        return PE_READ_SHORT_READ;
    }

    fclose(fp);
    *out_data = data;
    *out_len = len;
    return PE_READ_OK;
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
