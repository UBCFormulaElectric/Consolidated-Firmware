import SignalStore, { SignalStoreReturnType } from "@/lib/signals/SignalStore";
import { SignalMetadata, SignalType } from "@/lib/types/Signal";
import { HistoricalSignalPoint } from "@/lib/api/historicalSignals";

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

    clearSignals(signals: SignalMetadata[]): void {
        signals.forEach((signal) => {
            this.getOrCreateSignalData(signal);
            this.clearSignalData(signal.name);
        });
    }

    hydrateSignal(signal: SignalMetadata, points: HistoricalSignalPoint[]): void {
        const entry = this.getOrCreateSignalData(signal);
        if (signal.type === SignalType.ENUM && entry.storeType === SignalType.ENUM) {
            const enumItems = signal.enum.items;
            Object.keys(enumItems).forEach((enumValue) => {
                entry.data.enumValuesToNames[Number(enumValue)] = [enumItems[Number(enumValue)]];
            });
        }
        points.forEach((point) => {
            this.addDataPoint(signal.name, point.timestampMs, point.value);
        });
    }
}

export default HistoricalSignalStore;
