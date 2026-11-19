# UUIDv7

C Library for [UUIDv7](https://www.rfc-editor.org/info/rfc9562/)

- Average of `630` ns per `UUID` or 1.6 million/second
- No batching or pre-aggragating of the entropy
- Hand written
- Attempt to support as many platform as possible
- Maps instead of loops
- System does not use pre-gen entropy number

## How AI was used?

Mostly, for learning and setupping the workspace

## RFC

```

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
