#!/bin/bash

show_help() {
cat << EOF
Usage: ${0##*/} [-h|--help] [-c|--clean] FILE
Run telemetry locally, sourcing data from a CSV log file.

    -h|--help       Display this help and exit
    -c|--clean      Clear all existing data in the local Influx database
    FILE            CSV log file to upload to the local Influx database
EOF
}

# Initialize our own variables.
clean=0

# Process flags and arguments.
while :; do
    case "$1" in
        -h|--help)
            show_help    # Display a help message
            exit 0
            ;;
        -c|--clean)
            clean=1      # Set a flag to run a clean command later
            shift
            ;;
        --)             # End of all options
            shift
            break
            ;;
        -*)
            echo "Error: Unknown option: $1" >&2
            show_help    # Display a help message
            exit 1
            ;;
        *)              # No more options, break out of the loop
            break
            ;;
    esac
done

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

# Handle the clean command.
if [ "$clean" -eq 1 ]; then
    echo "Cleaning database."
    docker compose -f "$SCRIPT_DIR/docker/local.yml" down -v
fi

# Pass the positional argument to the Python script.
echo "Starting telemetry locally."
env DATA_FILE="$1" docker compose -f "$SCRIPT_DIR/docker/local.yml" up --build