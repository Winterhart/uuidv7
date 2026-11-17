/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * UUIDv7 values are created by allocating a Unix timestamp in milliseconds.
 * The ts gets saved in the most significant 48 bits
 *
 * https://www.rfc-editor.org/info/rfc9562
 */

#include "UUIDv7.h"

#include <stdint.h>
/**
 * Platform adapter function to get entropy
 */
uuidv7_error uuidv7_get_entropy(uint8_t buf[]);

/*
 * Platform adapter function to get time
 */
uuidv7_error uuidv7_clock_ms(uint64_t *time_ms);
