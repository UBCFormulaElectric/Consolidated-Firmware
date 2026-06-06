"use client";

export type TelemetryMarker = {
    id: string;
    timestampMs: number;
    messageName: string;
    source: string;
    canId?: string;
    value?: string | number;
    details?: Record<string, string | number | boolean | null>;
};

const STORAGE_KEY = "tracksight_telem_button_markers_v1";
const MAX_MARKERS = 500;

let markerCache: TelemetryMarker[] | null = null;

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
            .filter((marker): marker is TelemetryMarker => marker !== null && typeof marker === "object" && typeof marker.id === "string" && typeof marker.timestampMs === "number" && typeof marker.messageName === "string" && typeof marker.source === "string") // someone kill typescript
            .sort((left, right) => left.timestampMs - right.timestampMs)
            .slice(-MAX_MARKERS);
    } catch {
        return [];
    }
}

function writeMarkersToStorage(markers: TelemetryMarker[]) {
    markerCache = markers.slice(-MAX_MARKERS);
    if (!canUseStorage()) return;

    window.localStorage.setItem(STORAGE_KEY, JSON.stringify(markerCache));
}

export function getTelemetryMarkers(): TelemetryMarker[] {
    if (markerCache === null) {
        markerCache = readMarkersFromStorage();
    }

    return markerCache;
}

export function addTelemetryMarker(marker: Omit<TelemetryMarker, "id"> & { id?: string }) {
    const nextMarker: TelemetryMarker = {
        ...marker,
        id: marker.id ?? `${marker.messageName}-${marker.timestampMs}-${Math.random().toString(36).slice(2)}`,
    };

    writeMarkersToStorage([...getTelemetryMarkers(), nextMarker].sort((left, right) => left.timestampMs - right.timestampMs));
    return nextMarker;
}
