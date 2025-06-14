// Signal type classification
export enum SignalType {
  Numerical = "numerical",
  Enumeration = "enumeration",
  Any = "any", // For signals that could be either
  Warning = "Warning",
  WarningCount = "WarningCount",
}

// Signal metadata interface
export interface SignalMeta {
  name: string;
  unit: string;
  cycle_time_ms?: number;
  msg_id: number;
  msg_name: string;
}

// Data point interface
export interface DataPoint {
  time: number | string;
  name?: string;
  value?: number | string;
  [signalName: string]: number | string | undefined;
}

// Default maximum number of data points to keep
export const DEFAULT_MAX_DATA_POINTS = 1000;
export const BACKEND_URL =
  typeof window !== "undefined"
    ? `http://${window.location.hostname}:5000`
    : `http://localhost:5000`;
export const MAX_RECONNECT_ATTEMPTS = 5;
export const RECONNECT_INTERVAL = 3000; // 3 seconds
export const DEBUG = false; // Set to false to disable debug logs
