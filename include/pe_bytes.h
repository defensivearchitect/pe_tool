/* pe_bytes.h -- bounds-checked reads of little-endian values. */
#ifndef PE_BYTES_H
#define PE_BYTES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Read the 16-bit little-endian value that starts at data[offset].
   If the value does not fit inside the first len bytes of data, read
   nothing and return false. */
bool pe_read_u16_le(const uint8_t *data, size_t len, size_t offset,
                    uint16_t *out);

#endif
