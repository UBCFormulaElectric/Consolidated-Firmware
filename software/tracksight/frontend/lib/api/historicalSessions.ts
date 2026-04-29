export type HistoricalSession = {
    id: string;
    startUtcMs: number;
    endUtcMs: number;
    label: string;
};

const SESSION_BLOCKS = [
    { startHour: 0, endHour: 6, label: "00:00 - 06:00" },
    { startHour: 6, endHour: 12, label: "06:00 - 12:00" },
    { startHour: 12, endHour: 18, label: "12:00 - 18:00" },
    { startHour: 18, endHour: 24, label: "18:00 - 00:00" },
] as const;

function getUtcDayStartFromDateKey(dateKey: string): number {
    const [year, month, day] = dateKey.split("-").map(Number);
    return Date.UTC(year, month - 1, day, 0, 0, 0, 0);
}

export async function fetchHistoricalSessionsForDate(dateKey: string): Promise<HistoricalSession[]> {
    const dayStartUtcMs = getUtcDayStartFromDateKey(dateKey);

    // TODO: replace w/ actual api call to jack's sessions endpoint
    return SESSION_BLOCKS.map((block) => ({
        id: `${dateKey}-${block.startHour}-${block.endHour}`,
        startUtcMs: dayStartUtcMs + block.startHour * 60 * 60 * 1000,
        endUtcMs: dayStartUtcMs + block.endHour * 60 * 60 * 1000,
        label: block.label,
    }));
}
