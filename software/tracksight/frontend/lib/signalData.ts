"use client";

// TODO(evan): All these functions should be split into separate files.
//             I just didn't want to name a file for these stupid global
//             stores.

import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL, IS_DEBUG, IS_VERBOSE_DEBUG } from "@/lib/constants";
import socket from "@/lib/realtime/socket";
import subscribeToSignal from "@/lib/api/subscribeToSignal";
import unsubscribeFromSignal from "@/lib/api/unsubscribeFromSignal";
import { useEffect, useState } from "react";
import { sign } from "crypto";

/**
 * Map to hold subscribers for data updates, keyed by signal name.
 */
const dataSubscribers: Map<string, Set<() => void>> = new Map();

/**
 * Map to hold number of subscribers for signal data updates, keyed by signal name.
 * When the count reaches zero, we stop fetching data for that signal to save resources.
 */
const signalDataSubscribers = new Map<string, number>();

/**
 * Global store for signal data, keyed by signal name.
 */
const signalDataStore: Record<string, any[]> = {};

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

const subscribeToSignalData = (signalName: string) => {
  IS_DEBUG &&
    console.log(
      `%c[Added Signal Data Subscription] %cSignal: ${signalName}`,
      "color: #ebcb8b; font-weight: bold;",
      "color: #d08770;"
    );

  const currentCount = signalDataSubscribers.get(signalName) || 0;
  signalDataSubscribers.set(signalName, currentCount + 1);

  if (currentCount !== 0) return;

  IS_DEBUG &&
    console.log(
      `%c[Subscribe Signal Data] %cSignal: ${signalName}`,
      "color: #a3be8c; font-weight: bold;",
      "color: #d08770;"
    );

  subscribeToSignal(signalName);
};

const unsubscribeFromSignalData = (signalName: string) => {
  IS_DEBUG &&
    console.log(
      `%c[Removed Signal Data Subscription] %cSignal: ${signalName}`,
      "color: #d08770; font-weight: bold;",
      "color: #d08770;"
    );

  const currentCount = signalDataSubscribers.get(signalName) || 0;

  if (currentCount > 1) {
    signalDataSubscribers.set(signalName, currentCount - 1);

    return;
  }

  IS_DEBUG &&
    console.log(
      `%c[Unsubscribe Signal Data] %cSignal: ${signalName}`,
      "color: #bf616a; font-weight: bold;",
      "color: #d08770;"
    );

  signalDataSubscribers.delete(signalName);
  unsubscribeFromSignal(signalName);
};

type SignalPayload = {
  value: number;
  name: string;
  time?: string;
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

  if (
    !("value" in data)
    || !("name" in data)
  ) {
    return false;
  }

  const {
    value,
    name
  } = data;

  if (
    typeof value !== "number"
    || typeof name !== "string"
    || ("time" in data && typeof (data as any).time !== "string")
  ) {
    return false;
  }

  return true;
};

const handleData = (
  data: unknown,
) => {
  if (!isValidSignalPayload(data)) {
    console.warn("Received invalid signal data:", data);
    return;
  }

  const { name } = data;


  data.time ||= new Date().toISOString();
  
  IS_VERBOSE_DEBUG && console.groupCollapsed(
    `%c[Handling Signal Data] %cSignal: %s`,
    "color: #81a1c1; font-weight: bold;",
    "color: #d08770;",
    name
  )

  IS_VERBOSE_DEBUG && console.log(
    `%cData: %o`,
    "color: #d08770;",
    data
  );
  
  signalDataStore[name].push(data);

  dataSubscribers.get(name)?.forEach((callback) => {
    callback();
  }); 

  IS_VERBOSE_DEBUG && console.groupEnd();
}

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
      if (!dataSubscribers.has(signalName)) {
        dataSubscribers.set(signalName, new Set());
      }

      dataSubscribers.get(signalName)!.add(notify);
    });

    return () => {
      signals.forEach((signalName) => {
        dataSubscribers.get(signalName)?.delete(notify);
      });
    };
  }, [signals]);

  return dataVersion;
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
  useEffect(() => {
    signalNames.forEach(subscribeToSignalData);
  
    return () => {
      signalNames.forEach(unsubscribeFromSignalData);
    };
  }, [signalNames]);

  const dataRef: Record<string, readonly any[]> = {};

  signalNames.forEach((signalName) => {
    dataRef[signalName] = signalDataStore[signalName];
  });

  return dataRef;
};

export { useDataVersion, useSignalDataRef };
