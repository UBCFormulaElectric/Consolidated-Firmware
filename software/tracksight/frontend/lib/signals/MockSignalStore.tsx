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

        let previousValue = 0;

        // NOTE(evan): When a browser tab isn't focused setInterval is throttled to run once per second.
        //             This can make the renderers appear frozen if they rely on frequent updates. In 
        //             production when using an actual signal source this shouldn't be an issue.
        const intervalId = setInterval(() => {
            const currentTime = Date.now();
            const shouldSwitch = Math.random() < 0.5;
            const randomValue = shouldSwitch ? 1 - previousValue : previousValue;
            previousValue = randomValue;

            this.addDataPoint(signalName, currentTime, randomValue);
        }, 0.0001);

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
