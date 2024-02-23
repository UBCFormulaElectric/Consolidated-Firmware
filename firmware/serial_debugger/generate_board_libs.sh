#!/bin/bash

    for file in board_protos/*.proto
    do
        protoc -I=board_protos --python_out=board_libs $file
    done
