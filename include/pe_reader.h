/* pe_reader.h -- read an input file into memory. */
#ifndef PE_READER_H
#define PE_READER_H

#include <stddef.h>
#include <stdint.h>

/* Result of the pe_read_file(). */
typedef enum {
    PE_READ_OK = 0,
    PE_READ_OPEN_FAILED, /* errno holds the reason */
    PE_READ_SEEK_FAILED,
    PE_READ_EMPTY,
    PE_READ_NO_MEMORY,
    PE_READ_SHORT_READ,
} pe_read_status;

/* Read the whole file at path into memory. On success, store the
   buffer in *out_data and its length in *out_len, and return
   PE_READ_OK; the caller must free the buffer. On failure, return
   the reason; *out_data is then NULL. */
pe_read_status pe_read_file(const char *path, uint8_t **out_data, size_t *out_len);

/* Short description of a status, for error messages. */
const char *pe_read_status_str(pe_read_status status); 

#endif
