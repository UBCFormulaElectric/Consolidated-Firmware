import { API_BASE_URL } from "@/lib/constants";

type HistoricalSignalRow = {
    timestamp: string;
    value: number;
    name: string;
};

export type HistoricalSignalSource = "Radio" | "SdCard";

export type HistoricalSignalPoint = {
    timestampMs: number;
    value: number;
    name: string;
};

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function parseHistoricalPayload(payloadText: string): HistoricalSignalRow[] {
    const parsed = JSON.parse(payloadText) as unknown;
    if (!Array.isArray(parsed)) {
        console.warn("[historical signal] Expected an array payload from backend.", parsed);
        return [];
    }
    return parsed as HistoricalSignalRow[];
}

export type HistoricalSignalAggregation = "mean" | "max";

export async function fetchHistoricalSignal(params: { signalName: string; startUtcMs: number; endUtcMs: number; source: HistoricalSignalSource; agg?: HistoricalSignalAggregation }): Promise<HistoricalSignalPoint[]> {
    const { signalName, startUtcMs, endUtcMs, source, agg } = params;

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    // Alert/boolean signals must downsample with `max` so a window containing any
    // active sample resolves to 1 instead of being averaged away.
    const aggParam = agg ? `&agg=${encodeURIComponent(agg)}` : "";
    const url = `${API_BASE_URL}/api/v1/signal/tiles/${encodeURIComponent(signalName)}/${start}/${end}?source=${encodeURIComponent(JSON.stringify(source))}${aggParam}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical signal ${signalName}: ${response.status} ${errorText}`);
    }

    const payloadText = await response.text();
    const rows = parseHistoricalPayload(payloadText);

    return rows.map((row) => ({
        name: row.name,
        value: row.value,
        timestampMs: Date.parse(row.timestamp),
    }));
}
