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

const sessionTimeOnlyFormatter = new Intl.DateTimeFormat("en-CA", {
    hour: "2-digit",
    hourCycle: "h23",
    minute: "2-digit",
    timeZone: UTC_TIME_ZONE,
});

const isSameUtcDay = (a: Date, b: Date) => a.getUTCFullYear() === b.getUTCFullYear() && a.getUTCMonth() === b.getUTCMonth() && a.getUTCDate() === b.getUTCDate();

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function formatSessionLabel(startUtcMs: number, endUtcMs: number): string {
    const start = new Date(startUtcMs);
    const end = new Date(endUtcMs);

    if (isSameUtcDay(start, end)) {
        return `${sessionTimeOnlyFormatter.format(start)} - ${sessionTimeOnlyFormatter.format(end)}`;
    }

    return `${sessionTimeFormatter.format(start)} - ${sessionTimeFormatter.format(end)}`;
}

function toHistoricalSession(startUtcMs: number, endUtcMs: number): HistoricalSession {
    return {
        id: `${startUtcMs}-${endUtcMs}`,
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
    return pairs.map(([startStr, endStr]) => {
        const startUtcMs = Number(startStr);
        // An ongoing session (null end) runs up to "now" so it stays selectable.
        const endUtcMs = endStr !== null ? Number(endStr) : Date.now();
        return toHistoricalSession(startUtcMs, endUtcMs);
    });
}

const DAY_MS = 24 * 60 * 60 * 1000;

// Synthesizes a backend-shaped payload (string ms, null-terminated last
// session) for NEXT_PUBLIC_USE_MOCK_DATA=true, so the historical session
// dropdown is usable without a running backend/InfluxDB. Runs through the
// same parse/map path as real data. Four "boot events" across the UTC day,
// the last one left open-ended to mirror an ongoing session.
function buildMockSessionPairs(startUtcMs: number, endUtcMs: number): SessionBoundaryPair[] {
    const bootMs: number[] = [];
    const firstDayStart = Math.floor(startUtcMs / DAY_MS) * DAY_MS;

    for (let dayStart = firstDayStart; dayStart < endUtcMs; dayStart += DAY_MS) {
        if ((dayStart / DAY_MS) % 3 === 0) continue;
        
        bootMs.push(dayStart + 9 * 60 * 60 * 1000); // 09:00 UTC
        bootMs.push(dayStart + 15 * 60 * 60 * 1000); // 15:00 UTC
    }

    return bootMs
        .filter((ms) => ms >= startUtcMs && ms < endUtcMs)
        .map((ms, i, all) => [String(ms), i < all.length - 1 ? String(ms + 3 * 60 * 60 * 1000) : null] as SessionBoundaryPair);
}

export async function fetchHistoricalSessionsForRange(startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource): Promise<HistoricalSession[]> {
    if (IS_MOCK) {
        return pairsToSessions(buildMockSessionPairs(startUtcMs, endUtcMs));
    }

    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
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
