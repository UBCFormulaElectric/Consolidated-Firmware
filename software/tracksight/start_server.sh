#!/bin/bash

echo "Starting backend..."
python backend/app/telemetry.py &

sleep 5

echo "Starting frontend server..."
cd frontend
npm run dev
