#!/bin/bash

rootDir=$(realpath "$(dirname "$BASH_SOURCE")/../../../../")

# Iterate over all .proto files in the specified directory
find "$rootDir/firmware/quadruna/VC/proto" -name "*.proto" | while read -r file; do
    protoc \
        -I "$rootDir/firmware/quadruna/VC/proto" \
        --python_out="$rootDir/software/tracksight/backend/util_scripts" "$file"

    protol \
        --in-place \
        --python-out "$rootDir/software/tracksight/backend/util_scripts" \
        protoc --proto-path="$rootDir/firmware/quadruna/VC/proto" $file
done