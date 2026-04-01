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

function escapeRegexLiteral(value: string): string {
    return value.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
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
    resolution: string;
}): Promise<HistoricalSignalPoint[]> {
    const { signalName, startUtcMs, endUtcMs, resolution } = params;

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    const exactRegex = `(?:^${escapeRegexLiteral(signalName)}$)`;
    const url = `${API_BASE_URL}/api/v1/signal/${encodeURIComponent(start)}/${encodeURIComponent(end)}/${encodeURIComponent(resolution)}?name=${encodeURIComponent(exactRegex)}`;

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
        timestampMs: new Date(row.timestamp).getTime(),
    }));
}
