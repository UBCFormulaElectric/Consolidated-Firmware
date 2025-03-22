#!/bin/sh
# Cache pwd and chimera directory,
# so that we can step into chimera directory, and then step back.
CACHED_WORKING_DIR=$(pwd)
CHIMERA_V2_DIR="$(dirname "$0")/.."

cd $CHIMERA_V2_DIR
    BRANCH="${1:-"master"}"
    GITHUB_SRC_URL="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/$BRANCH"
    echo "Generating Chimera V2 docs using $BRANCH branch for base url"

    # Reinstall this package.
    pip uninstall -y chimera_v2
    pip install .

    # Generate docs.
    lazydocs --validate --src-base-url="$GITHUB_SRC_URL" \
        chimera_v2 \
        load_bank \
        power_supply \
        scope

    # Uninstall this package.
    pip uninstall -y chimera_v2
    
cd $CACHED_WORKING_DIR