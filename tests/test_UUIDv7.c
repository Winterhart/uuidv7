/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * Unit tests for the UUIDv7 library.
 *
 * Criterion dialect: each Test(suite, name) is auto-registered; run with
 *   make test        (ctest, exit-code contract)
 * or  ./build/uuidv7_tests --verbose   (Criterion's own runner output)
 */
#include <criterion/criterion.h>

#include "UUIDv7.h"

/* ---------------------------------------------------------------------------
 * Smoke test: proves the harness works end to end (discover, compile, link,
 * run). A green bar here means every failure below is a REAL test failure,
 * not infrastructure noise. Once the real tests are green, feel free to
 * delete this one.
 * ------------------------------------------------------------------------- */
Test(uuidv7, harness_smoke) {
    cr_assert_eq(UUIDV7_STRING_LEN, 37, "32 hex + 4 dash + 1 NUL = 37");
}

/* ---------------------------------------------------------------------------
 * TODO(you) 1 — Golden test: RFC 9562 Appendix A.6 test vector.
 *
 *   struct bytes (big-endian):
 *     unix_ts_ms = { 0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0 }   (1645557742000)
 *     ver_rand_a = { 0x7C, 0xC3 }                           (ver 7, rand_a 0xCC3)
 *     var_rand_b = { 0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F }
 *
 *   expected string (RFC Figure 26, uppercase — matches our formatter):
 *     "017F22E2-79B0-7CC3-98C4-DC0C0C07398F"
 *
 *   assert: return code, the full string, and buf[36] == '\0'.
 * ------------------------------------------------------------------------- */
Test(uuidv7_to_string, TODO_rfc9562_appendix_a6_vector) {
    cr_assert_fail("write me");
}

/* TODO(you) 2 — Error contract: NULL val, and NULL buf.
 *   each call must return UUIDV7_ERR_NULL_PTR (and not crash).            */

/* TODO(you) 3 — Untouched output on error: poison buf with 0xFF, call with
 *   NULL val, assert every byte is still 0xFF (the error path writes
 *   nothing).                                                              */

/* TODO(you) 4 — All-slots-written: poison buf, run a valid call, assert no
 *   0xFF survives (proves all 37 positions are written).                  */
