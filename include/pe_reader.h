/* pe_reader.h -- read an input file into memory. */
#ifndef PE_READER_H
#define PE_READER_H

#include <stddef.h>
#include <stdint.h>

/* Read the whole file at path into memory. On success, return the 
   buffer and store its length in *out_len; the caller must free the
   buffer. On failure, print the reason and return NULL. */
uint8_t *pe_read_file(const char *path, size_t *out_len);

#endif

