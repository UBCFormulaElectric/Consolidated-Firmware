#!/bin/sh
# Cache pwd and chimera directory,
# so that we can step into chimera directory, and then step back.
CACHED_WORKING_DIR=$(pwd)
CHIMERA_V2_DIR="$(dirname "$0")/.."

cd $CHIMERA_V2_DIR
    mypy --install-types
    mypy ./chimera_v2.py ./load_bank.py ./power_supply.py ./scope.py
    ruff format .
cd $CACHED_WORKING_DIR