/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * Unit tests for the UUIDv7 library.
 *
 * YOUR assignment: the five tests described below. The version this file
 * replaces is retired from the working tree but preserved in git history
 * (commit 1d23202) — compare with it only AFTER you have written your own.
 *
 * Run with `make test`.
 */
#include "UUIDv7.h"

#include <criterion/criterion.h>

/* Reference data — RFC 9562 Appendix A.6 (Figure 26):
 *
 *   unix_ts_ms = 0x017F22E279B0   (1645557742000)
 *   ver        = 0x7
 *   rand_a     = 0xCC3
 *   var        = 0b10
 *   rand_b     = 0x8C4DC0C0C07398F
 *   string     = "017F22E2-79B0-7CC3-98C4-DC0C0C07398F"
 *
 * The same value as struct fields (big-endian, MSB first):
 *   unix_ts_ms = { 0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0 }
 *   ver_rand_a = { 0x7C, 0xC3 }
 *   var_rand_b = { 0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F }
 */

const char zero_buff[UUIDV7_STRING_LEN] = {0};
Test(uuidv7, string_len) {
    cr_assert_eq(UUIDV7_STRING_LEN, 37, "Must be 37 length");
}

// RFC A6
Test(uuidv7, golden_rfc_9562_a6) {
    const uuidv7 data = {
        .unix_ts_ms = {0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0},
        .ver_rand_a = {0x7C, 0xC3},
        .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
    };
    char buff[UUIDV7_STRING_LEN] = {0};
    cr_assert_eq(uuidv7_to_string(&data, buff), UUIDV7_OK);
    cr_log(CR_LOG_INFO, "Got UUID string: %s", buff);
    cr_assert_str_eq(buff, "017F22E2-79B0-7CC3-98C4-DC0C0C07398F");
}
// Golden Test for to_string
Test(uuidv7, uuid_to_string_happy_path) {
    uuidv7 data = {
        .unix_ts_ms = {0x01, 0xA1, 0xBC, 0x50, 0x8C, 0xF0},
        .ver_rand_a = {0x7c, 0xC3},
        .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
    };
    char buff[UUIDV7_STRING_LEN] = {0};
    const uuidv7_error err = uuidv7_to_string(&data, buff);
    cr_log(CR_LOG_INFO, "Got UUID string: %s", buff);

    cr_assert_eq(err, UUIDV7_OK);
    cr_assert_str_eq(buff, "01A1BC50-8CF0-7CC3-98C4-DC0C0C07398F");
    // last char is NUL
    cr_assert_eq(buff[UUIDV7_STRING_LEN - 1], '\0');
}

Test(uuidv7, uuid_to_string_err_nul_val) {
    char buff[UUIDV7_STRING_LEN] = {0};
    const uuidv7_error err = uuidv7_to_string(0, buff);
    cr_assert_eq(err, UUIDV7_ERR_NULL_PTR);
    // Array are not touch on error
    cr_assert_arr_eq(buff, zero_buff, UUIDV7_STRING_LEN);
}

Test(uuidv7, uuid_to_string_err_invalid_v8) {
    uuidv7 data = {
        .unix_ts_ms = {0x01, 0xA1, 0xBC, 0x50, 0x8C, 0xF0},
        .ver_rand_a = {0x8c, 0xC3},
        .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
    };
    char buff[UUIDV7_STRING_LEN] = {0};
    const uuidv7_error err = uuidv7_to_string(&data, buff);
    cr_assert_eq(err, UUIDV7_ERR_INVALID);
    // Array are not touch on error
    cr_assert_arr_eq(buff, zero_buff, UUIDV7_STRING_LEN);
}

Test(uuidv7, uuid_to_string_err_invalid_var_b) {
    uuidv7 data = {
        .unix_ts_ms = {0x01, 0xA1, 0xBC, 0x50, 0x8C, 0xF0},
        .ver_rand_a = {0x7c, 0xC3},
        .var_rand_b = {0x01, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
    };
    char buff[UUIDV7_STRING_LEN] = {0};
    const uuidv7_error err = uuidv7_to_string(&data, buff);
    cr_assert_eq(err, UUIDV7_ERR_INVALID);
    // Array are not touch on error
    cr_assert_arr_eq(buff, zero_buff, UUIDV7_STRING_LEN);
}

/* ---------------------------------------------------------------------------
 *
 * 2. Error contract: NULL val must return UUIDV7_ERR_NULL_PTR.
 *
 * 3. Error contract: NULL buf must return UUIDV7_ERR_NULL_PTR.
 *
 * 4. Error writes nothing: fill buf with 0xFF, call with NULL val, assert
 *    every one of the 37 bytes is still 0xFF afterwards.
 *
 * 5. All slots written: fill buf with 0xFF, valid call, assert no 0xFF
 *    survives anywhere in the buffer (the output alphabet — hex chars,
 *    dashes, NUL — can never contain 0xFF).
 *
 * Criterion vocabulary you will need:
 *   Test(suite, name)              — declares + auto-registers a test
 *   cr_assert_eq(a, b, "fmt", ...) — assert equality, fatal on failure
 *   cr_assert_str_eq(a, b)         — compare strings by content
 *
 * Two traps to remember from our lessons:
 *   - The project compiles with -Wconversion: comparing a char against
 *     0xFF wants an (unsigned char) cast, not a raw comparison.
 *   - memset lives in <string.h>.
 * -------------------------------------------------------------------------
 */
