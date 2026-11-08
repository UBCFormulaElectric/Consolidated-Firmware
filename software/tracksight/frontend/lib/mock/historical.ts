import type { HistoricalSignalPoint, HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { getMockAlertNames, getMockSignalCatalog } from "@/lib/mock/catalog";
import { readMockParam } from "@/lib/mock/config";
import { tileDurationMs } from "@/lib/signals/lodLevels";
import { SignalType } from "@/lib/types/Signal";

// Mirror the backend's resolution ladder and 256-point viewport target.
const RESOLUTIONS_MS = [10, 20, 50, 100, 500, 1000, 10000, 60000, 600000, 3600000];
const PERIOD_MS = 60000;
const catalog = getMockSignalCatalog();
const patterns = new Map<string, ReturnType<typeof buildPattern>>();

function buildPattern(name: string, source: HistoricalSignalSource) {
    const signal = catalog.find((signal) => signal.name === name);
    const seed = [...name].reduce((seed, char) => (seed * 31 + char.charCodeAt(0)) >>> 0, source === "Radio" ? 0 : 997);
    const intervalMs = signal?.cycle_time_ms ?? 100;
    const states = signal?.type === SignalType.ENUM ? Object.values(signal.enum_signal.enum_values) : [0, 1];
    const values: (number | null)[] = [];
    const sums = [0];
    const counts = [0];

    for (let time = 0; time < PERIOD_MS; time += intervalMs) {
        let value: number | null = null;
        // A shared five-second dropout makes missing-data behaviour visible.
        if (time < 40000 || time >= 45000) {
            if (!signal) {
                value = (time + (seed % 15000)) % 15000 < 1000 ? 1 : 0;
            } else if (signal.type === SignalType.NUMERICAL) {
                let fraction = (Math.sin(time / 2000 + (seed % 100)) + 1) / 2;
                if (time >= 15000 && time < 25000) fraction = (time - 15000) / 10000;
                if (time >= 25000 && time < 40000) fraction = time >= 37000 && time < 37200 ? 1 : 0.4;
                value = signal.min_val + fraction * (signal.max_val - signal.min_val);
            } else {
                const dwellMs = time >= 30000 && time < 35000 ? 200 : 3000;
                value = states[(Math.floor(time / dwellMs) + seed) % states.length];
            }
        }
        values.push(value);
        sums.push(sums[sums.length - 1] + (value ?? 0));
        counts.push(counts[counts.length - 1] + (value === null ? 0 : 1));
    }

    // Prefix sums and next-present indices avoid replaying hours of raw samples.
    const nextPresent = new Array<number>(values.length);
    let next = values.length + values.findIndex((value) => value !== null);
    for (let i = values.length - 1; i >= 0; i--) {
        if (values[i] !== null) next = i;
        nextPresent[i] = next;
    }
    return { intervalMs, values, sums, counts, nextPresent, type: signal?.type ?? SignalType.ALERT };
}

function sampleSum(prefix: number[], first: number, stop: number): number {
    const length = prefix.length - 1;
    const firstPeriod = Math.floor(first / length);
    const lastPeriod = Math.floor(stop / length);
    return (lastPeriod - firstPeriod) * prefix[length] + prefix[stop - lastPeriod * length] - prefix[first - firstPeriod * length];
}

export function buildMockHistoricalPayload(signalName: string, startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource) {
    const targetMs = (endUtcMs - startUtcMs) / 256;
    const resolutionMs = RESOLUTIONS_MS.filter((resolution) => resolution <= targetMs).pop() ?? RESOLUTIONS_MS[0];
    const points: HistoricalSignalPoint[] = [];
    const names = signalName === "alert" ? getMockAlertNames(readMockParam("mockAlerts") ?? 12) : [signalName];
    const tileMs = tileDurationMs(resolutionMs);
    const start = Math.floor(startUtcMs / tileMs) * tileMs;
    const end = Math.ceil(endUtcMs / tileMs) * tileMs;

    if (startUtcMs < endUtcMs) {
        for (const name of names) {
            const key = `${source}:${name}`;
            let pattern = patterns.get(key);
            if (!pattern) {
                pattern = buildPattern(name, source);
                patterns.set(key, pattern);
            }
            const { intervalMs, values, sums, counts, nextPresent, type } = pattern;
            for (let time = start; time < end; time += resolutionMs) {
                const first = Math.ceil(time / intervalMs);
                const stop = Math.ceil((time + resolutionMs) / intervalMs);
                const count = sampleSum(counts, first, stop);
                if (!count) continue;

                const sum = sampleSum(sums, first, stop);
                const index = ((first % values.length) + values.length) % values.length;
                const value = type === SignalType.NUMERICAL ? sum / count : type === SignalType.ALERT ? Number(sum > 0) : values[nextPresent[index] % values.length]!;
                // Flux aggregateWindow timestamps each value at the bucket's end.
                points.push({ timestampMs: time + resolutionMs, value, name });
            }
        }
    }

    return {
        resolution_ms: resolutionMs,
        rows: points.map(({ timestampMs, value, name }) => ({ timestamp: new Date(timestampMs).toISOString(), value, name })),
    };
}

export function buildMockHistoricalMarkers(startUtcMs: number, endUtcMs: number): number[] {
    const markers: number[] = [];
    for (let time = Math.ceil(startUtcMs / PERIOD_MS) * PERIOD_MS; time < endUtcMs; time += PERIOD_MS) markers.push(time);
    return markers;
}

export async function waitForMockHistoricalSignal(): Promise<void> {
    const params = new URLSearchParams(typeof window === "undefined" ? "" : window.location.search);
    const delayMs = Number(params.get("mockDelay"));
    if (Number.isSafeInteger(delayMs) && delayMs > 0) await new Promise((resolve) => setTimeout(resolve, delayMs));
    if (params.get("mockError") === "1") throw new Error("Simulated historical signal failure");
}
