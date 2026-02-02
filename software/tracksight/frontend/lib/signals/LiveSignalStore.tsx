import SignalStore from "@/lib/signals/SignalStore";
import socket from "@/lib/realtime/socket";

type SignalSubscriptionCallbacks = {
  onSuccess?: () => void;
  onError?: (error: any) => void;
};

type SignalMutationFunction = (signalName: string, callbacks?: SignalSubscriptionCallbacks) => void;

class LiveSignalStore extends SignalStore {
  private subscribeToSignal: SignalMutationFunction
  private unsubscribeFromSignal: SignalMutationFunction

  constructor(
    subscribeToSignal: SignalMutationFunction,
    unsubscribeFromSignal: SignalMutationFunction
  ) {
    super();

    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;

    socket.on("data", (payload) => {
      const { name: signalName, timestamp, value } = payload;

      const signalData = this.getSignalData(signalName);
      if (!signalData) return;

      this.addDataPoint(signalName, new Date(timestamp).getTime(), value);
    });
  }

  getReferenceToSignal(signalName: string) {
    const signalData = this.getOrCreateSignalData(signalName);
    this.incrementSubscribers(signalName);

    if (this.getSubscriberCount(signalName) !== 1) return signalData;

    this.subscribeToSignal(signalName, {
      onError: (error) => {
        this.setError(signalName, error);
      }
    });

    return signalData;
  }

  purgeReferenceToSignal(signalName: string) {
    const shouldCleanup = this.decrementSubscribers(signalName);

    if (!shouldCleanup) return;

    this.markAsUnsubscribed(signalName);

    this.unsubscribeFromSignal(signalName, {
      onSuccess: () => {
        if (this.getSubscriberCount(signalName) !== 0) return;

        this.removeSignal(signalName);
      },
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signalName}:`, error);
      }
    });
  }
}

export default LiveSignalStore;



