"use client";

import React, { createContext, useContext, useState, useEffect, ReactNode } from "react";
import { SignalMetadata, SignalMetadataMap } from "@/lib/types/Signal";
import { fetchSignalMetadata } from "../api/signals";

type SignalsMetadataContextType = {
  signalMetadata: SignalMetadataMap;

  isLoading: boolean;
  error: Error | null;

  getSignalMetadata: (signalName: string) => SignalMetadata | undefined;

  refetch: () => Promise<void>;
};

const SignalsMetadataContext = createContext<SignalsMetadataContextType | undefined>(undefined);

type SignalsProviderProps = {
  children: ReactNode;
  apiBaseUrl: string;
  initialData?: SignalMetadata[];
};

const convertMetadataToMap = (data: SignalMetadata[] | undefined): SignalMetadataMap => {
  const map = new Map<string, SignalMetadata>();

  if (!data) return map;

  data.forEach((signal) => {
    map.set(signal.name, signal);
  });

  return map;
};

const SignalsMetadataProvider = ({ children, apiBaseUrl, initialData }: SignalsProviderProps) => {
  const initialMap = React.useMemo(() => convertMetadataToMap(initialData), [initialData]);

  const [signalMetadata, setSignalMetadata] = useState<SignalMetadataMap>(initialMap);
  const [isLoading, setIsLoading] = useState(!initialData);
  const [error, setError] = useState<Error | null>(null);

  const fetchSignals = React.useCallback(async () => {
    try {
      setIsLoading(true);
      setError(null);

      const newSignalMetadata = await fetchSignalMetadata(apiBaseUrl);
      const signalMap = convertMetadataToMap(newSignalMetadata);

      setSignalMetadata(signalMap);
    } catch (err) {
      if (!(err instanceof Error)) {
        throw err;
      }

      setError(err);
    } finally {
      setIsLoading(false);
    }
  }, [apiBaseUrl]);

  useEffect(() => {
    if (initialData) return;

    fetchSignals();
  }, [fetchSignals, initialData]);

  const getSignalMetadata = React.useCallback(
    (signalName: string): SignalMetadata | undefined => {
      return signalMetadata.get(signalName);
    },
    [signalMetadata]
  );

  return (
    <SignalsMetadataContext.Provider
      value={{
        signalMetadata,

        isLoading,
        error,

        getSignalMetadata,

        refetch: fetchSignals,
      }}
    >
      {children}
    </SignalsMetadataContext.Provider>
  );
};

const useSignalsMetadataList = () => {
  const context = useContext(SignalsMetadataContext);

  if (context === undefined) {
    throw new Error("useSignals must be used within a SignalsProvider");
  }

  return context;
};

const useSignalMetadata = (signal: string) => {
  const context = useContext(SignalsMetadataContext);

  if (context === undefined) {
    throw new Error("useSignalMetadata must be used within a SignalsProvider");
  }

  return context.getSignalMetadata(signal);
};

export { SignalsMetadataProvider, useSignalsMetadataList, useSignalMetadata };
