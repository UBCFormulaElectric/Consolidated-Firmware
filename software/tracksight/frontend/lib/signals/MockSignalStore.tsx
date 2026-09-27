import { getMockAlertNames } from "@/lib/mock/catalog";
import { getMockConfig } from "@/lib/mock/config";
import SignalStore from "@/lib/signals/SignalStore";
import { addTelemetryMarker } from "../telemetryMarkers";
import { isEnumSignalMetadata, SignalMetadata, SignalType } from "../types/Signal";
import propagateHaar, { HaarLodBuffer } from "../utils/propagateHaar";
import propagateMode, { ModeLodBuffer } from "../utils/propagateMode";

const NUM_LOD_LEVELS = 15;
// one shared clock for every mocked signal: per-signal timers would make the mock itself the bottleneck under stress
const TICK_MS = 10;
// a throttled background tab skips ahead rather than emitting a burst of backlogged samples
const MAX_CATCH_UP_MS = 1_000;
const MOCK_MARKER_INTERVAL_MS = 8_000;

// real alert messages are sent at 10 Hz and are inactive most of the time
const ALERT_SAMPLE_INTERVAL_MS = 100;
const ALERT_MEAN_ACTIVE_MS = 2_000;
const ALERT_MEAN_INACTIVE_MS = 12_000;
// enums and booleans hold a state for a while instead of changing every sample
const STATE_MEAN_DWELL_MS = 1_500;
const FALLBACK_STATES = [0, 1, 2, 3];

type SampleGenerator = {
    signal: SignalMetadata;
    periodMs: number;
    nextSampleAt: number;
    phase: number;
    value: number;
    states: number[];
    lodBuffers: HaarLodBuffer[] | ModeLodBuffer[];
};

const alertMetadata = (name: string): SignalMetadata => ({ name, type: SignalType.ALERT, tx_node: "", msg_name: "", id: -1, min_val: 0, max_val: 1, cycle_time_ms: ALERT_SAMPLE_INTERVAL_MS });

function numericalValue(time: number, phase: number, min: number, max: number) {
    const wave = (Math.sin(time / 1000 + phase) + 1) / 2;
    const noise = (Math.random() - 0.5) * 0.1;
    return min + Math.max(0, Math.min(1, wave + noise)) * (max - min);
}

class MockSignalStore extends SignalStore {
    private generators = new Map<string, SampleGenerator>();
    private tickIntervalId: number | null = null;
    private markerIntervalId: number | null = null;

    constructor(updateWithTimestamp: (timestamp: number) => void) {
        super(updateWithTimestamp);

        if (typeof window === "undefined") return;

        getMockAlertNames(getMockConfig().alertCount).forEach((name) => {
            const signal = alertMetadata(name);
            this.getOrCreateSignalData(signal);
            this.generators.set(name, this.createGenerator(signal, ALERT_SAMPLE_INTERVAL_MS));
        });
    }

    /** Starts generating; paired with stop() so unmounting doesn't leak timers. Safe to call repeatedly. */
    start() {
        if (typeof window === "undefined" || this.tickIntervalId !== null) return;

        this.tickIntervalId = window.setInterval(this.tick, TICK_MS);
        this.markerIntervalId = window.setInterval(() => {
            const now = Date.now();
            addTelemetryMarker({ timestampMs: now });
            this.updateWithTimestamp(now);
        }, MOCK_MARKER_INTERVAL_MS);
    }

    stop() {
        if (this.tickIntervalId !== null) window.clearInterval(this.tickIntervalId);
        if (this.markerIntervalId !== null) window.clearInterval(this.markerIntervalId);
        this.tickIntervalId = null;
        this.markerIntervalId = null;
    }

    private createGenerator(signal: SignalMetadata, periodMs: number): SampleGenerator {
        const states = signal.type === SignalType.BOOLEAN ? [0, 1] : isEnumSignalMetadata(signal) ? Object.values(signal.enum_signal?.enum_values ?? {}) : [];

        return {
            signal,
            periodMs,
            nextSampleAt: Date.now(),
            phase: Math.random() * Math.PI * 2,
            value: states.length > 0 ? states[Math.floor(Math.random() * states.length)] : 0,
            states: states.length > 0 ? states : FALLBACK_STATES,
            lodBuffers: new Array(NUM_LOD_LEVELS).fill(null),
        };
    }

    private tick = () => {
        const now = Date.now();
        this.generators.forEach((generator) => {
            if (now - generator.nextSampleAt > MAX_CATCH_UP_MS) generator.nextSampleAt = now - MAX_CATCH_UP_MS;

            while (generator.nextSampleAt <= now) {
                this.emitSample(generator, generator.nextSampleAt);
                generator.nextSampleAt += generator.periodMs;
            }
        });
    };

    private emitSample(generator: SampleGenerator, timestamp: number) {
        const { signal, periodMs } = generator;
        const onLodSample = (level: number, intervalMs: number, lodTimestamp: number, lodValue: number) => this.addDataPointAtLOD(signal.name, level, intervalMs, lodTimestamp, lodValue);

        switch (signal.type) {
            case SignalType.NUMERICAL: {
                const value = numericalValue(timestamp, generator.phase, signal.min_val, signal.max_val);
                this.addDataPoint(signal.name, timestamp, value);
                propagateHaar(generator.lodBuffers as HaarLodBuffer[], 0, timestamp, value, onLodSample, NUM_LOD_LEVELS);
                return;
            }
            case SignalType.ENUM:
            case SignalType.BOOLEAN: {
                if (Math.random() < periodMs / STATE_MEAN_DWELL_MS) {
                    const others = generator.states.filter((state) => state !== generator.value);
                    generator.value = others[Math.floor(Math.random() * others.length)] ?? generator.value;
                }
                this.addDataPoint(signal.name, timestamp, generator.value);
                propagateMode(generator.lodBuffers as ModeLodBuffer[], 0, timestamp, { [generator.value]: 1 }, onLodSample, NUM_LOD_LEVELS);
                return;
            }
            case SignalType.ALERT: {
                const meanDwellMs = generator.value === 1 ? ALERT_MEAN_ACTIVE_MS : ALERT_MEAN_INACTIVE_MS;
                if (Math.random() < periodMs / meanDwellMs) generator.value = generator.value === 1 ? 0 : 1;
                this.addDataPoint(signal.name, timestamp, generator.value);
                return;
            }
        }
    }

    getReferenceToSignal<T extends SignalMetadata>(signal: T) {
        const signalData = this.getOrCreateSignalData(signal);
        this.incrementSubscribers(signal.name);

        if (this.getSubscriberCount(signal.name) === 1 && signal.type !== SignalType.ALERT) {
            this.generators.set(signal.name, this.createGenerator(signal, 1000 / getMockConfig().sampleHz));
        }

        return signalData.data as any;
    }

    purgeReferenceToSignal<T extends SignalMetadata>(signal: T) {
        if (!this.decrementSubscribers(signal.name)) return;

        this.markAsUnsubscribed(signal.name);
        this.generators.delete(signal.name);
    }
}

export default MockSignalStore;
