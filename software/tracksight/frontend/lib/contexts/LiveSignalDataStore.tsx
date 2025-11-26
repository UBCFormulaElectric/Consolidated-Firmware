import { SignalDataStoreProvider, type GenericSignalStore } from "./SignalDataStore";
import socket from "../realtime/socket";
import useUnsubscribeToSignal from "../mutations/useUnsubscribeToSignal";
import useSubscribeToSignal from "../mutations/useSubscribeToSignal";
import { useRef } from "react";

type SignalSubscriptionFn = (signalName: string, options?: { onSuccess?: () => void, onError: (error: unknown) => void }) => void;

class LiveSignalDataStore implements GenericSignalStore {
  private subscriberCounts: { [signalName: string]: number };
  private liveData: { [signalName: string]: ReturnType<GenericSignalStore['getReferenceToSignal']> };

  private subscribeToSignal: SignalSubscriptionFn;
  private unsubscribeFromSignal: SignalSubscriptionFn;

  constructor(subscribeToSignal: SignalSubscriptionFn, unsubscribeFromSignal: SignalSubscriptionFn) {
    this.subscriberCounts = {};
    this.liveData = {};

    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;

    socket.on("data", (payload) => {
      const { name: signalName, time, value } = payload;

      if (!this.liveData[signalName]) {
        console.error(`Received data for unsubscribed signal: ${signalName}`);

        return;
      }

      this.liveData[signalName].data.push(payload);
    });
  }

  getReferenceToSignal(signalName: string) {
    if (!this.liveData[signalName]) {
      this.liveData[signalName] = {
        data: [],
        error: null,
        isSubscribed: false,
      }

      this.subscriberCounts[signalName] = 0;

      this.subscribeToSignal(signalName, {
        onSuccess: () => {
          this.liveData[signalName].isSubscribed = true;
        },
        onError: (error) => {
          this.liveData[signalName].error = error;
        }
      });
    }

    this.subscriberCounts[signalName] += 1;

    return this.liveData[signalName];
  }

  purgeReferenceToSignal(signalName: string) {
    if (!this.subscriberCounts[signalName]) return;

    this.subscriberCounts[signalName] -= 1;

    if (this.subscriberCounts[signalName] > 0) return;

    this.unsubscribeFromSignal(signalName, {
      onSuccess: () => {
        delete this.liveData[signalName];
        delete this.subscriberCounts[signalName];
      },
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signalName}:`, error);
        this.subscriberCounts[signalName] = 1;
      }
    });
  }
}

const LiveSignalDataStoreProvider = ({ children }: { children: React.ReactNode }) => {
  const subscribeToSignalMutation = useSubscribeToSignal();
  const unsubscribeFromSignalMutation = useUnsubscribeToSignal();

  const liveSignalStore = useRef(new LiveSignalDataStore(
    subscribeToSignalMutation.mutate,
    unsubscribeFromSignalMutation.mutate
  ));

  return (
    <SignalDataStoreProvider signalStore={liveSignalStore}>
      {children}
    </SignalDataStoreProvider>
  );
};

export { LiveSignalDataStoreProvider };


