#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
dest="$root/.toolchain"
downloads="$dest/downloads"
mkdir -p "$downloads"

gcc_release=https://github.com/decompals/old-gcc/releases/download/0.13

maspsx_commit=423ffc6bca05e53f9b7ab06159d0cbe71111bc90
maspsx_url="https://github.com/mkst/maspsx/archive/$maspsx_commit.zip"
maspsx_archive="$downloads/maspsx-$maspsx_commit.zip"
maspsx_sha=3894086ee2c662f2cbc565a1b1fdd0e2231a2a90db132fe788eba1a6cb7d62d2
binutils_name=binutils-mipsel-linux-gnu_2.42-2ubuntu1cross5_amd64.deb
binutils_url=https://archive.ubuntu.com/ubuntu/pool/universe/b/binutils-mipsen/$binutils_name
binutils_archive="$downloads/$binutils_name"
binutils_sha=678275fa6abda0b7fabbc55ff6a09654701a6c2e86794c7a0b6e773e663e49db

# Compiler selection is per original object. EA-era code is known to span the
# 2.7.2, 2.8.0, and 2.8.1 PSX compiler releases.
for spec in \
    gcc-2.8.1-psx:51e94d84906d5ac1fa58cf66b7eed43e2c995526ef71df6ff5562df0177117e3 \
    gcc-2.8.0-psx:6e3aa53f117d9957feca444e9bbc9b5435b70624fd629a4dd306efdbe5d25f54 \
    gcc-2.7.2-cdk:42bb0df96db11a9b5d2e23d78bdc962791f40046280d3d360da93fe5eef6f0bb \
    gcc-2.7.2-psx:aca64479041aa2d645dc52ebcaace276c0aa06f258c0e3f190ccf6d76701ffbc
do
    name=${spec%%:*}
    sha=${spec#*:}
    archive="$downloads/$name.tar.gz"
    if [ ! -f "$archive" ]; then
        curl -L --fail --output "$archive" "$gcc_release/$name.tar.gz"
    fi
    printf '%s  %s\n' "$sha" "$archive" | sha256sum -c -
    mkdir -p "$dest/$name"
    tar -xzf "$archive" -C "$dest/$name"
done

if [ ! -f "$maspsx_archive" ]; then
    curl -L --fail --output "$maspsx_archive" "$maspsx_url"
fi
printf '%s  %s\n' "$maspsx_sha" "$maspsx_archive" | sha256sum -c -
unzip -q -o "$maspsx_archive" -d "$dest"

if ! command -v mipsel-linux-gnu-as >/dev/null 2>&1; then
    if [ ! -f "$binutils_archive" ]; then
        curl -L --fail --output "$binutils_archive" "$binutils_url"
    fi
    printf '%s  %s\n' "$binutils_sha" "$binutils_archive" | sha256sum -c -
    mkdir -p "$dest/mipsel-binutils/root"
    dpkg-deb -x "$binutils_archive" "$dest/mipsel-binutils/root"
fi

test -x "$dest/gcc-2.8.1-psx/cc1"
test -x "$dest/gcc-2.8.0-psx/cc1"
test -x "$dest/gcc-2.7.2-cdk/cc1"
test -x "$dest/gcc-2.7.2-psx/cc1"
test -f "$dest/maspsx-$maspsx_commit/maspsx.py"
test -x "$dest/mipsel-binutils/root/usr/bin/mipsel-linux-gnu-as" || \
    command -v mipsel-linux-gnu-as >/dev/null
printf 'toolchain ready in %s\n' "$dest"
