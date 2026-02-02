import SignalStore from "@/lib/signals/SignalStore";

class MockSignalStore extends SignalStore {
    private signalSubscriptionIntervals: Map<string, number>;

    constructor() {
        super();

        this.signalSubscriptionIntervals = new Map();
    }

    getReferenceToSignal(signalName: string) {
        const signalData = this.getOrCreateSignalData(signalName);
        this.incrementSubscribers(signalName);

        if (this.getSubscriberCount(signalName) !== 1) return signalData;

        const intervalId = setInterval(() => {
            const currentTime = Date.now();
            const randomValue = Math.floor(Math.random() * 5);

            this.addDataPoint(signalName, currentTime, randomValue);
        }, 1000);

        this.signalSubscriptionIntervals.set(signalName, intervalId as unknown as number);

        return signalData;
    }

    purgeReferenceToSignal(signalName: string) {
        const shouldCleanup = this.decrementSubscribers(signalName);

        if (!shouldCleanup) return;

        this.markAsUnsubscribed(signalName);

        clearInterval(this.signalSubscriptionIntervals.get(signalName));
        this.signalSubscriptionIntervals.delete(signalName);
    }
}

export default MockSignalStore;
