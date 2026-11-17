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

#include <unistd.h>

#include <criterion/criterion.h>

#include "../src/platform.h"

Test(uuidv7, golden_uuidv7_get_clock) {
    uint64_t got_time = 0;
    const uuidv7_error err = uuidv7_clock_ms(&got_time);
    cr_assert_eq(err, UUIDV7_OK);
    cr_log(CR_LOG_INFO, "Got Clock Time: %llu", got_time);
    cr_assert_gt(got_time, 0);
    sleep(1); // NOLINT
    uint64_t second_time = 0;
    const uuidv7_error err2 = uuidv7_clock_ms(&second_time);
    cr_assert_eq(err2, UUIDV7_OK);
    cr_log(CR_LOG_INFO, "Got Second Clock Time : %llu", got_time);
    // second time is bigger than first time
    cr_assert_gt(second_time, got_time);
}
