#!/bin/bash
# ----------------------------------------------------------------------------
# This script downloads and installs CMake
# ----------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/../shared_funcs.sh

check_installation_path_argument $#

echo_border
echo "Start downloading and installing CMake..."
echo_border

# Modify these variables to update the CMake version
VERSION_MAJOR=3
VERSION_MINOR=15
VERSION_MICRO=2
SHA256=f8cbec2abc433938bd9378b129d1d288bb33b8b5a277afe19644683af6e32a59

VERSION=$VERSION_MAJOR.$VERSION_MINOR.$VERSION_MICRO
INSTALL_DIR=$1
BIN_DIR=$INSTALL_DIR/bin

if test -f $BIN_DIR/cmake && ($BIN_DIR/cmake --version | grep -q "$VERSION"); then
    echo "CMake $VERSION already installed in $BIN_DIR"
else
    FILE=cmake-$VERSION-Linux-x86_64.tar.gz
    URL=https://cmake.org/files/v$VERSION_MAJOR.$VERSION_MINOR/$FILE
    TMPFILE=$(mktemp --tmpdir cmake-$VERSION-Linux-x86_64.XXXXXXXX.tar.gz)
    # Download CMake
    echo "Downloading CMake ($URL)..."
    wget "$URL" -O "$TMPFILE"
    # Check if downloaded file is correupt
    if ! (shasum -a256 "$TMPFILE" | grep -q "$SHA256"); then
        echo "Checksum mismatch ($TMPFILE)"
        exit 1
    fi
    # Create the installation directory
    mkdir -p "$INSTALL_DIR"
    # Unzip CMake to the installation directory
    tar xzf "$TMPFILE" -C "$INSTALL_DIR" --strip 1
    # Remove temporary installation file
    rm $TMPFILE
fi

echo_border
echo "Downloading and installing CMake completed successfully!"
echo_border
