"use client";

// TODO(evan): All these functions should be split into separate files.
//             I just didn't want to name a file for these stupid global
//             stores.

import { fetchSignalMetadata } from "@/lib/api/signals";
import subscribeToSignal from "@/lib/api/subscribeToSignal";
import unsubscribeFromSignal from "@/lib/api/unsubscribeFromSignal";
import { API_BASE_URL, IS_DEBUG, IS_VERBOSE_DEBUG } from "@/lib/constants";
import socket from "@/lib/realtime/socket";
import SignalDataReducer from "@/lib/types/SignalDataReducer";
import { useEffect, useState } from "react";

type DataSubscriberCallback = (payload: ParsedSignalPayload) => void;

/**
 * Map to hold subscribers for data updates, keyed by signal name.
 */
const dataSubscribers: Map<string, Set<DataSubscriberCallback>> = new Map();

/**
 * Global store for signal data, keyed by signal name.
 */
const signalDataStore: Record<string, ParsedSignalPayload[]> = {};

type SignalDataStore<Reducers extends SignalDataReducer<unknown>> = Record<
  string,
  {
    [Index in keyof Reducers]: Reducers[Index] extends SignalDataReducer<infer T> ? T : never;
  }
>;

// NOTE(evan): Fill the signalDataStore with all of our known signals so that
//             javascript can hopefully optimize access to the object.
const signals = fetchSignalMetadata(API_BASE_URL);

signals
  .then((metadataList) => {
    metadataList.forEach((metadata) => {
      signalDataStore[metadata.name] = [];
    });
  })
  .catch((err) => {
    console.error("Failed to fetch signal metadata:", err);
  });

const subscribeToSignalData = (signalName: string, callback: DataSubscriberCallback) => {
  IS_DEBUG &&
    console.log(
      `%c[Added Signal Data Subscription] %cSignal: ${signalName}`,
      "color: #ebcb8b; font-weight: bold;",
      "color: #d08770;"
    );

  const currentSubscribers = dataSubscribers.get(signalName) || new Set();
  currentSubscribers.add(callback);
  dataSubscribers.set(signalName, currentSubscribers);

  if (currentSubscribers.size > 1) return;

  IS_DEBUG &&
    console.log(
      `%c[Subscribe Signal Data] %cSignal: ${signalName}`,
      "color: #a3be8c; font-weight: bold;",
      "color: #d08770;"
    );

  subscribeToSignal(signalName);
};

const unsubscribeFromSignalData = (signalName: string, callback: DataSubscriberCallback) => {
  IS_DEBUG &&
    console.log(
      `%c[Removed Signal Data Subscription] %cSignal: ${signalName}`,
      "color: #d08770; font-weight: bold;",
      "color: #d08770;"
    );

  const currentSubscribers = dataSubscribers.get(signalName);

  if (!currentSubscribers) return;

  currentSubscribers.delete(callback);

  if (currentSubscribers.size > 0) return;

  IS_DEBUG &&
    console.log(
      `%c[Unsubscribe Signal Data] %cSignal: ${signalName}`,
      "color: #bf616a; font-weight: bold;",
      "color: #d08770;"
    );

  dataSubscribers.delete(signalName);
  unsubscribeFromSignal(signalName);
};

type SignalPayload = {
  value: number;
  name: string;
  time?: string;
};

type ParsedSignalPayload = {
  value: number;
  signal: string;
  time: Date;
};

// TODO(evan): Might want to move this into a zod schema or something later. Depends
//             on how complex this data validation needs to be.
/**
 * Type guard to validate signal data payloads.
 *
 *
 * @param data - The data to validate.
 * @returns True if the data is a valid SignalPayload, false otherwise.
 */
const isValidSignalPayload = (data: unknown): data is SignalPayload => {
  if (data == null || typeof data !== "object" || Array.isArray(data)) {
    return false;
  }

  if (!("value" in data) || !("name" in data)) {
    return false;
  }

  const { value, name } = data;

  if (
    typeof value !== "number" ||
    typeof name !== "string" ||
    ("time" in data && typeof (data as any).time !== "string")
  ) {
    return false;
  }

  return true;
};

const handleData = (data: unknown) => {
  if (!isValidSignalPayload(data)) {
    console.warn("Received invalid signal data:", data);
    return;
  }

  const { name } = data;

  data.time ||= new Date().toISOString();

  IS_VERBOSE_DEBUG &&
    console.groupCollapsed(
      `%c[Handling Signal Data] %cSignal: %s`,
      "color: #81a1c1; font-weight: bold;",
      "color: #d08770;",
      name
    );

  IS_VERBOSE_DEBUG && console.log(`%cData: %o`, "color: #d08770;", data);

  const parsedData: ParsedSignalPayload = {
    value: data.value,
    signal: data.name,
    time: new Date(data.time),
  };

  dataSubscribers.get(name)?.forEach((callback) => {
    callback(parsedData);
  });

  IS_VERBOSE_DEBUG && console.groupEnd();
};

socket.on("data", handleData);

/**
 * Hook to get the data version for a list of signals, used to allow
 * for re-rendering without having to create new arrays which would
 * trigger unnecessary garbage collection.
 *
 * @param signals - Array of signal names.
 */
const useDataVersion = (signals: string[]) => {
  const [dataVersion, setDataVersion] = useState(0);

  useEffect(() => {
    // NOTE(evan): When data for any of the subscribed signals updates, we increment
    //             the data version to trigger a re-render.
    const notify = () => {
      IS_VERBOSE_DEBUG &&
        console.log(
          `%c[Signal Data Update] %cSignals: ${signals.join(", ")}`,
          "color: #88c0d0; font-weight: bold;",
          "color: #d08770;"
        );

      setDataVersion((prev) => prev + 1);
    };

    signals.forEach((signalName) => {
      subscribeToSignalData(signalName, notify);
    });

    return () => {
      signals.forEach((signalName) => {
        unsubscribeFromSignalData(signalName, notify);
      });
    };
  }, [signals]);

  return dataVersion;
};

const useSignalSubscription = (signalNames: string[]) => {
  useEffect(() => {
    const signalCallbacks: Record<string, DataSubscriberCallback> = {};

    signalNames.forEach((signalName) => {
      signalCallbacks[signalName] = (newData) => {
        signalDataStore[signalName].push(newData);
      };

      subscribeToSignalData(signalName, signalCallbacks[signalName]);
    });

    return () => {
      signalNames.forEach((signalName) => {
        unsubscribeFromSignalData(signalName, signalCallbacks[signalName]);
      });
    };
  }, [signalNames]);
};

/**
 * Hook that subscribes to signal data updates and returns a stable reference to the signal's data array.
 *
 * The returned array reference never changes - it's mutated in place when data updates occur.
 * To trigger re-renders on data updates, use the `useDataVersion` hook alongside this.
 *
 * @param signalNames - Array of signal names to subscribe to.
 *
 * @returns A stable reference to the signal's data arrays
 */
const useSignalDataRef = (signalNames: string[]): Record<string, readonly any[]> => {
  useSignalSubscription(signalNames);

  const dataRef: Record<string, readonly any[]> = {};

  signalNames.forEach((signalName) => {
    dataRef[signalName] = signalDataStore[signalName];
  });

  return dataRef;
};

/**
 * TODO(evan): Write some good documentation here
 */
const useSignalDataRefWithReducers = <U, Reducer extends SignalDataReducer<U>>(
  signalNames: string[],
  reducer: Reducer,
  initialValue: U
): U => {
  useSignalSubscription(signalNames);

  let dataRef = initialValue;

  signalNames.forEach((signalName) => {
    subscribeToSignalData(signalName, (newData) => {
      dataRef = reducer(newData, dataRef);
    });
  });

  return dataRef;
};

export type { ParsedSignalPayload };

export { useDataVersion, useSignalDataRef, useSignalDataRefWithReducers };
