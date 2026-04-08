import { API_BASE_URL } from "@/lib/constants";

type HistoricalSignalRow = {
    timestamp: string;
    value: number;
    name: string;
};

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
        return [];
    }
    return parsed as HistoricalSignalRow[];
}

export async function fetchHistoricalSignal(params: {
    signalName: string;
    startUtcMs: number;
    endUtcMs: number;
}): Promise<HistoricalSignalPoint[]> {
    const { signalName, startUtcMs, endUtcMs } = params;

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    const url = `${API_BASE_URL}/api/v1/signal/tiles/${signalName}/${start}/${end}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical signal ${signalName}: ${response.status} ${errorText}`);
    }

    const payloadText = await response.text();
    const rows = parseHistoricalPayload(payloadText);

    console.log(start, end, rows.length);

    return rows.map((row) => ({
        name: row.name,
        value: row.value,
        timestampMs: Date.parse(row.timestamp),
    }));
}
