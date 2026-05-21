import { HISTORICAL_SESSION_TIME_ZONE } from "@/lib/constants";

export type HistoricalSession = {
    id: string;
    startUtcMs: number;
    endUtcMs: number;
    label: string;
    timeZoneLabel: string;
};

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

function formatSessionLabel(startUtcMs: number, endUtcMs: number): string {
    return `${sessionTimeFormatter.format(new Date(startUtcMs))} - ${sessionTimeFormatter.format(new Date(endUtcMs))}`;
}

function getTimeZoneLabel(timestampMs: number): string {
    return timeZoneNameFormatter.formatToParts(new Date(timestampMs)).find((part) => part.type === "timeZoneName")?.value ?? HISTORICAL_SESSION_TIME_ZONE;
}

function toHistoricalSession(startUtcMs: number, endUtcMs: number, index: number): HistoricalSession {
    return {
        id: `${startUtcMs}-${endUtcMs}-${index}`,
        startUtcMs,
        endUtcMs,
        label: formatSessionLabel(startUtcMs, endUtcMs),
        timeZoneLabel: getTimeZoneLabel(startUtcMs),
    };
}

export async function fetchHistoricalSessionsForDate(dateKey: string): Promise<HistoricalSession[]> {
    const [year, month, day] = dateKey.split("-").map(Number);

    // TODO TODO TODO TODO TODO TODO TODO  !!!!! very important - jaden
    // MOCK HISTORICAL SESSIONS
    // assumes four six-hour sessions in the configured historical timezone; 00-06, 06-12, 12-18, 18-24
    return [0, 6, 12, 18].map((hour, index) =>
        toHistoricalSession(
            localTimeToUtcMs({ year, month, day, hour }),
            hour === 18 ? getUtcRangeForLocalDay(dateKey).endUtcMs : localTimeToUtcMs({ year, month, day, hour: hour + 6 }),
            index
        )
    );
}
