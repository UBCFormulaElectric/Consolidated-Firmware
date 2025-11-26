"use client";

import { createContext, RefObject, ReactNode, useContext, useEffect } from "react";

interface GenericSignalStore {
  getReferenceToSignal: (signalName: string) => {
    data: {
      time: number;
      value: number;
    }[];
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

  useEffect(() => {
    return () => {
      signalStore.current.purgeReferenceToSignal(signalName);
    };
  }, [signalName, signalStore]);

  return signalStore.current.getReferenceToSignal(signalName);
}

export type { GenericSignalStore };

export { SignalDataStoreProvider, useSignalDataStore };

