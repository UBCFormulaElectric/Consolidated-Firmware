#!/bin/bash

# Save the parent dir of this so we can always run commands relative to the
# location of this script, no matter where it is called from. This
# helps prevent bugs and odd behaviour if this script is run through a symlink
# or from a different directory.
CURR_DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

# Config Variables
export INSTALL_DIR="/usr/local"
export CUBE_INSTALL_DIR="$INSTALL_DIR/STM32CubeMX"

# Python Setup
pip3 install pipenv
pipenv install

# Git 
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
sudo apt-get install git-lfs
git lfs install
git lfs pull
$CURR_DIR/scripts/travis_ci/pull_git_submodules.sh

# STM32CubeMX Installation
$CURR_DIR/scripts/travis_ci/travis_install_binaries.sh $INSTALL_DIR $CUBE_INSTALL_DIR
