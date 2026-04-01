import SignalStore from "@/lib/signals/SignalStore";
import socket from "@/lib/realtime/socket";
import { SignalMetadata, SignalType } from "../types/Signal";
import propagateHaar from "../utils/propagateHaar";

type SignalSubscriptionCallbacks = {
  onSuccess?: () => void;
  onError?: (error: any) => void;
};

type SignalMutationFunction = (signalName: string, callbacks?: SignalSubscriptionCallbacks) => void;

const NUM_LOD_LEVELS = 10;

class LiveSignalStore extends SignalStore {
  private subscribeToSignal: SignalMutationFunction;
  private unsubscribeFromSignal: SignalMutationFunction;
  private waveletBuffers: Map<string, Array<{
    timestamp: number;
    value: number;
  }[] | null>>;

  constructor(
    updateWithTimestamp: (timestamp: number) => void,
    subscribeToSignal: SignalMutationFunction,
    unsubscribeFromSignal: SignalMutationFunction
  ) {
    super(updateWithTimestamp);

    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;
    this.waveletBuffers = new Map();

    socket.on("data", (payload) => {
      const { name: signalName, timestamp, value } = payload;

      if (!this.storage[signalName]) return;

      const ts = new Date(timestamp).getTime();
      this.addDataPointAtLOD(signalName, 0, 1, ts, value);

      if (this.storage[signalName].storeType === SignalType.NUMERICAL) {
        propagateHaar(
          this.waveletBuffers.get(signalName) || [],
          0,
          ts,
          value,
          (level, intervalMs, timestamp, value) => {
            this.addDataPointAtLOD(signalName, level, intervalMs, timestamp, value);
          },
          NUM_LOD_LEVELS
        )
      }
    });
  }

  getReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.name);

    if (this.getSubscriberCount(signal.name) !== 1) return signalData.data as any;

    if (signal.type === SignalType.NUMERICAL) {
      this.waveletBuffers.set(signal.name, new Array(NUM_LOD_LEVELS).fill(null));
    }

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
    this.waveletBuffers.delete(signal.name);

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
