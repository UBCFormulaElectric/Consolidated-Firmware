#!/bin/sh
AUTOGEN_DEST="./proto_autogen"

# Cache pwd and chimera directory,
# so that we can step into chimera directory, and then step back.
CACHED_WORKING_DIR=$(pwd)
CHIMERA_V2_DIR="$(dirname "$0")/.."

cd $CHIMERA_V2_DIR
    # Clear old autogenned files.
    rm -rf ./proto_autogen/*

    # Clone nanopb.proto options so that Python has access.
    curl https://raw.githubusercontent.com/nanopb/nanopb/refs/heads/master/generator/proto/nanopb.proto \
        > ./proto/nanopb.proto

    # For each proto file,
    for file in ./proto/*.proto
    do
        # Generate proto python lib and mypy stubs.
        echo "Generating python proto lib for for $file"
        protoc \
            -I proto \
            --python_out $AUTOGEN_DEST \
            --mypy_out=$AUTOGEN_DEST \
            $file

        # Fix import paths.
        # See https://github.com/protocolbuffers/protobuf/issues/1491,
        # Also https://github.com/cpcloud/protoletariat.
        echo "Running protoletariat for $file to fix paths"
        protol \
            --in-place \
            --python-out $AUTOGEN_DEST \
            protoc \
            --proto-path=proto $file
    done
cd "$CACHED_WORKING_DIR"