#!/bin/bash
# ------------------------------------------------------------------------------
# This script contains shared functions for Travis related scripts.
# ------------------------------------------------------------------------------
# Any command in .travis.yml that exits with a non-zero value will cause Travis
# to exit. However, commands encapsulated within the top-level command may not
# report their return codes. For example, if we run './my_script.sh' in
# .travis.yml, and in that script.sh there is a CMake command. If this CMake
# command throws an error, this information is lost unless my_script.sh
# explicitly propogate the error code to its parent. Using `set -e` ensures
# that any command with non-zero command will immediately return and abort
# Travis.
set -e

# This allows us to 'fold' commands in Travis console
TRAVIS_FOLD_COUNTER=0
function travis_run() {
  local command=$@

  echo -e "\e[0Ktravis_fold:start:$(basename $0).command.$TRAVIS_FOLD_COUNTER \e[34m$ $command\e[0m"
  # Actually run command, and kill the build if there's a non-zero return code.
  eval ${command} || exit 1
  echo -e -n "\e[0Ktravis_fold:end:$(basename $0).command.$TRAVIS_FOLD_COUNTER\e[0m"

  let "TRAVIS_FOLD_COUNTER += 1"
}
