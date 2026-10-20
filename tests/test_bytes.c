/* test_bytes.c -- tests for the bounds-checked byte readers. */
#include "pe_bytes.h"

#include <stdint.h>

#include "check.h"

static void test_u16_reads_little_endian(void) {
    const uint8_t data[] = {0x34, 0x12};
    uint16_t v = 0;
    CHECK(pe_read_u16_le(data, sizeof data, 0, &v));
    CHECK(v == 0x1234);
}

static void test_u16_last_offset_that_fits(void) {
    const uint8_t data[] = {0x00, 0xaa, 0xbb};
    uint16_t v = 0;
    CHECK(pe_read_u16_le(data, sizeof data, 1, &v));
    CHECK(v == 0xbbaa);
}

static void test_u16_rejects_out_of_bounds(void) {
    const uint8_t data[] = {0x34, 0x12};
    uint16_t v = 0x5555;
    CHECK(!pe_read_u16_le(data, sizeof data, 1, &v));
    CHECK(!pe_read_u16_le(data, sizeof data, 2, &v));
    CHECK(!pe_read_u16_le(data, 0, 0, &v));
    CHECK(!pe_read_u16_le(data, sizeof data, SIZE_MAX, &v));
    CHECK(v == 0x5555);
}

static void test_u32_reads_little_endian(void) {
    const uint8_t data[] = {0x78, 0x56, 0x34, 0x12};
    uint32_t v = 0;
    CHECK(pe_read_u32_le(data, sizeof data, 0, &v));
    CHECK(v == 0x12345678);
}

static void test_u32_top_byte_set(void) {
    const uint8_t data[] = {0x00, 0x00, 0x00, 0xff};
    uint32_t v = 0;
    CHECK(pe_read_u32_le(data, sizeof data, 0, &v));
    CHECK(v == 0xff000000);
}

static void test_u32_rejects_out_of_bounds(void) {
    const uint8_t data[] = {0x78, 0x56, 0x34, 0x12};
    uint32_t v = 0x55555555;
    CHECK(!pe_read_u32_le(data, sizeof data, 1, &v));
    CHECK(!pe_read_u32_le(data, 3, 0, &v));
    CHECK(!pe_read_u32_le(data, sizeof data, SIZE_MAX, &v));
    CHECK(!pe_read_u32_le(data, sizeof data, SIZE_MAX - 2, &v));
    CHECK(v == 0x55555555);
}

int main(void) {
    test_u16_reads_little_endian();
    test_u16_last_offset_that_fits();
    test_u16_rejects_out_of_bounds();
    test_u32_reads_little_endian();
    test_u32_top_byte_set();
    test_u32_rejects_out_of_bounds();

    return check_report();
}
