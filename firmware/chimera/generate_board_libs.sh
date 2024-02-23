#!/bin/bash

for file in board_protos/*.proto
do
    protoc -I board_protos --python_out board_libs board_protos/shared.proto

    protol \
    --in-place \
    --python-out board_libs \
    protoc --proto-path=board_protos board_protos/shared.proto
done

