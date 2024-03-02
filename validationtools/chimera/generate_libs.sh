#!/bin/bash

for file in proto/*.proto
do
    protoc -I proto --python_out chimera/proto_libs $file

    protol \
    --in-place \
    --python-out chimera/proto_libs \
    protoc --proto-path=proto $file
done

