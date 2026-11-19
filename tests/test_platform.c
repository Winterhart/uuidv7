/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * Unit tests for the UUIDv7 library.
 */
#include "UUIDv7.h"

#include <inttypes.h>
#include <unistd.h>

#include <criterion/criterion.h>

#include "../src/platform.h"

Test(uuidv7, golden_uuidv7_get_clock) {
    uint64_t got_time = 0;
    const uuidv7_error err = uuidv7_clock_ms(&got_time);
    cr_assert_eq(err, UUIDV7_OK);
    cr_log(CR_LOG_INFO, "Got Clock Time: %llu", got_time);
    cr_assert_gt(got_time, 0);
#ifdef _WIN32
    Sleep(1);
#else
    sleep(1); // NOLINT
#endif
    uint64_t second_time = 0;
    const uuidv7_error err2 = uuidv7_clock_ms(&second_time);
    cr_assert_eq(err2, UUIDV7_OK);
    // cr_log(CR_LOG_INFO, "Got Second Clock Time : %lu", got_time);
    cr_log(CR_LOG_INFO, "Got Clock Time: %" PRIu64, got_time);
    // second time is bigger than first time
    cr_assert_gt(second_time, got_time);
}
