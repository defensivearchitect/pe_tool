/* pe_headers.c -- locate and check the headers of a PE file. */
#include "pe_headers.h"

#include "pe_bytes.h"

/* "MZ" and "PE\0\0" read as little-endian numbers. */
#define DOS_MAGIC 0x5a4du
#define PE_SIGNATURE 0x00004550u

/* Offset of e_lfanew, the last field of the 64-byte DOS header. */
#define LFANEW_OFFSET 0x3cu

pe_header_status pe_find_pe_header(const uint8_t *data, size_t len,
                                   size_t *out_offset) {
    uint16_t magic = 0;
    if (!pe_read_u16_le(data, len, 0, &magic)) {
        return PE_HEADER_DOS_TRUNCATED;
    }
    if (magic != DOS_MAGIC) {
        return PE_HEADER_NOT_MZ;
    }

    /* Microsoft declares e_lfanew as a signed LONG. Read as unsigned,
       a negative value becomes a huge offset that the bounds check
       below rejects like any other offset outside the file. */
    uint32_t lfanew = 0;
    if (!pe_read_u32_le(data, len, LFANEW_OFFSET, &lfanew)) {
        return PE_HEADER_DOS_TRUNCATED;
    }

    uint32_t signature = 0;
    if (!pe_read_u32_le(data, len, lfanew, &signature)) {
        return PE_HEADER_BAD_LFANEW;
    }
    if (signature != PE_SIGNATURE) {
        return PE_HEADER_NOT_PE;
    }

    *out_offset = lfanew;
    return PE_HEADER_OK;
}

const char *pe_header_status_str(pe_header_status status) {
    switch (status) {
    case PE_HEADER_OK:
        return "ok";
    case PE_HEADER_DOS_TRUNCATED:
        return "file is too small for a DOS header";
    case PE_HEADER_NOT_MZ:
        return "not an MZ executable";
    case PE_HEADER_BAD_LFANEW:
        return "PE header offset points outside the file";
    case PE_HEADER_NOT_PE:
        return "no PE signature";
    }
    return "unknown error";
}
