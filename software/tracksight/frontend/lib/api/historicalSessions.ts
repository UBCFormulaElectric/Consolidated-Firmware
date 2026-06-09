import { API_BASE_URL, IS_MOCK } from "@/lib/constants";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";

export type HistoricalSession = {
    id: string;
    startUtcMs: number;
    endUtcMs: number;
    label: string;
    timeZoneLabel: string;
};

const UTC_TIME_ZONE = "UTC";

const sessionTimeFormatter = new Intl.DateTimeFormat("en-CA", {
    day: "2-digit",
    hour: "2-digit",
    hourCycle: "h23",
    minute: "2-digit",
    month: "2-digit",
    timeZone: UTC_TIME_ZONE,
    year: "numeric",
});

function getUtcRangeForUtcDay(dateKey: string): { startUtcMs: number; endUtcMs: number } {
    const [year, month, day] = dateKey.split("-").map(Number);
    return {
        startUtcMs: Date.UTC(year, month - 1, day, 0, 0, 0, 0),
        endUtcMs: Date.UTC(year, month - 1, day + 1, 0, 0, 0, 0),
    };
}

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function formatSessionLabel(startUtcMs: number, endUtcMs: number): string {
    return `${sessionTimeFormatter.format(new Date(startUtcMs))} - ${sessionTimeFormatter.format(new Date(endUtcMs))}`;
}

function toHistoricalSession(startUtcMs: number, endUtcMs: number, index: number): HistoricalSession {
    return {
        id: `${startUtcMs}-${endUtcMs}-${index}`,
        startUtcMs,
        endUtcMs,
        label: formatSessionLabel(startUtcMs, endUtcMs),
        timeZoneLabel: UTC_TIME_ZONE,
    };
}

// Backend returns paired session boundaries as [startMs, endMs | null] tuples,
// with millisecond values encoded as strings. A null end marks the most recent
// session that is still ongoing (no later boot event observed yet).
type SessionBoundaryPair = [string, string | null];

function parseSessionPayload(payloadText: string): SessionBoundaryPair[] {
    const parsed = JSON.parse(payloadText) as unknown;
    if (!Array.isArray(parsed)) {
        console.warn("[historical sessions] Expected an array payload from backend.", parsed);
        return [];
    }
    return parsed as SessionBoundaryPair[];
}

function pairsToSessions(pairs: SessionBoundaryPair[]): HistoricalSession[] {
    return pairs.map(([startStr, endStr], index) => {
        const startUtcMs = Number(startStr);
        // An ongoing session (null end) runs up to "now" so it stays selectable.
        const endUtcMs = endStr !== null ? Number(endStr) : Date.now();
        return toHistoricalSession(startUtcMs, endUtcMs, index);
    });
}

// Synthesizes a backend-shaped payload (string ms, null-terminated last
// session) for NEXT_PUBLIC_USE_MOCK_DATA=true, so the historical session
// dropdown is usable without a running backend/InfluxDB. Runs through the
// same parse/map path as real data. Four "boot events" across the UTC day,
// the last one left open-ended to mirror an ongoing session.
function buildMockSessionPairs(dateKey: string): SessionBoundaryPair[] {
    const { startUtcMs } = getUtcRangeForUtcDay(dateKey);
    const sixHoursMs = 6 * 60 * 60 * 1000;
    const bootMs = [0, 1, 2, 3].map((i) => startUtcMs + i * sixHoursMs);
    return bootMs.map((ms, i) => [String(ms), i < bootMs.length - 1 ? String(bootMs[i + 1]) : null] as SessionBoundaryPair);
}

export async function fetchHistoricalSessionsForDate(dateKey: string, source: HistoricalSignalSource): Promise<HistoricalSession[]> {
    if (IS_MOCK) {
        return pairsToSessions(buildMockSessionPairs(dateKey));
    }

    const dayRange = getUtcRangeForUtcDay(dateKey);

    const start = toIsoUtcSeconds(dayRange.startUtcMs);
    const end = toIsoUtcSeconds(dayRange.endUtcMs);
    const url = `${API_BASE_URL}/api/v1/signal/sessions/${start}/${end}?source=${encodeURIComponent(JSON.stringify(source))}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical sessions: ${response.status} ${errorText}`);
    }

    return pairsToSessions(parseSessionPayload(await response.text()));
}
