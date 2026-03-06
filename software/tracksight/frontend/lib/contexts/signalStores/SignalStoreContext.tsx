"use client";

import { createContext, RefObject, ReactNode, useContext, useEffect, useRef } from "react";
import SignalStore, { SignalStoreReturnType } from "@/lib/signals/SignalStore";
import { WidgetConfigs, WidgetSignalConfig } from "@/components/widgets/WidgetTypes";

const SignalDataStoreContext = createContext<RefObject<SignalStore> | null>(null);

function SignalDataStoreProvider({ children, signalStore }: { children: ReactNode, signalStore: RefObject<SignalStore> }) {
  return (
    <SignalDataStoreContext.Provider
      value={signalStore}
    >
      {children}
    </SignalDataStoreContext.Provider>
  );
};

const useSignalDataStore = <T extends WidgetSignalConfig>(signal: T) => {
  const context = useContext(SignalDataStoreContext);

  if (context === null) {
    throw new Error("useSignalDataStore must be used within a SignalDataStoreProvider");
  }

  const signalStore = context;
  const cachedReferenceRef = useRef<SignalStoreReturnType<T> | null>(null);

  useEffect(() => {
    return () => {
      if (!signalStore.current) return;

      signalStore.current.purgeReferenceToSignal(signal);
    };
  }, [signal, signalStore]);

  useEffect(() => {
    if (!signalStore.current) return;

    cachedReferenceRef.current = signalStore.current.getReferenceToSignal(signal);
  }, [signal, signalStore]);


  return cachedReferenceRef;
}

const useSignalDataStores = <T extends WidgetConfigs>(signals: T) => {
  const context = useContext(SignalDataStoreContext);

  if (context === null) {
    throw new Error("useSignalDataStores must be used within a SignalDataStoreProvider");
  }

  const signalStore = context;
  const cachedReferencesRef = useRef<SignalStoreReturnType<T[number]>[]>([]);

  useEffect(() => {
    return () => {
      if (!signalStore.current) return;

      signals.forEach((signal) => {
        signalStore.current!.purgeReferenceToSignal(signal);
      });
    };
  }, [signals, signalStore]);

  useEffect(() => {
    if (!signalStore.current) return;

    cachedReferencesRef.current = [];
    signals.forEach((signal) => {
      const reference = signalStore.current!.getReferenceToSignal<T[number]>(signal);

      cachedReferencesRef.current.push(reference);
    });
  }, [signals, signalStore]);

  return cachedReferencesRef;
}

export { SignalDataStoreProvider, useSignalDataStore, useSignalDataStores };

