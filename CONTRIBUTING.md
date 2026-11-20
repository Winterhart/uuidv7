# Contributing

The main repository is hosted offline this is a mirror
Your PR might be integrated to the offline repository and re-push to GitHub

## Build & verify

- `cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && (cd build && ctest --output-on-failure)`
- or: `make test` (build + run Criterion suite)
- `make tidy` (clang-tidy, must be clean)
- `make fmt` (clang-format check)

## Conventions

- Pass `make test` and `make tidy`.
- Target is `RFC 9562` conformance; keep the Appendix A.6 golden vector green.
- Style enforced by `.clang-format` and `.clang-tidy` — run `make fmt` before pushing.
- Keep the public C API stable; add rather than break.
- License is MIT, Copyright Farfatech Inc.; commits must be your own work or clearly attributed.

## Reporting security issues

- See SECURITY.md — do not use public issues for vulnerabilities.
