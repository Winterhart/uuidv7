# UUIDv7

C Library for [UUIDv7](https://www.rfc-editor.org/info/rfc9562/)

## RFC

```

UUIDv7 features a time-ordered value field derived from the widely implemented and well-known > Unix Epoch timestamp source, the number of milliseconds since midnight 1 Jan 1970 UTC, leap seconds excluded. Generally, UUIDv7 has improved entropy characteristics over UUIDv1 (Section 5.1) or UUIDv6 (Section 5.6).

UUIDv7 values are created by allocating a Unix timestamp in milliseconds in the most significant 48 bits and filling the remaining 74 bits, excluding the required version and variant bits, with random bits for each new UUIDv7 generated to provide uniqueness as per Section 6.9. Alternatively, implementations MAY fill the 74 bits, jointly, with a combination of the following subfields, in this order from the most significant bits to the least, to guarantee additional monotonicity within a millisecond:

1. An OPTIONAL sub-millisecond timestamp fraction (12 bits at maximum) as per Section 6.2 (Method 3).
2. An OPTIONAL carefully seeded counter as per Section 6.2 (Method 1 or 2).
3. Random data for each new UUIDv7 generated for any remaining space.

Implementations SHOULD utilize UUIDv7 instead of UUIDv1 and UUIDv6 if possible.

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

```

https://www.rfc-editor.org/info/rfc9562/#name-uuid-version-7

## Build and test

Portable across Unix systems (macOS, Linux). Prerequisites:

- CMake >= 3.21 and `make`
- A C11 compiler (clang or gcc)
- `pkg-config`
- [Criterion](https://github.com/Snaipe/Criterion) for the test suite, installed
  from your system package manager (Criterion is not packaged in ConanCenter
  or vcpkg; system packages are its designed distribution channel):
  - macOS: `brew install criterion`
  - Debian/Ubuntu: `apt install libcriterion-dev`
  - Arch: `pacman -S criterion`
  - Nix: `nix-shell -p criterion`

Build and run the tests:

```
make test
```

The configure step picks a preset per machine: on macOS it pins Homebrew's
clang so the generated compile database matches the Homebrew clang-tidy run
by `make tidy`; on other systems it uses the compiler from the environment.

## Packaging (Conan)

The library is also a Conan package:

```
conan profile detect --force   # once per machine: detect local toolchain
conan create .                 # build package + run the consumer smoke test
```

`conan create` runs `test_package/`, a small consumer project that links the
packaged library from the Conan cache and verifies the RFC 9562 Appendix A.6
golden vector end to end — testing the *artifact*, not the source tree. After
it succeeds, any project can depend on UUIDv7:

```
conan install --requires=uuidv7/0.1
```

and in CMake:

```cmake
find_package(UUIDv7 REQUIRED)
target_link_libraries(myapp PRIVATE UUIDv7::UUIDv7)
```
