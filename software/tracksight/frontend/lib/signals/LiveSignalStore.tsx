import SignalStore, { SignalStoreReturnType } from "@/lib/signals/SignalStore";
import socket from "@/lib/realtime/socket";
import { SignalMetadata } from "../types/Signal";

type SignalSubscriptionCallbacks = {
  onSuccess?: () => void;
  onError?: (error: any) => void;
};

type SignalMutationFunction = (signalName: string, callbacks?: SignalSubscriptionCallbacks) => void;

class LiveSignalStore extends SignalStore {
  private subscribeToSignal: SignalMutationFunction
  private unsubscribeFromSignal: SignalMutationFunction

  constructor(
    updateWithTimestamp: (timestamp: number) => void,
    subscribeToSignal: SignalMutationFunction,
    unsubscribeFromSignal: SignalMutationFunction
  ) {
    super(updateWithTimestamp);

    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;

    socket.on("data", (payload) => {
      const { name: signalName, timestamp, value } = payload;

      const signalData = this.getSignalData(signalName);
      if (!signalData) return;

      this.addDataPoint(signalName, new Date(timestamp).getTime(), value);
    });
  }

  // FIXME(evan): Type stuff I can't be bothered to do right now
  getReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.name);

    if (this.getSubscriberCount(signal.name) !== 1) return signalData.data as any;

    this.subscribeToSignal(signal.name, {
      onError: (error) => {
        this.setError(signal.name, error);
      }
    });

    return signalData.data as any;
  }

  purgeReferenceToSignal<T extends SignalMetadata>(signal: T) {
    const shouldCleanup = this.decrementSubscribers(signal.name);

    if (!shouldCleanup) return;

    this.markAsUnsubscribed(signal.name);

    this.unsubscribeFromSignal(signal.name, {
      onSuccess: () => {
        if (this.getSubscriberCount(signal.name) !== 0) return;

        this.removeSignal(signal.name);
      },
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signal.name}:`, error);
      }
    });
  }
}

export default LiveSignalStore;


