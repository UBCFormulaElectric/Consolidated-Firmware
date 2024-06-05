# Tracksight

## Backend
<!-- 
This is outdated, we've moved to Docker now. -Gus
The backend is brought up by running the pipenv shell after it is installed. Then enter /software/tracksight/backend/app and run python/telemetry.py 
-->

Tracksight can be run in one of two ways: Either it pulls data from a local CSV data file, or it receives data wirelessly from the car in real time. 
The backend is a Flask app that pipes data to the frontend in either case.

## Frontend

This is a [Next.js](https://nextjs.org/) project bootstrapped with [`create-next-app`](https://github.com/vercel/next.js/tree/canary/packages/create-next-app).

### Getting Started

1. Install Node.js > v18.8.0. `sudo apt install node` on Linux will install an outdated version so you need to manually seek it(should probably use `nvm`).
2. Install packages with `npm install` inside this directory
3. Run the development server: `npm run dev`

Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.

You can start editing the page by modifying `app/page.tsx`. The page auto-updates as you edit the file.

This project uses [`next/font`](https://nextjs.org/docs/basic-features/font-optimization) to automatically optimize and load Inter, a custom Google Font.

### Learn More

To learn more about Next.js, take a look at the following resources:

- [Next.js Documentation](https://nextjs.org/docs) - learn about Next.js features and API.
- [Learn Next.js](https://nextjs.org/learn) - an interactive Next.js tutorial.

You can check out [the Next.js GitHub repository](https://github.com/vercel/next.js/) - your feedback and contributions are welcome!

<!-- 
I think this is outdated but I didn't want to delete. -Gus

### Deploy on Vercel

The easiest way to deploy your Next.js app is to use the [Vercel Platform](https://vercel.com/new?utm_medium=default-template&filter=next.js&utm_source=create-next-app&utm_campaign=create-next-app-readme) from the creators of Next.js.

Check out our [Next.js deployment documentation](https://nextjs.org/docs/deployment) for more details. -->

## Running with Docker

### Running Locally

To start the local stack, navigate to `software/tracksight` and run `./run_log.sh`. This is used for viewing log files, such as from the onboard SD card.

This starts the frontend, backend, and InfluxDB database. These are all available at [http://localhost:3000](http://localhost:3000), [http://localhost:5000](http://localhost:5000), and [http://localhost:8086](http://localhost:8086), respectively.

The data source for running locally is from CSV data files. These are essentially a time-series list of the signals sent on the CAN bus, with 
their timestamps, names, values, and units. In these CSV files, the required columns are "time" (timestamp that the signal was sent), "signal" (signal name), "value", and "unit" (physical unit that the value is in, leave blank if not applicable).

To specify a data file to read, pass it with the `--file` or `-f` flag to `run_log.sh`. You can upload multiple by providing a comma-seperated list instead. This will upload all of the data in your provided files to the local InfluxDB database. IMPORTANT: Your log file must be in the `software/tracksight/backend/data` directory. If it is not, it will not be uploaded. Provide the path relative to this folder. (Yes I know this is pretty silly, but comp is in 7 days)

Note that if you stop the compose stack, and restart it again, your data will remain since the data is stored on a Docker volume which isn't wiped if the compose stack is brought down. This is useful if you want to upload more data files, and keep your old ones. To clear the local database, pass the `-c` or `--clean` flag to `run_log.sh`.

Example:

```
# sine_wave_data.csv is located in software/tracksight/backend/data
./run_local.sh sine_wave_data.csv
```

### Running Wireless Telemetry

To start everything manually, do the following steps: 

1. Start Docker.
2. Start the frontend and InfluxDB database with docker. Navigate to `Consolidated-Firmware/software/tracksight` and run `docker compose -f docker/wireless.yml up --build`.
3. Start the backend. Navigate to `Consolidated-Firmware/software/tracksight/backend/app`  and run `python3 telemetry.py --debug --mode wireless --serial-port /dev/ttyUSB0`. This will start the backend which (currently) uploads some tester signal values in signal_util.py. Note that you need to do step 1 before this, as well as pass through the radio receiver serial port (usually `/dev/ttyUSB0` on Linux, change if running on another OS).

To start everything automatically, navigate to `software/tracksight` and run `./run_wireless.sh`. Pass the serial port to this script using the `--serial-port` or `-p` flag. Note that this method is easier, but less flexibile than starting everything manually. With the backend running in its own terminal, its easier to start/stop it without re-starting the frontend/database Docker compose stack.

Note that if you stop the compose stack, and restart it again, your data will remain since the data is stored on a Docker volume which isn't wiped if the compose stack is brought down. To clear the local database, pass the `-c` or `--clean` flag to `run_wireless.sh`.
