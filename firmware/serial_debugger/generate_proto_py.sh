#!/bin/bash

echo "Generating Python File"
protoc --python_out=. debug_modules.proto
echo "Done"
