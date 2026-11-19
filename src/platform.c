/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * UUIDv7 implementation
 *
 * https://www.rfc-editor.org/info/rfc9562
 */

/* Allow to unhide the getentropy and clock */
#ifndef _WIN32
    #define _POSIX_C_SOURCE 200809L // NOLINT *
    #define _DEFAULT_SOURCE         // NOLINT *
#endif

#include "platform.h"

#include "UUIDv7.h"

#ifdef _WIN32
// prevent ordering windows and bcrypt in alph. order
// clang-format off
    #define _WIN32_WINNT 0x0602
    #include <windows.h>
    #include <bcrypt.h>
// clang-format on
#elif defined(__GLIBC__) || defined(__APPLE__) || defined(__OpenBSD__) ||      \
    defined(__FreeBSD__)
    #include <time.h>

    #include <sys/random.h>
#else
    #include <time.h>
    #include <unistd.h>
#endif

uuidv7_error uuidv7_get_entropy(uint8_t buf[]) {
#ifdef _WIN32
    const NTSTATUS status = BCryptGenRandom(NULL,
                                            (PUCHAR)buf,
                                            (ULONG)UUIDV7_ENTROPY_SIZE_LEN,
                                            BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (status != 0) {
        return UUIDV7_ERR_ENTROPY;
    }
#else
    const int result = getentropy(buf, UUIDV7_ENTROPY_SIZE_LEN);
    if (result != 0) {
        return UUIDV7_ERR_ENTROPY;
    }
#endif

    return UUIDV7_OK;
}

uuidv7_error uuidv7_clock_ms(uint64_t *time_ms) {
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);
    const uint64_t ticks_100ns =
        (uint64_t)ft.dwHighDateTime << 32U | ft.dwLowDateTime;
    const uint64_t ticks_since_1601 = UINT64_C(116444736000000000);
    *time_ms = (ticks_100ns - ticks_since_1601) / UINT64_C(10000);
#else
    struct timespec ts;
    const int result = clock_gettime(CLOCK_REALTIME, &ts);
    if (result != 0) {
        return UUIDV7_ERR_CLOCK;
    }

    *time_ms = ((uint64_t)ts.tv_sec * 1000) + ((uint64_t)ts.tv_nsec / 1000000);
#endif

    return UUIDV7_OK;
}
