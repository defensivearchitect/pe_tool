/* pe_headers.h -- locate and check the headers of a PE file. */
#ifndef PE_HEADERS_H
#define PE_HEADERS_H

#include <stddef.h>
#include <stdint.h>

/* Result of pe_find_pe_header(). */
typedef enum {
    PE_HEADER_OK = 0,
    PE_HEADER_DOS_TRUNCATED, /* the file ends inside the DOS header */
    PE_HEADER_NOT_MZ,        /* the file does not start with "MZ" */
    PE_HEADER_BAD_LFANEW,    /* e_lfanew points outside the file */
    PE_HEADER_NOT_PE,        /* no "PE\0\0" signature at e_lfanew */
} pe_header_status;

/* Follow e_lfanew from the DOS header to the PE signature. On success,
   store the file offset of the signature in *out_offset and return
   PE_HEADER_OK. On failure, return the reason and leave *out_offset
   unchanged. */
pe_header_status pe_find_pe_header(const uint8_t *data, size_t len,
                                   size_t *out_offset);

/* Short description of a status, for error messages. */
const char *pe_header_status_str(pe_header_status status);

#endif
