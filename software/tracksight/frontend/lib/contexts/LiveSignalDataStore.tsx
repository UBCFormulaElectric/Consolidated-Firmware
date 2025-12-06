import { CircularBuffer, SignalDataStoreProvider, type GenericSignalStore } from "@/lib/contexts/SignalDataStore";
import socket from "@/lib/realtime/socket";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import React, { memo, useRef } from "react";

const MAX_ELEMENTS_IN_BUFFER = 500000;

class LiveSignalDataStore implements GenericSignalStore {
  private subscriberCounts: { [signalName: string]: number };
  private liveData: { [signalName: string]: ReturnType<GenericSignalStore['getReferenceToSignal']> };

  private subscribeToSignal: ReturnType<typeof useSubscribeToSignal>;
  private unsubscribeFromSignal: ReturnType<typeof useUnsubscribeToSignal>;

  // NOTE(evan): Might be unnecessary but it stops the double rendering creating multiple instances
  private static instance: LiveSignalDataStore | null = null;

  constructor(subscribeToSignal: ReturnType<typeof useSubscribeToSignal>, unsubscribeFromSignal: ReturnType<typeof useUnsubscribeToSignal>) {
    LiveSignalDataStore.instance = this;

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

      this.liveData[signalName].data.timePoints.push(time);
      this.liveData[signalName].data.values.push(value);
    });
  }

  static getInstance(subscribeToSignal: ReturnType<typeof useSubscribeToSignal>, unsubscribeFromSignal: ReturnType<typeof useUnsubscribeToSignal>) {
    if (!LiveSignalDataStore.instance) {
      LiveSignalDataStore.instance = new LiveSignalDataStore(subscribeToSignal, unsubscribeFromSignal);
    }

    return LiveSignalDataStore.instance;
  }

  getReferenceToSignal(signalName: string) {
    if (!this.liveData[signalName]) {
      this.liveData[signalName] = {
        data: {
          timePoints: new CircularBuffer(MAX_ELEMENTS_IN_BUFFER),
          values: new CircularBuffer(MAX_ELEMENTS_IN_BUFFER),
        },
        error: null,
        isSubscribed: false,
      }

      this.subscriberCounts[signalName] = 0;

      this.subscribeToSignal.mutate(signalName, {
        onError: (error) => {
          this.liveData[signalName].error = error;
        }
      });

      if (!this.liveData[signalName].error) {
        this.liveData[signalName].isSubscribed = true;
      }
    }

    this.subscriberCounts[signalName] += 1;

    return this.liveData[signalName];
  }

  purgeReferenceToSignal(signalName: string) {
    if (!this.subscriberCounts[signalName]) return;

    this.subscriberCounts[signalName] -= 1;

    if (this.subscriberCounts[signalName] > 0) return;

    this.unsubscribeFromSignal.mutate(signalName, {
      onError: (error) => {
        console.error(`Error unsubscribing from signal ${signalName}:`, error);
        this.subscriberCounts[signalName] = 1;
      }
    });

    if (this.subscriberCounts[signalName] !== 0) return;

    delete this.liveData[signalName];
    delete this.subscriberCounts[signalName];
  }
}

const LiveSignalDataStoreProvider = memo(({ children }: { children: React.ReactNode }) => {
  const subscribeToSignalMutation = useSubscribeToSignal();
  const unsubscribeFromSignalMutation = useUnsubscribeToSignal();

  console.log("Creating LiveSignalDataStoreProvider");

  const liveSignalStore = useRef(LiveSignalDataStore.getInstance(
    subscribeToSignalMutation,
    unsubscribeFromSignalMutation
  ));

  return (
    <SignalDataStoreProvider signalStore={liveSignalStore}>
      {children}
    </SignalDataStoreProvider>
  );
});

export { LiveSignalDataStoreProvider };


