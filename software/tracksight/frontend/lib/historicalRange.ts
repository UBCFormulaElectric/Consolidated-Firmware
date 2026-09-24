export type HistoricalRange = { min: number; max: number };

export function inHistoricalRange(timestampMs: number, range: HistoricalRange): boolean {
    return Number.isFinite(timestampMs) && timestampMs >= range.min && timestampMs < range.max;
}

export function historicalPointsInRange(rows: { timestamp: string; name: string; value: number }[], range: HistoricalRange) {
    const points: { timestampMs: number; name: string; value: number }[] = [];
    for (const row of rows) {
        const timestampMs = Date.parse(row.timestamp);
        if (inHistoricalRange(timestampMs, range)) points.push({ timestampMs, name: row.name, value: row.value });
    }
    return points;
}

export function rangeOfHistoricalPoints(groups: { timestampMs: number }[][], range: HistoricalRange): HistoricalRange | null {
    let min = Infinity;
    let max = -Infinity;

    for (const points of groups) {
        for (const point of points) {
            if (!inHistoricalRange(point.timestampMs, range)) continue;
            min = Math.min(min, point.timestampMs);
            max = Math.max(max, point.timestampMs);
        }
    }

    return min <= max ? { min, max } : null;
}
