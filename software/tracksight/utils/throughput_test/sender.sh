#!/usr/bin/env bash

script_dir="$(dirname "$(readlink -f "$0")")"
backend_dir="$(readlink -f "$script_dir/../../rust_backend")"

if [ "$#" -ne 1 ]; then
    echo "Specify serial port"
    exit 1
fi

echo "Building and running throughput sender on port $1"

echo "$backend_dir/Cargo.toml"

cargo build --manifest-path "$backend_dir/Cargo.toml" --bin throughput
cargo run --manifest-path "$backend_dir/Cargo.toml" --bin throughput -- --sender --port $1 --baud 230400 --cts --hold-space