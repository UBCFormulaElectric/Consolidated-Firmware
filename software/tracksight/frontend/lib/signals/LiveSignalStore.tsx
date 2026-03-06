import SignalStore from "@/lib/signals/SignalStore";
import socket from "@/lib/realtime/socket";
import { WidgetSignalConfig } from "@/components/widgets/WidgetTypes";

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

  getReferenceToSignal<T extends WidgetSignalConfig>(signal: T) {
    const signalData = this.getOrCreateSignalData(signal);
    this.incrementSubscribers(signal.signal_name);

    if (this.getSubscriberCount(signal.signal_name) !== 1) return signalData.data;

    this.subscribeToSignal(signal.signal_name, {
      onError: (error) => {
        this.setError(signal.signal_name, error);
      }
    });

    return signalData.data;
  }

  purgeReferenceToSignal<T extends WidgetSignalConfig>(signal: T) {
    const shouldCleanup = this.decrementSubscribers(signal.signal_name);

    if (!shouldCleanup) return;

    this.markAsUnsubscribed(signal.signal_name);

    this.unsubscribeFromSignal(signal.signal_name, {
      onSuccess: () => {
        if (this.getSubscriberCount(signal.signal_name) !== 0) return;

        this.removeSignal(signal.signal_name);
      },
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signal.signal_name}:`, error);
      }
    });
  }
}

export default LiveSignalStore;


