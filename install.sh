#!/bin/sh
# UUIDv7 installer — macOS/Linux (Windows: install.ps1)
set -eu

base='https://github.com/Winterhart/uuidv7/releases/latest/download'
die() { printf 'install: %s\n' "$*" >&2; exit 1; }

case "$(uname -s) $(uname -m)" in
    Darwin*) asset=uuidv7-macos-universal ;;
    "Linux x86_64" | "Linux amd64") asset=uuidv7-linux-x86_64 ;;
    "Linux aarch64" | "Linux arm64") asset=uuidv7-linux-arm64 ;;
    *) die "unsupported platform (Windows: install.ps1)" ;;
esac

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

fetch() { curl -fsSL "$1" -o "$2" 2>/dev/null || wget -qO "$2" "$1" || die "download failed: $1"; }
fetch "$base/$asset" "$tmp/$asset"
fetch "$base/SHA256SUMS" "$tmp/SHA256SUMS"

want=$(grep " $asset\$" "$tmp/SHA256SUMS" | cut -d' ' -f1)
[ -n "$want" ] || die "no checksum for $asset in SHA256SUMS"
got=$( (sha256sum "$tmp/$asset" 2>/dev/null || shasum -a 256 "$tmp/$asset") | cut -d' ' -f1)
[ "$got" = "$want" ] || die "checksum mismatch for $asset"

dest=/usr/local/bin
s=''
if [ ! -w "$dest" ]; then
    if [ -d "$dest" ] && command -v sudo >/dev/null 2>&1; then s=sudo
    else dest=$HOME/.local/bin; mkdir -p "$dest"; fi
fi

$s cp "$tmp/$asset" "$dest/uuidv7"
$s chmod 755 "$dest/uuidv7"
"$dest/uuidv7" >/dev/null || die "binary failed to run"

case ":$PATH:" in *":$dest:"*) ;; *) echo "install: note: $dest is not on your PATH" >&2 ;; esac
echo "install: done — run: uuidv7"
