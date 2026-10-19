/* pe_bytes.c -- bounds-checked reads of little-endian values. */
#include "pe_bytes.h"

/* True if size bytes starting at offset lie inside a buffer of len
   bytes. The check uses a subtraction, so a huge offset cannot wrap
   around and pass it. */
static bool fits(size_t len, size_t offset, size_t size) {
    return size <= len && offset <= len - size;
}

bool pe_read_u16_le(const uint8_t *data, size_t len, size_t offset,
                    uint16_t *out) {
    if (!fits(len, offset, 2)) {
        return false;
    }
    *out = (uint16_t)((uint32_t)data[offset] | (uint32_t)data[offset + 1] << 8);
    return true;
}
