#!/bin/bash
# ----------------------------------------------------------------------------
# This script downloads and installs gcc-arm-none-eabi
# ----------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/../shared_funcs.sh

check_installation_path_argument $#

echo_border
echo "Start downloading and installing gcc-arm-none-eabi..."
echo_border

INSTALL_DIR=$1
BIN_DIR=$INSTALL_DIR/bin

if test -f $BIN_DIR/arm-none-eabi-gcc ; then
    echo "arm-none-eabi-gcc already installed in $BIN_DIR"
else
    # Unfortunately, the download link is a literal URL which means it does not
    # pull the latest version automatically.
    #
    # To change the download link:
    # 1. Go to https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
    # 2. Right-click and save the link to download the Linux 64-bit version
    # TODO: Revert to 2019-q3 once the link is back online
    # https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2?revision=c34d758a-be0c-476e-a2de-af8c6e16a8a2?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,8-2019-q3-update
    URL=https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
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

echo_border
echo "Downloading and installing gcc-arm-none-eabi completed successfully!"
echo_border
