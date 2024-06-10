#!/bin/bash

show_help() {
cat << EOF
Usage: ${0##*/} [-h|--help] [-c|--clean] -f|--file FILE
Run telemetry in log mode, sourcing data from a CSV log file.

    -h|--help       Display this help and exit
    -c|--clean      Clear all existing data in the local Influx database
    -f|--file FILE   Specify the CSV log file to upload to the local Influx database
EOF
}

# Initialize our own variables.
clean=0
file=""

# Process flags and arguments.
while :; do
    if [ -z "$1" ]; then
        break  # Exit the loop if no more arguments are present
    fi

    case "$1" in
        -h|--help)
            show_help    # Display a help message
            exit 0
            ;;
        -c|--clean)
            clean=1      # Set a flag to run a clean command later
            shift
            ;;
        -f|--file)
            if [ -n "$2" ]; then
                file="$2"   # Assign the next parameter as the file path
                shift 2
            else
                echo "Error: --file requires a file argument." >&2
                exit 1
            fi
            ;;
        *)
            echo "Error: Unknown option: $1" >&2
            show_help    # Display a help message
            exit 1
            ;;
    esac
done

script_dir="$(dirname "$(readlink -f "$0")")"

# Handle the clean command.
if [ "$clean" -eq 1 ]; then
    echo "Cleaning database."
    docker compose -f "$script_dir/docker/log.yml" down -v
fi

# Check if the file path is provided
if [ -z "$file" ]; then
    echo "Warning: No file specified. Use -f or --file to specify the CSV file."
fi

# Pass the file argument to the Python script.
echo "Starting telemetry in log  mode."
env DATA_FILE="$file" docker compose -f "$script_dir/docker/log.yml" up --build
