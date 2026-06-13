import { API_BASE_URL } from "@/lib/constants";
import { SignalType } from "../types/Signal";

type HistoricalSignalRow = {
    timestamp: string;
    value: number;
    name: string;
};

type SignalTilesResponse = {
    resolution_ms: number;
    rows: HistoricalSignalRow[];
};

export type HistoricalSignalSource = "Radio" | "SdCard";

export type HistoricalSignalPoint = {
    timestampMs: number;
    value: number;
    name: string;
};

export type HistoricalSignalResult = {
    resolutionMs: number;
    points: HistoricalSignalPoint[];
};

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function parseHistoricalPayload(payloadText: string): SignalTilesResponse {
    const parsed = JSON.parse(payloadText) as unknown;
    if (parsed === null || typeof parsed !== "object" || !Array.isArray((parsed as SignalTilesResponse).rows)) {
        console.warn("[historical signal] Expected a { resolution_ms, rows } payload from backend.", parsed);
        return { resolution_ms: 0, rows: [] };
    }
    return parsed as SignalTilesResponse;
}

export async function fetchHistoricalSignal(params: { signalName: string; signalType: SignalType; startUtcMs: number; endUtcMs: number; source: HistoricalSignalSource }): Promise<HistoricalSignalResult> {
    const { signalName, signalType, startUtcMs, endUtcMs, source } = params;

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    const agg = signalType === SignalType.NUMERICAL ? "mean" : "first";
    const url = `${API_BASE_URL}/api/v1/signal/tiles/${encodeURIComponent(signalName)}/${start}/${end}?agg=${agg}&source=${encodeURIComponent(JSON.stringify(source))}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical signal ${signalName}: ${response.status} ${errorText}`);
    }

    const payloadText = await response.text();
    const { resolution_ms, rows } = parseHistoricalPayload(payloadText);

    return {
        resolutionMs: resolution_ms,
        points: rows.map((row) => ({
            name: row.name,
            value: row.value,
            timestampMs: Date.parse(row.timestamp),
        })),
    };
}
