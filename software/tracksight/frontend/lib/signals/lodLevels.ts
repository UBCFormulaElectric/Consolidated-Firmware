// NOTE(evan): Mirror of `TILE_SIZE` (points per tile) in signal_tile.rs.
export const TILE_SIZE = 512;

export function tileDurationMs(sampleIntervalMs: number): number {
    return TILE_SIZE * sampleIntervalMs;
}

export function tileIndexForTimestamp(timestampMs: number, sampleIntervalMs: number): number {
    return Math.floor(timestampMs / tileDurationMs(sampleIntervalMs));
}

export function markTilesCovered(coveredTiles: Set<number>, sampleIntervalMs: number, startMs: number, endMs: number): void {
    if (sampleIntervalMs <= 0) return;
    
    const first = tileIndexForTimestamp(startMs, sampleIntervalMs);
    const last = tileIndexForTimestamp(endMs, sampleIntervalMs);

    for (let tile = first; tile <= last; tile++) {
        coveredTiles.add(tile);
    }
}

export type LevelPoint = { timestampMs: number; value: number };

export function mergeSortedPoints(
    existingTimestamps: number[],
    existingValues: number[],
    batch: LevelPoint[],
): { timestamps: number[]; values: number[] } {
    const sorted = [...batch].sort((a, b) => a.timestampMs - b.timestampMs);

    const timestamps: number[] = [];
    const values: number[] = [];
    const push = (ts: number, value: number) => {
        if (timestamps.length > 0 && timestamps[timestamps.length - 1] === ts) {
            values[values.length - 1] = value;
        } else {
            timestamps.push(ts);
            values.push(value);
        }
    };

    let i = 0;
    let j = 0;
    
    while (i < existingTimestamps.length && j < sorted.length) {
        if (existingTimestamps[i] < sorted[j].timestampMs) {
            push(existingTimestamps[i], existingValues[i]);
            i++;
        } else if (existingTimestamps[i] > sorted[j].timestampMs) {
            push(sorted[j].timestampMs, sorted[j].value);
            j++;
        } else {
            push(sorted[j].timestampMs, sorted[j].value);
            i++;
            j++;
        }
    }
    
    while (i < existingTimestamps.length) {
        push(existingTimestamps[i], existingValues[i]);
        i++;
    }
    
    while (j < sorted.length) {
        push(sorted[j].timestampMs, sorted[j].value);
        j++;
    }

    return { timestamps, values };
}

export function coversRange(coveredTiles: Set<number>, sampleIntervalMs: number, minMs: number, maxMs: number): boolean {
    if (sampleIntervalMs <= 0) return false;

    const first = tileIndexForTimestamp(minMs, sampleIntervalMs);
    const last = tileIndexForTimestamp(maxMs, sampleIntervalMs);

    for (let tile = first; tile <= last; tile++) {
        if (!coveredTiles.has(tile)) return false;
    }

    return true;
}
