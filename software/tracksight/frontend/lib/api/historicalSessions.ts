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

function utcDateTimeToMs(parts: { year: number; month: number; day: number; hour: number }): number {
    return Date.UTC(parts.year, parts.month - 1, parts.day, parts.hour, 0, 0, 0);
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

export async function fetchHistoricalSessionsForDate(dateKey: string): Promise<HistoricalSession[]> {
    const [year, month, day] = dateKey.split("-").map(Number);
    const dayRange = getUtcRangeForUtcDay(dateKey);

    // TODO TODO TODO TODO TODO TODO TODO  !!!!! very important - jaden
    // MOCK HISTORICAL SESSIONS
    // assumes four six-hour sessions in UTC; 00-06, 06-12, 12-18, 18-24
    return [0, 6, 12, 18].map((hour, index) => toHistoricalSession(utcDateTimeToMs({ year, month, day, hour }), hour === 18 ? dayRange.endUtcMs : utcDateTimeToMs({ year, month, day, hour: hour + 6 }), index));
}
