# UUIDv7

C Library for [UUIDv7](https://www.rfc-editor.org/info/rfc9562/)

- Average of `630 ns` per `UUID` or 1.6 million/second
- No batching or pre-aggragating of the entropy
- Hand written
- Attempt to support as many platform as possible
- Maps instead of loops
- System does not use pre-gen entropy number

## Install

Prebuilt binaries for every release live on the [GitHub releases page](https://github.com/Winterhart/uuidv7/releases). Both installers verify the SHA-256 checksum published with the release before copying anything.

**macOS / Linux**

```
curl -fsSL https://raw.githubusercontent.com/Winterhart/uuidv7/main/install.sh | sh
```

**Windows (PowerShell)**

```
irm https://raw.githubusercontent.com/Winterhart/uuidv7/main/install.ps1 | iex
```

Manual download: grab the binary for your platform from the [latest release](https://github.com/Winterhart/uuidv7/releases/latest) and check it against `SHA256SUMS`:

```
sha256sum uuidv7-linux-x86_64                       # Linux
shasum -a 256 uuidv7-macos-universal               # macOS
certutil -hashfile uuidv7-windows-x86_64.exe SHA256 # Windows
```

Every release binary is built by CI from the tagged commit and carries a
[GitHub artifact attestation](https://docs.github.com/en/actions/security-for-github-actions/using-artifact-attestations/using-artifact-attestations-to-establish-provenance-for-builds):

```
gh attestation verify uuidv7 --repo Winterhart/uuidv7
```

The Linux binaries are static (musl) and run on any distro; the macOS binary is universal (Apple Silicon + Intel).

### Usage

```
$ uuidv7
01A2013D-0237-729F-8F0A-1DFAD9FAD20D
```

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
