#!/bin/bash
# -----------------------------------------------------------------------------
# Initialize Git submodules in Travis CI. The content is taken from:
# https://gist.github.com/iedemam/9830045
# -----------------------------------------------------------------------------
# Use sed to replace the SSH URL with the public URL, then initialize
# submodules becuase Travis can't clone from public SSH URLs.
sed -i 's/git@github.com:/https:\/\/github.com\//' .gitmodules

# Pull the Git submodules
git submodule update --init --recursive

# Revert the .gitmodules so it doesn't show up in git diff
git checkout .gitmodules
