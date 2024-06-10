#!/bin/bash

show_help() {
cat << EOF
Usage: ${0##*/} [-h|--help] [-c|--clean] -p|--serial-port PORT
Run telemetry in wireless mode

    -h|--help               Display this help and exit
    -c|--clean              Clear all existing data in the local Influx database
    -p|--serial-port PORT   The serial port of the radio receiver
EOF
}

# Initialize our own variables.
clean=0
port=""

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
        -p|--serial-port)
            if [ -n "$2" ]; then
                port="$2"   # Assign the next parameter as the port
                shift 2
            else
                echo "Error: --port requires a port argument." >&2
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
    docker compose -f "$script_dir/docker/wireless.yml" down -v
fi

# Check if the port path is provided
if [ -z "$port" ]; then
    echo "Error: No port specified. Use -p or --serial-port to specify the radio receiver's serial port."
    exit 1
fi

echo "Starting telemetry in wireless mode."

# Pass the port argument to the Python script.
docker compose -f "$script_dir/docker/wireless.yml" up --build & 

# Wait until pinging the database succeeds until we start the 
influxdb_url="http://localhost:8086"
timeout=20

start_time=$(date +%s)  # Record start time
while true; do
    # Ping the host with a timeout of 1 second and check if it's successful
    if curl --connect-timeout 1 --silent --head $influxdb_url > /dev/null 2>&1; then
        echo "Database started successfully."
        break
    else
        current_time=$(date +%s)
        elapsed_time=$((current_time - start_time))
        
        # Check if the total elapsed time is greater than 20s.
        if [ $elapsed_time -ge $timeout ]; then
            echo "Timeout reached without a successful connection."
            exit 1
        fi

        sleep 1
    fi
done

echo "Starting backend server."
python3 "$script_dir/backend/app/telemetry.py" --mode wireless --serial-port $port --debug 
