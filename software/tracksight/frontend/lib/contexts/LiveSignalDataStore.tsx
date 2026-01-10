import { CircularBuffer, SignalDataStoreProvider, type GenericSignalStore } from "@/lib/contexts/SignalDataStore";
import socket from "@/lib/realtime/socket";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import React, { memo, useRef } from "react";

const MAX_ELEMENTS_IN_BUFFER = 5000;

class LiveSignalDataStore implements GenericSignalStore {
  private subscriberCounts: { [signalName: string]: number };
  private liveData: { [signalName: string]: ReturnType<GenericSignalStore['getReferenceToSignal']> };

  private subscribeToSignal: ReturnType<typeof useSubscribeToSignal> | null = null;
  private unsubscribeFromSignal: ReturnType<typeof useUnsubscribeToSignal> | null = null;

  // NOTE(evan): Might be unnecessary but it stops the double rendering creating multiple instances
  private static instance: LiveSignalDataStore | null = null;

  constructor() {
    LiveSignalDataStore.instance = this;

    this.subscriberCounts = {};
    this.liveData = {};

    socket.on("data", (payload) => {
      const { name: signalName, timestamp, value } = payload;

      if (!this.liveData[signalName]) {
        console.error(`Received data for unsubscribed signal: ${signalName}`);

        return;
      }

      this.liveData[signalName].data.timePoints.push(new Date(timestamp).getTime());
      this.liveData[signalName].data.values.push(value);
    });
  }

  static getInstance() {
    if (!LiveSignalDataStore.instance) {
      LiveSignalDataStore.instance = new LiveSignalDataStore();
    }

    return LiveSignalDataStore.instance;
  }

  updateMutations(subscribeToSignal: ReturnType<typeof useSubscribeToSignal>, unsubscribeFromSignal: ReturnType<typeof useUnsubscribeToSignal>) {
    this.subscribeToSignal = subscribeToSignal;
    this.unsubscribeFromSignal = unsubscribeFromSignal;
  }

  getReferenceToSignal(signalName: string) {
    if (!this.liveData[signalName]) {
      this.liveData[signalName] = {
        data: {
          timePoints: new CircularBuffer(MAX_ELEMENTS_IN_BUFFER),
          values: new CircularBuffer(MAX_ELEMENTS_IN_BUFFER),
        },
        error: null,
        isSubscribed: true,
      }

      this.subscriberCounts[signalName] = 0;

      this.subscribeToSignal?.mutate(signalName, {
        onError: (error) => {
          if (!this.liveData[signalName]) return;

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

    if (this.liveData[signalName]) {
      this.liveData[signalName].isSubscribed = false;
    }

    this.unsubscribeFromSignal?.mutate(signalName, {
      onSuccess: () => {
        if (this.subscriberCounts[signalName] !== 0) return;

        delete this.liveData[signalName];
        delete this.subscriberCounts[signalName];
      },
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signalName}:`, error);
        if (this.subscriberCounts[signalName] !== 0) return;

        this.subscriberCounts[signalName] = 1;
      }
    });
  }
}

const LiveSignalDataStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
  const subscribeToSignalMutation = useSubscribeToSignal();
  const unsubscribeFromSignalMutation = useUnsubscribeToSignal();

  const liveSignalStore = useRef(LiveSignalDataStore.getInstance());

  liveSignalStore.current.updateMutations(subscribeToSignalMutation, unsubscribeFromSignalMutation);

  return (
    <SignalDataStoreProvider signalStore={liveSignalStore}>
      {children}
    </SignalDataStoreProvider>
  );
});

export { LiveSignalDataStoreProvider };


