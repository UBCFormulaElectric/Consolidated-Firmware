# Tracksight

## Backend
<!-- 
This is outdated, we've moved to Docker now. -Gus
The backend is brought up by running the pipenv shell after it is installed. Then enter /software/tracksight/backend/app and run python/telemetry.py 
-->

Tracksight can be run in one of two ways: Either it pulls data from a local CSV log file, or it receives data wirelessly from the car in real time. 
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

To start the local stack, navigate to `software/tracksight` and run `./run_local.sh`.

This starts the frontend, backend, and InfluxDB database. These are all available at [http://localhost:3000](http://localhost:3000), [http://localhost:5000](http://localhost:5000), and [http://localhost:8086](http://localhost:8086), respectively.

The data source for running locally is from CSV data files. These are essentially a time-series list of the signals sent on the CAN bus, with 
their timestamps, names, values, and units.

In these CSV files, the required columns are "time" (timestamp that the signal was sent), "signal" (signal name), "value", and "unit" (physical unit that the value is in, leave blank if not applicable).

### Running Wireless Telemetry

TODO