"use client";

import { createContext, RefObject, ReactNode, useContext, useEffect, useRef } from "react";

export class CircularBuffer {
  private buffer: Uint32Array;

  private head: number = 0;
  private length: number = 0;

  constructor(size: number) {
    this.buffer = new Uint32Array(size);
  }

  size() {
    return this.length;
  }

  push(value: number) {
    const writeIndex = (this.head + this.length) % this.buffer.length;
    this.buffer[writeIndex] = value;

    if (this.length < this.buffer.length) {
      this.length++;
      return;
    }

    this.head = (this.head + 1) % this.buffer.length;
  }

  get(index: number) {
    if (index < 0 || index >= this.length) {
      return undefined;
    }

    const bufferIndex = (this.head + index) % this.buffer.length;
    return this.buffer[bufferIndex];
  }
}

interface GenericSignalStore {
  getReferenceToSignal: (signalName: string) => {
    data: {
      timePoints: CircularBuffer;
      values: CircularBuffer;
    },
    error: unknown | null;
    isSubscribed: boolean;
  };

  purgeReferenceToSignal: (signalName: string) => void;
};

type SignalDataStoreType = {
  signalStore: RefObject<GenericSignalStore>;
};

const SignalDataStoreContext = createContext<SignalDataStoreType | null>(null);

function SignalDataStoreProvider({ children, signalStore }: { children: ReactNode, signalStore: SignalDataStoreType['signalStore'] }) {
  return (
    <SignalDataStoreContext.Provider
      value={{ signalStore }}
    >
      {children}
    </SignalDataStoreContext.Provider>
  );
};

const useSignalDataStore = (signalName: string) => {
  const context = useContext(SignalDataStoreContext);

  if (context === null) {
    throw new Error("useSignalDataStore must be used within a SignalDataStoreProvider");
  }

  const { signalStore } = context;
  const cachedReferenceRef = useRef<ReturnType<GenericSignalStore['getReferenceToSignal']> | null>(null);

  useEffect(() => {
    return () => {
      if (!signalStore.current) return;

      signalStore.current.purgeReferenceToSignal(signalName);
    };
  }, [signalName, signalStore.current]);

  useEffect(() => {
    if (!signalStore.current) return;

    cachedReferenceRef.current = signalStore.current.getReferenceToSignal(signalName);
  }, [signalName, signalStore.current]);


  return cachedReferenceRef;
}

export type { GenericSignalStore };

export { SignalDataStoreProvider, useSignalDataStore };

