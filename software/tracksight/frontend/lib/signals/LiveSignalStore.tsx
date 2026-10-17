import socket from "@/lib/realtime/socket";
import SignalStore from "@/lib/signals/SignalStore";
import { addTelemetryMarker } from "../telemetryMarkers";
import { SignalMetadata, SignalType } from "../types/Signal";
import propagateHaar, { HaarLodBuffer } from "../utils/propagateHaar";
import propagateMode, { ModeLodBuffer } from "../utils/propagateMode";

type SignalSubscriptionCallbacks = {
    onSuccess?: () => void;
    onError?: (error: any) => void;
};

type SignalMutationFunction = (signalName: string, callbacks?: SignalSubscriptionCallbacks) => void;

const NUM_LOD_LEVELS = 10;

class LiveSignalStore extends SignalStore {
    private subscribeToSignal: SignalMutationFunction;
    private unsubscribeFromSignal: SignalMutationFunction;
    private lodBuffers: Map<string, HaarLodBuffer[] | ModeLodBuffer[]>;
    private pendingUnsubscribes: Map<string, ReturnType<typeof setTimeout>>;

    constructor(updateWithTimestamp: (timestamp: number) => void, subscribeToSignal: SignalMutationFunction, unsubscribeFromSignal: SignalMutationFunction) {
        super(updateWithTimestamp);

        this.subscribeToSignal = subscribeToSignal;
        this.unsubscribeFromSignal = unsubscribeFromSignal;
        this.lodBuffers = new Map();
        this.pendingUnsubscribes = new Map();
    }

    /**
     * Starts handling the shared socket's data. Paired with detach() on unmount: the socket outlives the page, so a
     * store that never detached kept receiving every sample after the live page was left and opened again.
     */
    attach() {
        socket.on("data", this.handleData);
        socket.on("connect", this.handleConnect);
    }

    detach() {
        socket.off("data", this.handleData);
        socket.off("connect", this.handleConnect);
    }

    private handleData = (payload: unknown) => {
        const {
            name: signalName,
            timestamp,
            value,
            signal_type,
        } = payload as {
            name: string;
            timestamp: number;
            value: number;
            signal_type: "Numerical" | "Alert" | "Enum" | "Boolean" | "Marker";
        };

        const ts = new Date(timestamp).getTime();

        if (signal_type === "Marker") {
            if (!signalName.endsWith("TelemMarkEvent")) return;

            addTelemetryMarker({
                timestampMs: ts,
            });

            this.updateWithTimestamp(ts);
            return;
        }

        if (!this.storage[signalName] && signal_type !== "Alert") return;

        if (signal_type === "Alert") {
            if (!this.storage[signalName]) {
                this.getOrCreateSignalData({
                    name: signalName,
                    type: SignalType.ALERT,
                    tx_node: "",
                    msg_name: "",
                    id: -1,
                    min_val: 0,
                    max_val: 1,
                    cycle_time_ms: null,
                });
            }

            this.addDataPoint(signalName, ts, value);

            return;
        }

        this.addDataPoint(signalName, ts, value);

        if (signal_type !== "Numerical" && signal_type !== "Enum" && signal_type !== "Boolean") return;

        // samples already in flight when a chart unsubscribed; nothing is charting this signal any more
        const lodBuffer = this.lodBuffers.get(signalName);
        if (!lodBuffer) return;

        const onLodSample = (level: number, _intervalMs: number, timestamp: number, value: number) => {
            this.addDataPointAtLOD(signalName, level, timestamp, value);
        };

        if (signal_type === "Numerical") {
            propagateHaar(lodBuffer as HaarLodBuffer[], 0, ts, value, onLodSample, NUM_LOD_LEVELS);
        } else {
            propagateMode(lodBuffer as ModeLodBuffer[], 0, ts, { [value]: 1 }, onLodSample, NUM_LOD_LEVELS);
        }
    };

    private handleConnect = () => {
        Object.entries(this.subscriberCounts).forEach(([signalName, subscriberCount]) => {
            if (subscriberCount <= 0) return;

            this.subscribeToSignal(signalName, {
                onError: (error) => {
                    this.setError(signalName, error);
                },
            });
        });
    };

    getReferenceToSignal<T extends SignalMetadata>(signal: T) {
        const signalData = this.getOrCreateSignalData(signal);
        this.incrementSubscribers(signal.name);

        if (this.getSubscriberCount(signal.name) !== 1) return signalData.data as any;

        const pendingUnsubscribe = this.pendingUnsubscribes.get(signal.name);
        if (pendingUnsubscribe !== undefined) {
            clearTimeout(pendingUnsubscribe);
            this.pendingUnsubscribes.delete(signal.name);

            return signalData.data as any;
        }

        if (signal.type !== SignalType.ALERT) this.lodBuffers.set(signal.name, new Array(NUM_LOD_LEVELS).fill(null));

        this.subscribeToSignal(signal.name, {
            onError: (error) => {
                this.setError(signal.name, error);
            },
        });

        return signalData.data as any;
    }

    purgeReferenceToSignal<T extends SignalMetadata>(signal: T) {
        const shouldCleanup = this.decrementSubscribers(signal.name);

        if (!shouldCleanup) return;

        const pendingUnsubscribe = setTimeout(() => {
            this.pendingUnsubscribes.delete(signal.name);

            this.markAsUnsubscribed(signal.name);
            this.lodBuffers.delete(signal.name);

            this.unsubscribeFromSignal(signal.name, {
                onSuccess: () => {
                    if (this.getSubscriberCount(signal.name) !== 0) return;

                    this.removeSignal(signal.name);
                },
                onError: (error) => {
                    console.error(`Error unsubscribing from signal ${signal.name}:`, error);
                },
            });
        }, 0);

        this.pendingUnsubscribes.set(signal.name, pendingUnsubscribe);
    }
}

export default LiveSignalStore;
