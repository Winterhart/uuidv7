/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * Unit tests for the UUIDv7 library.
 */

/* Feature-test macros: tests are POSIX-tier. */
#define _POSIX_C_SOURCE 200809L // NOLINT

#include "UUIDv7.h"

#include <inttypes.h>
#include <time.h>

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
static const uuidv7 RFC_A6_DATA = {
    .unix_ts_ms = {0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0},
    .ver_rand_a = {0x7C, 0xC3},
    .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
};
Test(uuidv7, string_len) {
    cr_assert_eq(UUIDV7_STRING_LEN, 37, "Must be 37 length");
}

Test(uuidv7, golden_new_uuidv7) {
    uuidv7 data = {0};
    const uuidv7_error err = uuidv7_new(&data);
    cr_assert_eq(err, UUIDV7_OK);
    char buff[UUIDV7_STRING_LEN] = {0};
    cr_assert_eq(uuidv7_to_string(&data, buff), UUIDV7_OK);
    cr_log(CR_LOG_INFO, "New UUID test, got string: %s", buff);
}

// RFC A6
Test(uuidv7, golden_rfc_9562_a6) {
    char buff[UUIDV7_STRING_LEN] = {0};
    cr_assert_eq(uuidv7_to_string(&RFC_A6_DATA, buff), UUIDV7_OK);
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

// non empty buff gets overwritten
Test(uuidv7, uuid_to_string_non_empty_buff) {
    char buff[UUIDV7_STRING_LEN];
    memset(buff, 0xFF, sizeof(buff));
    const uuidv7_error err = uuidv7_to_string(&RFC_A6_DATA, buff);
    cr_assert_eq(err, UUIDV7_OK);
    cr_assert_str_eq(buff, "017F22E2-79B0-7CC3-98C4-DC0C0C07398F");
}

// NUL PTR does not touch buff
Test(uuidv7, uuid_to_string_nul_val_dont_change_buf) {
    char buff[UUIDV7_STRING_LEN];
    memset(buff, 0xFF, sizeof(buff));

    // clean buff to compare
    char untouch_buff[UUIDV7_STRING_LEN];
    memset(untouch_buff, 0xFF, sizeof(untouch_buff));

    const uuidv7_error err = uuidv7_to_string(NULL, buff);
    cr_assert_eq(err, UUIDV7_ERR_NULL_PTR);
    cr_assert_arr_eq(untouch_buff, buff, sizeof(untouch_buff));
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

/* Performance smoke test + report */
Test(uuidv7, uuid_new_basic_perf) {
    const int max_loop = 100000;
    const uint64_t ns = 1000000000;
    /* Warm-up initialize kernel state. */
    uuidv7 warmup = {0};
    cr_assert_eq(uuidv7_new(&warmup), UUIDV7_OK);

    uint8_t sink = 0;
    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < max_loop; i++) {
        uuidv7 data = {0};
        const uuidv7_error err = uuidv7_new(&data);
        cr_assert_eq(err, UUIDV7_OK);
        sink ^= data.var_rand_b[0];
    }
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    /* Normalize in nanoseconds first, subtract once handles. */
    const uint64_t start_ns = ((uint64_t)t0.tv_sec * ns) + (uint64_t)t0.tv_nsec;
    const uint64_t end_ns = ((uint64_t)t1.tv_sec * ns) + (uint64_t)t1.tv_nsec;
    const uint64_t ela_ns = end_ns - start_ns;
    const uint64_t per_uuid_ns = ela_ns / (uint64_t)max_loop;
    const uint64_t uuid_per_sec = ela_ns != 0 ? max_loop * ns / ela_ns : 0;

    cr_log(CR_LOG_INFO,
           "perf new: %" PRIu64 " ns total, %" PRIu64 " ns/uuid, %" PRIu64
           " uuid/s (sink=%u)",
           ela_ns,
           per_uuid_ns,
           uuid_per_sec,
           sink);

    /* Smoke bound: fires only on pathological (>=1000x) regressions. */
    cr_assert_lt(per_uuid_ns, UINT64_C(1000000));
}
