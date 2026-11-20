# UUIDv7

C Library for [UUIDv7](https://www.rfc-editor.org/info/rfc9562/)

- Average of `630 ns` per `UUID` or 1.6 million/second
- No batching or pre-aggragating of the entropy
- Hand written
- Attempt to support as many platform as possible
- Maps instead of loops
- System does not use pre-gen entropy number

## How AI was used?

Mostly hybrid model:

- The workflow scripts were generated
- The `Makefiles` and C setup artifacts were generated
- The core code and test was hand-written

## RFC

```

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           unix_ts_ms                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          unix_ts_ms           |  ver  |       rand_a          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|var|                        rand_b                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            rand_b                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


UUIDv7 features a time-ordered value field derived from the widely implemented and well-known > Unix Epoch timestamp source, the number of milliseconds since midnight 1 Jan 1970 UTC, leap seconds excluded. Generally, UUIDv7 has improved entropy characteristics over UUIDv1 (Section 5.1) or UUIDv6 (Section 5.6).

UUIDv7 values are created by allocating a Unix timestamp in milliseconds [...]

```

https://www.rfc-editor.org/info/rfc9562/#name-uuid-version-7

## Build and test

Attempt to make it portable across Unix systems (macOS, Linux). Prerequisites:

- CMake >= 3.21 and `make`
- A C11 compiler (clang or gcc)
- `pkg-config`
- [Criterion](https://github.com/Snaipe/Criterion) for the test suite

Run the tests:

```
make test
```
