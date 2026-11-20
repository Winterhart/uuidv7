/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * UUIDv7 values are created by allocating a Unix timestamp in milliseconds.
 * The ts gets saved in the most significant 48 bits
 *
 * https://www.rfc-editor.org/info/rfc9562
 */

#ifndef UUIDV7_H
#define UUIDV7_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Max len of uuidv7 as a string */
#define UUIDV7_STRING_LEN 37

/* Max len of entropy */
#define UUIDV7_ENTROPY_SIZE_LEN 10

/* uuidv7 main struct */
typedef struct {
    uint8_t unix_ts_ms[6]; /** 48-bit timestamp ms */
    uint8_t ver_rand_a[2]; /** starts with ver 4-bit version then 12
                              bits of pseudorandom data to provide uniqueness */
    uint8_t var_rand_b[8]; /** starts 2-bit variant then 62 bits of
                           pseudorandom data */
} uuidv7;

/**
 * uuidv7 errors
 */
typedef enum {
    UUIDV7_OK = 0,
    UUIDV7_ERR_CLOCK,
    UUIDV7_ERR_ENTROPY,
    UUIDV7_ERR_NULL_PTR,
    UUIDV7_ERR_INVALID,
} uuidv7_error;

uuidv7_error uuidv7_new(uuidv7 *out);
/**
 * saves string of 37 cha\: 32 hex, 4 dash, 1 NUL in buffer
 */
uuidv7_error uuidv7_to_string(const uuidv7 *val, char buf[UUIDV7_STRING_LEN]);

#ifdef __cplusplus
}
#endif
#endif /* UUIDV7_H */
