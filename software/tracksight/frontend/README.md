# Tracksight Frontend

This is a [Next.js](https://nextjs.org/) project bootstrapped with [`create-next-app`](https://github.com/vercel/next.js/tree/canary/packages/create-next-app).

## Getting Started (running the dev server)

1. Install Node.js > v18.8.0. `sudo apt install node` on Linux will install an outdated version so you need to manually seek it(should probably use `nvm`).
2. Install packages with `npm install` inside this directory
3. Run the development server: `npm run dev`

Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.

## Deployment Server

Please refer to the `frontend` service of `sofware/tracksight/docker-compose.yml` for notes on how to deploy the frontend server.

## TODOs

- Move shared zoom data to a shared context
- Migrate all fetch calls to [Tanstack Query](https://tanstack.com/query/latest)
- Move from src to root directory
- Better chart library
- When auto fetch times when zooming
