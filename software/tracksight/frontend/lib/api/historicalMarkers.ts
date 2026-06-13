import { API_BASE_URL, IS_MOCK } from "@/lib/constants";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";

import { TelemetryMarker } from "@/lib/telemetryMarkers";

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function parseMarkerPayload(payloadText: string): number[] {
    const parsed = JSON.parse(payloadText) as unknown;
    if (!Array.isArray(parsed)) {
        console.warn("[historical markers] Expected an array payload from backend.", parsed);
        return [];
    }

    return parsed
        .filter((entry): entry is number => typeof entry === "number")
        .sort((left, right) => left - right);
}

export async function fetchHistoricalMarkers(startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource): Promise<TelemetryMarker[]> {
    if (IS_MOCK) return [];

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    const url = `${API_BASE_URL}/api/v1/signal/markers/${start}/${end}?source=${encodeURIComponent(JSON.stringify(source))}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical markers: ${response.status} ${errorText}`);
    }

    return parseMarkerPayload(await response.text()).map((timestampMs) => ({ timestampMs }));
}
