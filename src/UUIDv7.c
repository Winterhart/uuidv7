/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * UUIDv7 implementation
 *
 * https://www.rfc-editor.org/info/rfc9562
 */
#include "UUIDv7.h"

#include <stdint.h>
#include <stdio.h>

static const char HEX_CHAR[17] = "0123456789ABCDEF";

// helper function takes a raw uint8 to nibble one and two e.g. 69 -> 45
static void nibble_map(const uint8_t *source, char *nZero, char *nOne) {
    // map the hex to char map
    *nZero = HEX_CHAR[(unsigned)*source >> 4U];
    *nOne = HEX_CHAR[(unsigned)*source & 0xFU];
}

uuidv7_error uuidv7_to_string(const uuidv7 *val, char *buf) {
    // no NULL ptr
    if (val == 0 || buf == 0) {
        return UUIDV7_ERR_NULL_PTR;
    }

    // RFC requirements
    if (((unsigned)val->ver_rand_a[0] >> 4U) != 0x7 ||
        ((unsigned)val->var_rand_b[0] >> 6U) != 0x2) {
        return UUIDV7_ERR_INVALID;
    }

    nibble_map(&val->unix_ts_ms[0], &buf[0], &buf[1]);
    nibble_map(&val->unix_ts_ms[1], &buf[2], &buf[3]);
    nibble_map(&val->unix_ts_ms[2], &buf[4], &buf[5]);
    nibble_map(&val->unix_ts_ms[3], &buf[6], &buf[7]);
    buf[8] = '-';
    nibble_map(&val->unix_ts_ms[4], &buf[9], &buf[10]);
    nibble_map(&val->unix_ts_ms[5], &buf[11], &buf[12]);
    buf[13] = '-';
    nibble_map(&val->ver_rand_a[0], &buf[14], &buf[15]);
    nibble_map(&val->ver_rand_a[1], &buf[16], &buf[17]);
    buf[18] = '-';
    nibble_map(&val->var_rand_b[0], &buf[19], &buf[20]);
    nibble_map(&val->var_rand_b[1], &buf[21], &buf[22]);
    buf[23] = '-';
    nibble_map(&val->var_rand_b[2], &buf[24], &buf[25]);
    nibble_map(&val->var_rand_b[3], &buf[26], &buf[27]);
    nibble_map(&val->var_rand_b[4], &buf[28], &buf[29]);
    nibble_map(&val->var_rand_b[5], &buf[30], &buf[31]);
    nibble_map(&val->var_rand_b[6], &buf[32], &buf[33]);
    nibble_map(&val->var_rand_b[7], &buf[34], &buf[35]);
    buf[36] = '\0';

    return UUIDV7_OK;
}

uuidv7_error uuidv7_new(uuidv7 *out) {
    // todo
    return UUIDV7_OK;
}
