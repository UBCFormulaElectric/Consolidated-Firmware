import SignalStore, { SignalStoreReturnType } from "@/lib/signals/SignalStore";
import { AlertSignalMetadata, SignalMetadata, SignalType } from "@/lib/types/Signal";
import { HistoricalSignalPoint } from "@/lib/api/historicalSignals";

const alertMetadata = (name: string): AlertSignalMetadata => ({
    name,
    type: SignalType.ALERT,
    tx_node: "",
    msg_name: "",
    id: -1,
    min_val: 0,
    max_val: 1,
    cycle_time_ms: null,
});

class HistoricalSignalStore extends SignalStore {
    constructor(updateWithTimestamp: (timestamp: number) => void) {
        super(updateWithTimestamp);
    }

    getReferenceToSignal<T extends SignalMetadata>(signal: T): SignalStoreReturnType<T> {
        const signalData = this.getOrCreateSignalData(signal);
        this.incrementSubscribers(signal.name);
        return signalData.data as SignalStoreReturnType<T>;
    }

    purgeReferenceToSignal(signal: SignalMetadata): void {
        const shouldCleanup = this.decrementSubscribers(signal.name);
        if (!shouldCleanup) {
            return;
        }
        this.markAsUnsubscribed(signal.name);
    }

    /**
     * Merge a fetched window for a single widget signal into the LOD level for its resolution.
     * Enum metadata is (re)applied so the renderer can label values.
     */
    mergeSignal(signal: SignalMetadata, resolutionMs: number, requestStartMs: number, requestEndMs: number, points: HistoricalSignalPoint[]): void {
        const entry = this.getOrCreateSignalData(signal);

        if (signal.type === SignalType.ENUM && entry.storeType === SignalType.ENUM) {
            Object.values(signal.enum_signal.enum_values).forEach((enumValue) => {
                entry.data.enumValuesToNames[enumValue] = [enumValue];
            });
        }

        this.mergeHistoricalPoints(signal.name, resolutionMs, requestStartMs, requestEndMs, points);
    }

    /**
     * Merge a fetched window of the `"alert"` pseudo-signal, which fans out into many fault
     * series. Coverage is marked for every known alert series (not just the ones present in
     * this response) because a tile with no points means the fault was simply off there — its
     * level is still covered.
     */
    mergeAlerts(resolutionMs: number, requestStartMs: number, requestEndMs: number, points: HistoricalSignalPoint[]): void {
        const pointsByName = new Map<string, HistoricalSignalPoint[]>();
        points.forEach((point) => {
            const list = pointsByName.get(point.name);
            if (list) {
                list.push(point);
            } else {
                pointsByName.set(point.name, [point]);
            }
        });

        const names = new Set<string>([...this.alertSignalNames, ...pointsByName.keys()]);
        names.forEach((name) => {
            this.getOrCreateSignalData(alertMetadata(name));
            this.mergeHistoricalPoints(name, resolutionMs, requestStartMs, requestEndMs, pointsByName.get(name) ?? []);
        });
    }
}

export default HistoricalSignalStore;
