const backendPort = process.env.NEXT_PUBLIC_BACKEND_PORT || "5000";
const API_BASE_URL = process.env.NEXT_PUBLIC_API_BASE_URL || (
    typeof window !== "undefined"
        ? `http://${window.location.hostname}:${backendPort}`
        : `http://localhost:${backendPort}`
);
const MAX_RECCONECTION_ATTEMPTS = +(process.env.NEXT_PUBLIC_MAX_RECCONNECTION_ATTEMPTS || 5);

const IS_DEBUG = process.env.NEXT_PUBLIC_IS_DEBUG === "true";
const IS_VERBOSE_DEBUG = IS_DEBUG && process.env.NEXT_PUBLIC_IS_VERBOSE_DEBUG === "true";

export { API_BASE_URL, IS_DEBUG, IS_VERBOSE_DEBUG, MAX_RECCONECTION_ATTEMPTS };

export const ENUM_COLORS = [
    "#FF3B2F",
    "#FFCC02",
    "#FF9500",
    "#35C759",
    "#007AFF",
    "#5856D6",
    "#AF52DE",
    "#FF2D55",
];
