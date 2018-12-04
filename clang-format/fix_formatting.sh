#!/bin/bash

# For linux the clang-format executable comes from downloading the 
# LLVM package for the newest version of Ubuntu (under "Pre-Built Binaries"
# at "www.releases.llvm.org/download.html), extracting the file, and copying 
# the "clang-format" executable here (then renaming it to "clang-format-VERSION_NUMBER")

# The version of the clang executable to use
export CLANG_VERSION=4.0

# Figure out what directory this script is in 
CURR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# The command we use to format a single file
FORMAT_CMD="$CURR_DIR/clang-format-$CLANG_VERSION -i --style=file"

# Format all the files
find $CURR_DIR/../ \
     \( -name '*.c' \
     -o -name '*.cc' \
     -o -name '*.cpp' \
     -o -name '*.h' \
     -o -name '*.hh' \
     -o -name '*.hpp' \) -print0 |\
     xargs -0 -n1 ${FORMAT_CMD}

