"use client";

export type TelemetryMarker = {
    timestampMs: number;
};

const STORAGE_KEY = "tracksight_telem_button_markers_v1";
const MAX_MARKERS = 500;

let markerCache: TelemetryMarker[] | null = null;
let remoteMarkerCache: TelemetryMarker[] = [];
let combinedMarkerCache: TelemetryMarker[] | null = null;

function normalizeMarkers(markers: TelemetryMarker[]): TelemetryMarker[] {
    const timestamps = new Set<number>();

    markers.forEach((marker) => {
        if (marker && typeof marker.timestampMs === "number") {
            timestamps.add(marker.timestampMs);
        }
    });

    return Array.from(timestamps)
        .sort((left, right) => left - right)
        .slice(-MAX_MARKERS)
        .map((timestampMs) => ({ timestampMs }));
}

function canUseStorage() {
    return typeof window !== "undefined" && typeof window.localStorage !== "undefined";
}

function readMarkersFromStorage(): TelemetryMarker[] {
    if (!canUseStorage()) return [];

    const raw = window.localStorage.getItem(STORAGE_KEY);
    if (!raw) return [];

    try {
        const parsed = JSON.parse(raw);
        if (!Array.isArray(parsed)) return [];

        return parsed
            .filter((marker): marker is TelemetryMarker => marker !== null && typeof marker === "object" && typeof marker.timestampMs === "number")
            .sort((left, right) => left.timestampMs - right.timestampMs)
            .slice(-MAX_MARKERS);
    } catch {
        return [];
    }
}

function writeMarkersToStorage(markers: TelemetryMarker[]) {
    markerCache = normalizeMarkers(markers);
    combinedMarkerCache = null;
    if (!canUseStorage()) return;

    window.localStorage.setItem(STORAGE_KEY, JSON.stringify(markerCache));
}

export function getTelemetryMarkers(): TelemetryMarker[] {
    if (markerCache === null) {
        markerCache = readMarkersFromStorage();
    }

    return markerCache;
}

export function getVisibleTelemetryMarkers(startTimeMs: number, endTimeMs: number): TelemetryMarker[] {
    combinedMarkerCache ??= normalizeMarkers([...getTelemetryMarkers(), ...remoteMarkerCache]);
    return combinedMarkerCache.filter((marker) => marker.timestampMs >= startTimeMs && marker.timestampMs <= endTimeMs);
}

export function addTelemetryMarker(marker: TelemetryMarker) {
    const nextMarker: TelemetryMarker = {
        ...marker,
    };

    writeMarkersToStorage([...getTelemetryMarkers(), nextMarker].sort((left, right) => left.timestampMs - right.timestampMs));
    return nextMarker;
}

export function setRemoteTelemetryMarkers(markers: TelemetryMarker[]) {
    remoteMarkerCache = normalizeMarkers(markers);
    combinedMarkerCache = null;
}

export function clearRemoteTelemetryMarkers() {
    remoteMarkerCache = [];
    combinedMarkerCache = null;
}
