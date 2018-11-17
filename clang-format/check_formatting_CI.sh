#!/bin/bash

# The current directory
CURR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Format all the files
if ! $CURR_DIR/fix_formatting_linux.sh; then
    # Formatting Failed
    echo "There was en error with formatting!!"
    exit 1
fi

# Check if anything changed
if git diff-index --quiet HEAD --; then
    # No changes
    printf 'Formatting check PASSED, great work!'
    exit 0
else
    # Changes
    CHANGES=$(git diff --color)
    echo "$CHANGES"
    printf 'Formatting check FAILED, see above for what changed!!'
    exit 1
fi

