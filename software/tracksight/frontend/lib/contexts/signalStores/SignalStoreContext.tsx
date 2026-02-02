"use client";

import { createContext, RefObject, ReactNode, useContext, useEffect, useRef } from "react";
import SignalStore, { SignalStoreEntry } from "@/lib/signals/SignalStore";

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

const useSignalDataStore = (signalName: string) => {
  const context = useContext(SignalDataStoreContext);

  if (context === null) {
    throw new Error("useSignalDataStore must be used within a SignalDataStoreProvider");
  }

  const signalStore = context;
  const cachedReferenceRef = useRef<SignalStoreEntry | null>(null);

  useEffect(() => {
    return () => {
      if (!signalStore.current) return;

      signalStore.current.purgeReferenceToSignal(signalName);
    };
  }, [signalName, signalStore]);

  useEffect(() => {
    if (!signalStore.current) return;

    cachedReferenceRef.current = signalStore.current.getReferenceToSignal(signalName);
  }, [signalName, signalStore]);


  return cachedReferenceRef;
}

const useSignalDataStores = (signalNames: string[]) => {
  const context = useContext(SignalDataStoreContext);

  if (context === null) {
    throw new Error("useSignalDataStores must be used within a SignalDataStoreProvider");
  }

  const signalStore = context;
  const cachedReferencesRef = useRef<Map<string, SignalStoreEntry>>(new Map());

  useEffect(() => {
    return () => {
      if (!signalStore.current) return;

      signalNames.forEach((signalName) => {
        signalStore.current!.purgeReferenceToSignal(signalName);
      });
    };
  }, [signalNames, signalStore]);

  useEffect(() => {
    if (!signalStore.current) return;

    signalNames.forEach((signalName) => {
      if (!cachedReferencesRef.current.has(signalName)) {
        const reference = signalStore.current!.getReferenceToSignal(signalName);
        cachedReferencesRef.current.set(signalName, reference);
      }
    });
  }, [signalNames, signalStore]);

  return cachedReferencesRef;
}

export { SignalDataStoreProvider, useSignalDataStore, useSignalDataStores };

