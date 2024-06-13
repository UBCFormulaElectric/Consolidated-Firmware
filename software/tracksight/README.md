# Tracksight

## Overview

Tracksight is used for inspecting CAN bus data from the car, and works in two modes: Either it pulls data from a local CSV data file, or it receives data wirelessly from the car in real-time. 

This consists of 3 parts (each typically run inside of its own Docker container):
1. The InfluxDB database. This is responsible for storing time-series data representing our CAN bus messages. Defaults to [http://localhost:3000](http://localhost:3000).
2. The backend. The backend is a Python Flask app that provides data for the frontend, and is responsible for reading/writing data to the database. In log mode, the data is read from CSV log files, and in wireless telemetry, it receives data via from the car via a serial radio. Defaults to [http://localhost:5000](http://localhost:5000).
3. The frontend. This is a [Next.js](https://nextjs.org/) project which uses  for plotting CAN data. Defaults to [http://localhost:8086](http://localhost:8086).

## Frontend

This is a [Next.js](https://nextjs.org/) project bootstrapped with [`create-next-app`](https://github.com/vercel/next.js/tree/canary/packages/create-next-app).

### Getting Started

1. Install Node.js > v18.8.0. `sudo apt install node` on Linux will install an outdated version so you need to manually seek it (should probably use `nvm`).
2. We are managing packages with [yarn](https://classic.yarnpkg.com/en/). Run `npm install --global yarn` to install it.
2. Install packages with `yarn install` inside this directory.
3. Run the development server: `yarn run dev`.

Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.

You can start editing the page by modifying `app/page.tsx`. The page auto-updates as you edit the file.

This project uses [`next/font`](https://nextjs.org/docs/basic-features/font-optimization) to automatically optimize and load Inter, a custom Google Font.

### Learn More

To learn more about Next.js, take a look at the following resources:

- [Next.js Documentation](https://nextjs.org/docs) - learn about Next.js features and API.
- [Learn Next.js](https://nextjs.org/learn) - an interactive Next.js tutorial.

You can check out [the Next.js GitHub repository](https://github.com/vercel/next.js/) - your feedback and contributions are welcome!

## Running with Docker

Make sure you `yarn install` all packages for the frontend, and start Docker, before continuing. 

### Log Mode

This is the mode for using Tracksight to inspect CSV log files, parsed off of the vehicles SD card. These files are essentially a time-series list of the signals sent on the CAN bus, with 
their timestamps, names, values, and units.

To start the log mode container stack, navigate to `software/tracksight` and run:

```sh
./run_log.sh
```

To specify a data file to read, pass it with the `--file` or `-f` flag to `run_log.sh`. You can upload multiple by providing a comma-seperated list instead. This will upload all of the data in your provided files to the database. 

**Important**: Your log file must be in the `software/tracksight/backend/data` directory. If it is not, it will not be uploaded. Provide the path relative to this folder. (Yes I know this is pretty silly, but comp is in 7 days!)

Note that if you stop the compose stack, and restart it again, your data will remain since the data is stored on a Docker volume which isn't wiped if the compose stack is brought down. This is useful if you want to upload more data files, and keep your old ones. To delete the volume and clear the database, pass the `-c` or `--clean` flag to `run_log.sh`.

Example passing in a log file and cleaning:

```
# sine_wave_data.csv is located in software/tracksight/backend/data
./run_log.sh --clean --file sine_wave_data.csv
```

### Wireless Mode

This is the mode for using Tracksight to stream CAN bus data wireless from the car. To start everything manually:

1. Start the frontend and InfluxDB database with docker. Navigate to `Consolidated-Firmware/software/tracksight` and run `docker compose -f docker/wireless.yml up --build`.
2. Start the backend. Navigate to `Consolidated-Firmware/software/tracksight/backend/app`  and run `python3 telemetry.py --debug --mode wireless --serial-port /dev/ttyUSB0`. This will start the backend which (currently) uploads some tester signal values in signal_util.py. Note that you need to do step 1 before this, as well as pass the correct radio serial port (usually `/dev/ttyUSB0` on Linux, change if running on another OS).

To start everything automatically, navigate to `software/tracksight` and run `./run_wireless.sh`. Pass the serial port to this script using the `--serial-port` or `-p` flag. Note that this method is easier, but less flexibile than starting everything manually. With the backend running in its own terminal, its easier to start/stop it without re-starting the frontend/database Docker compose stack.

Note that if you stop the compose stack, and restart it again, your data will remain since the data is stored on a Docker volume which isn't wiped if the compose stack is brought down. To clear the local database, pass the `-c` or `--clean` flag to `run_wireless.sh`.

## Log Files

### Decoding SD Card Logs to CSV Files

All CAN messages sent or received by the VC are logged to the onboard SD card. These files just consist of the raw bytes of `CanMsgLog` structs defined in [firmware/shared/src/io/io_canLogging.h](../../firmware/shared/src/io/io_canLogging.h), written to disk using `logfs`. Each file is named like `/12.txt`, with the number corresponding to the number of times the car has been power-cycled. Unfortunately this is as precise as we can be on Quadruna, since the VC has no way to measure the current time.

A script is provided to parse these raw CAN frames, and decode them with JSONCAN. See [backend/scripts/decode_raw_log.py](backend/scripts/decode_raw_log.py). The output is CSV files, with each row corresponding to a signal transmitted on the bus. These CSV files can be used when running Tracksight in log mode (see above).

Example:

```sh
sudo python3 decode_raw_log.py --disk /dev/disk4 --time "2024-06-07T00:00" --name "example-driving-sesh"
```

The "time" flag provides the approximate time of the start of the log, "name" is used for a descriptive title to prefix log, and "disk" provides the path to the SD card. Run `python3 decode_raw_log.py -h` for more information. Note that (at least on Mac) `sudo` is required for accessing disks directly.

**Important:** Upon plugging in the SD card, you may see a prompt that the SD card cannot be read, and must be formatted. Do not format! This will wipe all data from the SD card. This just pops up because we are using a custom filesystem, and should be ignored.

### Converting CSV Files to MF4 Files

The outputted CSV files are useful since they work well with our InfuxDB database. However, another useful file format for CAN data is MDF or MF4 files. These are the standard for CAN, and so are supported by other programs. For example, asammdf GUI is a great program for inspecting MF4 files, which is much faster, and with more features, than our own frontend.

A script is also provided to convert from the CSV format mentioned earlier to the MF4 format for use with asammdf GUI. 

Example:

```sh
sudo python3 csv_to_mf4.py
```

This converts all CSV files in `software/tracksight/backend/data` to MF4 files in `software/tracksight/backend/data_mf4`. However, you can also provide your own input/output directories. Run `python3 csv_to_mf4.py -h` for more information. Note that you probably also have to `sudo` this command because its operating on CSV files produced by a sudo-enabled Python script.

#### More Information

1. [More information on MF4 files](https://www.csselectronics.com/pages/mf4-mdf4-measurement-data-format) from CSS Electronics.
2. Information on how to install [asammdf GUI](https://www.csselectronics.com/pages/asammdf-gui-api-mdf4-mf4).