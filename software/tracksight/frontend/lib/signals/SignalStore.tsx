import AutoResizingTypedArray from "@/lib/utils/AutoResizingTypedArray";

export type SignalData = {
    timePoints: AutoResizingTypedArray;
    values: AutoResizingTypedArray;
}

export type SignalStoreEntry = {
    data: SignalData;
    error: unknown | null;
    isSubscribed: boolean;
}

abstract class SignalStore {
    protected storage: { [signalName: string]: SignalStoreEntry };
    protected subscriberCounts: { [signalName: string]: number };

    constructor() {
        this.storage = {};
        this.subscriberCounts = {};
    }

    abstract getReferenceToSignal(signalName: string): SignalStoreEntry;
    abstract purgeReferenceToSignal(signalName: string): void;

    private createSignalDataEntry(signalName: string) {
        this.storage[signalName] = {
            data: {
                timePoints: new AutoResizingTypedArray(),
                values: new AutoResizingTypedArray(),
            },
            error: null,
            isSubscribed: true,
        };

        this.subscriberCounts[signalName] = 0;
    }

    protected getOrCreateSignalData(signalName: string): SignalStoreEntry {
        if (!this.storage[signalName]) this.createSignalDataEntry(signalName);

        return this.getSignalData(signalName)!;
    }

    getSignalData(signalName: string): SignalStoreEntry | undefined {
        return this.storage[signalName];
    }

    incrementSubscribers(signalName: string): void {
        if (!this.subscriberCounts[signalName]) this.subscriberCounts[signalName] = 0;

        this.subscriberCounts[signalName] += 1;
    }

    decrementSubscribers(signalName: string): boolean {
        if (!this.subscriberCounts[signalName]) return false;

        this.subscriberCounts[signalName] -= 1;

        return this.subscriberCounts[signalName] <= 0;
    }

    getSubscriberCount(signalName: string): number {
        return this.subscriberCounts[signalName] || 0;
    }

    markAsUnsubscribed(signalName: string): void {
        if (!this.storage[signalName]) return;

        this.storage[signalName].isSubscribed = false;
    }

    removeSignal(signalName: string): void {
        delete this.storage[signalName];
        delete this.subscriberCounts[signalName];
    }

    setError(signalName: string, error: unknown): void {
        if (!this.storage[signalName]) return;

        this.storage[signalName].error = error;
    }

    addDataPoint(signalName: string, timestamp: number, value: number): void {
        const entry = this.storage[signalName];

        if (!entry) return;

        entry.data.timePoints.push(timestamp);
        entry.data.values.push(value);
    }
}

export default SignalStore;