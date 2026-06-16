import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { API_BASE_URL, IS_MOCK } from "@/lib/constants";

export type HistoricalSession = {
    id: string;
    startUtcMs: number;
    endUtcMs: number;
    label: string;
    timeZoneLabel: string;
};

const SESSION_FORMATTERS_CACHE = new Map<string, { full: Intl.DateTimeFormat; timeOnly: Intl.DateTimeFormat; dateOnly: Intl.DateTimeFormat }>();

function getSessionFormatters(timeZone: string) {
    if (!SESSION_FORMATTERS_CACHE.has(timeZone)) {
        SESSION_FORMATTERS_CACHE.set(timeZone, {
            full: new Intl.DateTimeFormat("en-CA", {
                day: "2-digit",
                hour: "2-digit",
                hourCycle: "h23",
                minute: "2-digit",
                month: "2-digit",
                timeZone,
                year: "numeric",
            }),
            timeOnly: new Intl.DateTimeFormat("en-CA", {
                hour: "2-digit",
                hourCycle: "h23",
                minute: "2-digit",
                timeZone,
            }),
            dateOnly: new Intl.DateTimeFormat("en-CA", {
                timeZone,
                year: "numeric",
                month: "2-digit",
                day: "2-digit",
            }),
        });
    }
    return SESSION_FORMATTERS_CACHE.get(timeZone)!;
}

function isSameDay(startUtcMs: number, endUtcMs: number, timeZone: string) {
    const fmt = getSessionFormatters(timeZone).dateOnly;
    return fmt.format(new Date(startUtcMs)) === fmt.format(new Date(endUtcMs));
}

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

export function formatSessionLabel(startUtcMs: number, endUtcMs: number, timeZone: string): string {
    const start = new Date(startUtcMs);
    const end = new Date(endUtcMs);
    const formatters = getSessionFormatters(timeZone);

    if (isSameDay(startUtcMs, endUtcMs, timeZone)) {
        return `${formatters.timeOnly.format(start)} - ${formatters.timeOnly.format(end)}`;
    }

    return `${formatters.full.format(start)} - ${formatters.full.format(end)}`;
}

function toHistoricalSession(startUtcMs: number, endUtcMs: number, timeZone: string): HistoricalSession {
    return {
        id: `${startUtcMs}-${endUtcMs}`,
        startUtcMs,
        endUtcMs,
        label: formatSessionLabel(startUtcMs, endUtcMs, timeZone),
        timeZoneLabel: timeZone === "America/Vancouver" ? "PDT" : timeZone === "America/Detroit" ? "EDT" : timeZone === "UTC" ? "UTC" : timeZone.split("/").pop()!,
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

function pairsToSessions(pairs: SessionBoundaryPair[], timeZone: string): HistoricalSession[] {
    return pairs.map(([startStr, endStr]) => {
        const startUtcMs = Number(startStr);
        // An ongoing session (null end) runs up to "now" so it stays selectable.
        const endUtcMs = endStr !== null ? Number(endStr) : Date.now();
        return toHistoricalSession(startUtcMs, endUtcMs, timeZone);
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

    return bootMs.filter((ms) => ms >= startUtcMs && ms < endUtcMs).map((ms, i, all) => [String(ms), i < all.length - 1 ? String(ms + 3 * 60 * 60 * 1000) : null] as SessionBoundaryPair);
}

export async function fetchHistoricalSessionsForRange(startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource, timeZone: string): Promise<HistoricalSession[]> {
    if (IS_MOCK) {
        return pairsToSessions(buildMockSessionPairs(startUtcMs, endUtcMs), timeZone);
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

    return pairsToSessions(parseSessionPayload(await response.text()), timeZone);
}
