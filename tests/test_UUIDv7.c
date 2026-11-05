/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * Unit tests for the UUIDv7 library.
 *
 * Run with `make test` (CTest, exit-code contract) or directly:
 *   ./build/uuidv7_tests --verbose
 */
#include <criterion/criterion.h>
#include <string.h>

#include "UUIDv7.h"

/* RFC 9562 Appendix A.6 test vector (Figure 26):
 *   unix_ts_ms = 0x017F22E279B0 (1645557742000)
 *   ver = 0x7, rand_a = 0xCC3, var = 0b10,
 *   rand_b = 0x8C4DC0C0C07398F
 *   string: "017F22E2-79B0-7CC3-98C4-DC0C0C07398F"
 */
static const uuidv7 RFC_A6_VECTOR = {
    .unix_ts_ms = {0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0},
    .ver_rand_a = {0x7C, 0xC3},
    .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
};

/* Golden test: the RFC's own example, character for character. */
Test(uuidv7_to_string, rfc9562_appendix_a6_vector) {
    char buf[UUIDV7_STRING_LEN];

    cr_assert_eq(uuidv7_to_string(&RFC_A6_VECTOR, buf), UUIDV7_OK,
                 "expected UUIDV7_OK");

    cr_assert_str_eq(buf, "017F22E2-79B0-7CC3-98C4-DC0C0C07398F",
                     "RFC 9562 Figure 26 golden vector mismatch");

    cr_assert_eq(buf[UUIDV7_STRING_LEN - 1], '\0',
                 "output must be NUL-terminated at buf[36]");
}

/* Error contract: NULL inputs must be rejected, never dereferenced. */
Test(uuidv7_to_string, null_val_returns_error) {
    char buf[UUIDV7_STRING_LEN];

    cr_assert_eq(uuidv7_to_string(NULL, buf), UUIDV7_ERR_NULL_PTR,
                 "NULL val must return UUIDV7_ERR_NULL_PTR");
}

Test(uuidv7_to_string, null_buf_returns_error) {
    cr_assert_eq(uuidv7_to_string(&RFC_A6_VECTOR, NULL), UUIDV7_ERR_NULL_PTR,
                 "NULL buf must return UUIDV7_ERR_NULL_PTR");
}

/* The error path must write nothing: poison the buffer and verify
 * that a failed call leaves every byte untouched. */
Test(uuidv7_to_string, error_writes_nothing) {
    char buf[UUIDV7_STRING_LEN];

    memset(buf, 0xFF, sizeof buf);
    cr_assert_eq(uuidv7_to_string(NULL, buf), UUIDV7_ERR_NULL_PTR);

    for (size_t i = 0; i < sizeof buf; i++) {
        cr_assert_eq((unsigned char)buf[i], 0xFF,
                     "byte %zu must be untouched on error", i);
    }
}

/* All 37 slots must be written by a successful call: poison the
 * buffer and verify no 0xFF survives (output is ASCII hex, dashes
 * and NUL — never 0xFF). This guards against a dropped line in the
 * manual byte->position mapping. */
Test(uuidv7_to_string, writes_all_slots) {
    char buf[UUIDV7_STRING_LEN];

    memset(buf, 0xFF, sizeof buf);
    cr_assert_eq(uuidv7_to_string(&RFC_A6_VECTOR, buf), UUIDV7_OK);

    for (size_t i = 0; i < sizeof buf; i++) {
        cr_assert_neq((unsigned char)buf[i], 0xFF,
                     "slot %d was never written", (int)i);
    }
    cr_assert_eq(buf[UUIDV7_STRING_LEN - 1], '\0');
}
