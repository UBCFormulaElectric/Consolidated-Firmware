#!/bin/bash

rootDir=$(realpath "$(dirname "$BASH_SOURCE")/../../../")

# Create the directory, ignoring the error if it already exists
mkdir -p "$rootDir/validationtools/validationtools/chimera/proto_libs" 2> /dev/null

# Iterate over all .proto files in the specified directory
find "$rootDir/firmware/chimera/proto" -name "*.proto" | while read -r file; do
    protoc \
        -I "$rootDir/firmware/chimera/proto" \
        --python_out="$rootDir/validationtools/validationtools/chimera/proto_libs" "$file"

    protol \
        --in-place \
        --python-out "$rootDir/validationtools/validationtools/chimera/proto_libs" \
        protoc --proto-path="$rootDir/firmware/chimera/proto" $file
done