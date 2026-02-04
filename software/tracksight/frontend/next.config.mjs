import { fileURLToPath } from 'url';
import path from 'path';
import fs from 'fs';
import process from 'process';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

// this is a frustratingly uneccesary way of doing this. we shoudl really rewrite this.
// i needed to load in the port from the backend env file into the frontend env vars
function loadBackendEnv() {
  try {
    const envPath = path.resolve(__dirname, '../backend/local_backend.env');
    if (!fs.existsSync(envPath)) return;
    const content = fs.readFileSync(envPath, 'utf8');
    content.split('\n').forEach((line) => {
      const trimmed = line.trim();
      if (!trimmed || trimmed.startsWith('#')) return;
      const match = trimmed.match(/^([A-Z0-9_]+)\s*=\s*(.*)$/);
      if (!match) return;
      const [, key, rawVal] = match;
      // strip surrounding quotes if present
      const val = rawVal.replace(/^"|"$/g, '').replace(/^'|'$/g, '');
      if (!(key in process.env)) {
        process.env[key] = val;
      }
    });
  } catch (e) {
    console.warn('Warning: failed to load backend env:', e);
  }
}

loadBackendEnv();

/** @type {import('next').NextConfig} */
const nextConfig = {
  eslint: {
    ignoreDuringBuilds: true,
  },
  typescript: {
    ignoreBuildErrors: true,
  },
  images: {
    unoptimized: true,
  },
  env: {
    NEXT_PUBLIC_BACKEND_PORT: process.env.BACKEND_PORT || '5000',
  },
};

export default nextConfig;
