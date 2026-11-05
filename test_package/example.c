/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Consumes the UUIDv7 package exactly as an end user would: include the
 * public header, format the RFC 9562 Appendix A.6 golden vector, verify.
 * Exit code is the verdict (CTest contract).
 */
#include <assert.h>
#include <string.h>

#include "UUIDv7.h"

int main(void) {
    const uuidv7 v = {
        .unix_ts_ms = {0x01, 0x7F, 0x22, 0xE2, 0x79, 0xB0},
        .ver_rand_a = {0x7C, 0xC3},
        .var_rand_b = {0x98, 0xC4, 0xDC, 0x0C, 0x0C, 0x07, 0x39, 0x8F},
    };
    char buf[UUIDV7_STRING_LEN];

    assert(uuidv7_to_string(&v, buf) == UUIDV7_OK);
    assert(strcmp(buf, "017F22E2-79B0-7CC3-98C4-DC0C0C07398F") == 0);
    return 0;
}
