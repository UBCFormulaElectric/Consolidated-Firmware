#!/bin/bash
# ----------------------------------------------------------------------------
# This script downloads and installs gcc-arm-none-eabi
# ----------------------------------------------------------------------------

echo "Start downloading and installing gcc-arm-none-eabi..."

INSTALL_DIR=$1
BIN_DIR=$INSTALL_DIR/bin

if test -f $BIN_DIR/arm-none-eabi-gcc ; then
    echo "arm-none-eabi-gcc already installed in $BIN_DIR"
else
    # Unfortunately, the download link is a literal URL which means it does not pull the latest version automatically.
    #
    # To change the download link:
    # 1. Go to https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
    # 2. Right-click and save the link to download the Linux 64-bit version
    URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2?rev=78196d3461ba4c9089a67b5f33edf82a&hash=D484B37FF37D6FC3597EBE2877FB666A41D5253B"
    TMPFILE=$(mktemp --tmpdir gcc-arm-none-eabi.XXXXXXXX.tar.bz2)
    
    # Download gcc-arm-none-eabi
    echo "Downloading gcc-arm-none-eabi($URL)..."
    wget "$URL" -O "$TMPFILE"
    
    # Create the install directory
    mkdir -p "$INSTALL_DIR"
    
    # Unzip gcc-arm-none-eabi to the install directory
    tar -xjf "$TMPFILE" -C "$INSTALL_DIR" --strip 1
    
    # Remove the temporary installation file
    rm $TMPFILE
fi

echo "Downloading and installing gcc-arm-none-eabi completed successfully!"