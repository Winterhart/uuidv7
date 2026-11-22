/* SPDX-License-Identifier: MIT
 * UUIDv7 - RFC 9562
 * Copyright (c) 2026 Farfatech Inc.
 * UUIDv7 CLI
 */

#include "UUIDv7.h"

#include <stdio.h>

void log_error(uuidv7_error err) {
    const int sent = fprintf(stderr, "could not create UUIDv7, error: %d", err);
    if (sent < 0) {
        printf("could not log error");
    }
}

int main(void) {
    uuidv7 u = {0};
    uuidv7_error err = uuidv7_new(&u);
    if (err != UUIDV7_OK) {
        log_error(err);
        return 1;
    }

    // convert to string
    char buf[UUIDV7_STRING_LEN] = {0};
    err = uuidv7_to_string(&u, buf);
    if (err != UUIDV7_OK) {
        log_error(err);
        return 1;
    }

    const int result = printf("%s\n", buf);
    if (result < 0) {
        return 1;
    }

    if (fflush(stdout) != 0) {
        return 1;
    }
    return 0;
}
