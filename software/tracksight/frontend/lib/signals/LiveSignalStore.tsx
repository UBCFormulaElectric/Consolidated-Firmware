import SignalStore from "@/lib/signals/SignalStore";
import socket from "@/lib/realtime/socket";
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

  constructor(
    updateWithTimestamp: (timestamp: number) => void,
    subscribeToSignal: SignalMutationFunction,
    unsubscribeFromSignal: SignalMutationFunction
  ) {
    super(updateWithTimestamp);

    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;
    this.lodBuffers = new Map();

    socket.on("data", (payload) => {
      const { name: signalName, timestamp, value, signal_type } = payload as {
        name: string;
        timestamp: number;
        value: number;
        signal_type: "Numerical" | "Alert" | "Enum" | "Boolean";
      };

      if (!this.storage[signalName] && signal_type !== "Alert") return;

      const ts = new Date(timestamp).getTime();

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

      const lodBuffer = this.lodBuffers.get(signalName);

      if (!lodBuffer) {
        throw new Error(`Received data for signal ${signalName} which is not initialized in lodBuffers`);
      }

      const onLodSample = (level: number, intervalMs: number, timestamp: number, value: number) => {
        this.addDataPointAtLOD(signalName, level, intervalMs, timestamp, value);
      };

      if (signal_type === "Numerical") {
        propagateHaar(lodBuffer as HaarLodBuffer[], 0, ts, value, onLodSample, NUM_LOD_LEVELS);
      } else {
        propagateMode(lodBuffer as ModeLodBuffer[], 0, ts, { [value]: 1 }, onLodSample, NUM_LOD_LEVELS);
      }
    });

    socket.on("connect", () => {
      Object.entries(this.subscriberCounts).forEach(([signalName, subscriberCount]) => {
        if (subscriberCount <= 0) return;

        this.subscribeToSignal(signalName, {
          onError: (error) => {
            this.setError(signalName, error);
          },
        });
      });
    });
  }

  getReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.name);

    if (this.getSubscriberCount(signal.name) !== 1) return signalData.data as any;

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
  }
}

export default LiveSignalStore;
