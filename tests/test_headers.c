/* test_headers.c -- tests for locating the PE header. */
#include "pe_headers.h"

#include <stdint.h>
#include <string.h>

#include "check.h"

/* The smallest buffer that passes: "MZ" at 0, e_lfanew = 0x40, and
   "PE\0\0" at 0x40, ending exactly at the end of the buffer. */
static const uint8_t minimal[0x44] = {
    [0x00] = 'M', [0x01] = 'Z', [0x3c] = 0x40, [0x40] = 'P', [0x41] = 'E',
};

static void test_finds_signature(void) {
    size_t offset = 0;
    CHECK(pe_find_pe_header(minimal, sizeof minimal, &offset) == PE_HEADER_OK);
    CHECK(offset == 0x40);
}

static void test_rejects_short_files(void) {
    size_t offset = 0x5555;
    CHECK(pe_find_pe_header(minimal, 0, &offset) == PE_HEADER_DOS_TRUNCATED);
    CHECK(pe_find_pe_header(minimal, 1, &offset) == PE_HEADER_DOS_TRUNCATED);
    /* "MZ" and nothing else. */
    CHECK(pe_find_pe_header(minimal, 2, &offset) == PE_HEADER_DOS_TRUNCATED);
    /* e_lfanew is cut off by one byte. */
    CHECK(pe_find_pe_header(minimal, 0x3f, &offset) == PE_HEADER_DOS_TRUNCATED);
    CHECK(offset == 0x5555);
}

static void test_rejects_missing_mz(void) {
    const uint8_t text[] = {'P', 'E'};
    uint8_t data[sizeof minimal];
    size_t offset = 0x5555;

    CHECK(pe_find_pe_header(text, sizeof text, &offset) == PE_HEADER_NOT_MZ);

    /* The right bytes in the wrong order. */
    memcpy(data, minimal, sizeof data);
    data[0] = 'Z';
    data[1] = 'M';
    CHECK(pe_find_pe_header(data, sizeof data, &offset) == PE_HEADER_NOT_MZ);
    CHECK(offset == 0x5555);
}

static void test_rejects_lfanew_outside_file(void) {
    uint8_t data[sizeof minimal];
    size_t offset = 0x5555;

    /* The DOS header is complete, but the signature would start right
       after the end of a 64-byte file. */
    CHECK(pe_find_pe_header(minimal, 0x40, &offset) == PE_HEADER_BAD_LFANEW);

    /* One byte past the last offset that fits. */
    memcpy(data, minimal, sizeof data);
    data[0x3c] = 0x41;
    CHECK(pe_find_pe_header(data, sizeof data, &offset) ==
          PE_HEADER_BAD_LFANEW);

    /* -1 as a signed LONG, 0xffffffff when read as unsigned. */
    data[0x3c] = 0xff;
    data[0x3d] = 0xff;
    data[0x3e] = 0xff;
    data[0x3f] = 0xff;
    CHECK(pe_find_pe_header(data, sizeof data, &offset) ==
          PE_HEADER_BAD_LFANEW);
    CHECK(offset == 0x5555);
}

static void test_rejects_wrong_signature(void) {
    uint8_t data[sizeof minimal];
    size_t offset = 0x5555;

    /* "NE" marks a 16-bit Windows program, not a PE file. */
    memcpy(data, minimal, sizeof data);
    data[0x40] = 'N';
    CHECK(pe_find_pe_header(data, sizeof data, &offset) == PE_HEADER_NOT_PE);

    /* "PE" must be followed by two zero bytes. */
    memcpy(data, minimal, sizeof data);
    data[0x42] = 'X';
    CHECK(pe_find_pe_header(data, sizeof data, &offset) == PE_HEADER_NOT_PE);
    CHECK(offset == 0x5555);
}

int main(void) {
    test_finds_signature();
    test_rejects_short_files();
    test_rejects_missing_mz();
    test_rejects_lfanew_outside_file();
    test_rejects_wrong_signature();
    return check_report();
}
