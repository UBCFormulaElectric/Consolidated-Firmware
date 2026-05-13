import { API_BASE_URL, HISTORICAL_SESSION_TIME_ZONE } from "@/lib/constants";

export type HistoricalSession = {
    id: string;
    startUtcMs: number;
    endUtcMs: number;
    label: string;
    timeZoneLabel: string;
};

type HistoricalSessionRange = [start: string, end: string];

type LocalDateTimeParts = {
    year: number;
    month: number;
    day: number;
    hour: number;
    minute: number;
    second: number;
};

const sessionTimeFormatter = new Intl.DateTimeFormat("en-CA", {
    day: "2-digit",
    hour: "2-digit",
    hourCycle: "h23",
    minute: "2-digit",
    month: "2-digit",
    timeZone: HISTORICAL_SESSION_TIME_ZONE,
    year: "numeric",
});

const wallClockFormatter = new Intl.DateTimeFormat("en-US", {
    day: "2-digit",
    hour: "2-digit",
    hourCycle: "h23",
    minute: "2-digit",
    month: "2-digit",
    second: "2-digit",
    timeZone: HISTORICAL_SESSION_TIME_ZONE,
    year: "numeric",
});

const timeZoneNameFormatter = new Intl.DateTimeFormat("en-US", {
    timeZone: HISTORICAL_SESSION_TIME_ZONE,
    timeZoneName: "short",
});

function getPart(parts: Intl.DateTimeFormatPart[], type: Intl.DateTimeFormatPartTypes): number {
    const value = parts.find((part) => part.type === type)?.value;
    if (!value) {
        throw new Error(`Unable to read ${type} for ${HISTORICAL_SESSION_TIME_ZONE}.`);
    }

    return Number(value);
}

function getLocalDateTimeParts(timestampMs: number): LocalDateTimeParts {
    const parts = wallClockFormatter.formatToParts(new Date(timestampMs));

    return {
        year: getPart(parts, "year"),
        month: getPart(parts, "month"),
        day: getPart(parts, "day"),
        hour: getPart(parts, "hour"),
        minute: getPart(parts, "minute"),
        second: getPart(parts, "second"),
    };
}

function getTimeZoneOffsetMs(timestampMs: number): number {
    const localTime = getLocalDateTimeParts(timestampMs);
    const localTimeAsUtcMs = Date.UTC(localTime.year, localTime.month - 1, localTime.day, localTime.hour, localTime.minute, localTime.second);

    return localTimeAsUtcMs - timestampMs;
}

function localTimeToUtcMs(parts: Pick<LocalDateTimeParts, "year" | "month" | "day" | "hour">): number {
    const assumedUtcMs = Date.UTC(parts.year, parts.month - 1, parts.day, parts.hour, 0, 0, 0);
    const firstPassUtcMs = assumedUtcMs - getTimeZoneOffsetMs(assumedUtcMs);

    return assumedUtcMs - getTimeZoneOffsetMs(firstPassUtcMs);
}

function getUtcRangeForLocalDay(dateKey: string): { startUtcMs: number; endUtcMs: number } {
    const [year, month, day] = dateKey.split("-").map(Number);
    return {
        startUtcMs: localTimeToUtcMs({ year, month, day, hour: 0 }),
        endUtcMs: localTimeToUtcMs({ year, month, day: day + 1, hour: 0 }),
    };
}

function toIsoUtcSeconds(timestampMs: number): string {
    return new Date(timestampMs).toISOString().slice(0, 19) + "Z";
}

function parseHistoricalSessionsPayload(payloadText: string): HistoricalSessionRange[] {
    const parsed = JSON.parse(payloadText) as unknown;
    if (!Array.isArray(parsed)) {
        return [];
    }

    return parsed as HistoricalSessionRange[];
}

function parseSessionTimestamp(timestamp: string): number {
    const timestampMs = Date.parse(timestamp);

    if (!Number.isFinite(timestampMs)) {
        throw new Error("Historical sessions response included a session without valid start/end timestamps.");
    }

    return timestampMs;
}

function formatSessionLabel(startUtcMs: number, endUtcMs: number): string {
    return `${sessionTimeFormatter.format(new Date(startUtcMs))} - ${sessionTimeFormatter.format(new Date(endUtcMs))}`;
}

function getTimeZoneLabel(timestampMs: number): string {
    return timeZoneNameFormatter.formatToParts(new Date(timestampMs)).find((part) => part.type === "timeZoneName")?.value ?? HISTORICAL_SESSION_TIME_ZONE;
}

function toHistoricalSession(range: HistoricalSessionRange, index: number): HistoricalSession {
    const [start, end] = range;
    const startUtcMs = parseSessionTimestamp(start);
    const endUtcMs = parseSessionTimestamp(end);

    return {
        id: `${startUtcMs}-${endUtcMs}-${index}`,
        startUtcMs,
        endUtcMs,
        label: formatSessionLabel(startUtcMs, endUtcMs),
        timeZoneLabel: getTimeZoneLabel(startUtcMs),
    };
}

function overlapsRange(session: HistoricalSession, startUtcMs: number, endUtcMs: number): boolean {
    return session.startUtcMs < endUtcMs && session.endUtcMs > startUtcMs;
}

export async function fetchHistoricalSessionsForDate(dateKey: string): Promise<HistoricalSession[]> {
    const { startUtcMs, endUtcMs } = getUtcRangeForLocalDay(dateKey);
    const start = toIsoUtcSeconds(startUtcMs);
    const end = toIsoUtcSeconds(endUtcMs);
    const url = `${API_BASE_URL}/api/v1/signal/sessions/${start}/${end}`;

    const response = await fetch(url, {
        cache: "no-store",
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(`Failed to fetch historical sessions: ${response.status} ${errorText}`);
    }

    const payloadText = await response.text();
    const rows = parseHistoricalSessionsPayload(payloadText);

    return rows
        .map(toHistoricalSession)
        .filter((session) => overlapsRange(session, startUtcMs, endUtcMs))
        .sort((a, b) => a.startUtcMs - b.startUtcMs);
}
