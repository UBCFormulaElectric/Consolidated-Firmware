#!/bin/sh
AUTOGEN_DEST="./proto_autogen"

# Cache pwd and chimera directory,
# so that we can step into chimera directory, and then step back.
CACHED_WORKING_DIR=$(pwd)
CHIMERA_V2_DIR="$(dirname "$0")/.."

cd $CHIMERA_V2_DIR
    lazydocs .
cd $CACHED_WORKING_DIR