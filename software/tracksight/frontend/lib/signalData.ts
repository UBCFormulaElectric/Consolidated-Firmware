import { IS_VERBOSE_DEBUG } from "@/lib/constants";
import useSubscribeToSignal from "@/lib/mutations/useSubscribeToSignal";
import useUnsubscribeToSignal from "@/lib/mutations/useUnsubscribeToSignal";
import socket from "@/lib/realtime/socket";
import SignalDataReducer from "@/lib/types/SignalDataReducer";
import { useEffect } from "react";

type DataSubscriberCallback = (payload: ParsedSignalPayload) => void;

/**
 * Map to hold subscribers for data updates, keyed by signal name.
 */
const dataSubscribers: Map<string, Set<DataSubscriberCallback>> = new Map();

/**
 * Global store for signal data, keyed by signal name.
 */
const signalDataStore: Record<string, ParsedSignalPayload[]> = {};

const registerSignalDataCallback = (signalName: string, callback: DataSubscriberCallback) => {
  const currentSubscribers = dataSubscribers.get(signalName) || new Set();
  currentSubscribers.add(callback);
  dataSubscribers.set(signalName, currentSubscribers);

  if (currentSubscribers.size > 1) return;
};

const unregisterSignalDataCallback = (signalName: string, callback: DataSubscriberCallback) => {
  const currentSubscribers = dataSubscribers.get(signalName);

  if (!currentSubscribers) return;

  currentSubscribers.delete(callback);

  if (currentSubscribers.size > 0) return;

  dataSubscribers.delete(signalName);
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

const useSignalSubscription = (signalNames: string[]) => {
  const subscribeMutation = useSubscribeToSignal();
  const unsubscribeMutaiton = useUnsubscribeToSignal();

  useEffect(() => {
    const signalCallbacks: Record<string, DataSubscriberCallback> = {};

    signalNames.forEach((signalName) => {
      signalCallbacks[signalName] = (newData) => {
        signalDataStore[signalName].push(newData);
      };

      subscribeMutation.mutate(signalName, {
        onSuccess: () => {
          registerSignalDataCallback(signalName, signalCallbacks[signalName]);
        },
      });
    });

    return () => {
      signalNames.forEach((signalName) => {
        unsubscribeMutaiton.mutate(signalName, {
          onSuccess: () => {
            unregisterSignalDataCallback(signalName, signalCallbacks[signalName]);
          },
          // TODO(evan): Present an error to the user if unsubscription fails because the component
          //             is unmounting and they won't see it in the UI.
        });
      });
    };
  }, [signalNames]);

  return {
    subscriptionStatus: subscribeMutation.status,
    subscribeError: subscribeMutation.error,

    unsubscriptionStatus: unsubscribeMutaiton.status,
    unsubscribeError: unsubscribeMutaiton.error,
  };
};

/**
 * Hook that subscribes to signal data updates and returns a stable reference to the signal's data array.
 *
 * The returned array reference never changes - it's mutated in place when data updates occur.
 *
 * @param signalNames - Array of signal names to subscribe to.
 *
 * @returns An object containing subscription status, errors, and the data reference.
 */
const useSignalDataRef = (signalNames: string[]) => {
  const { subscriptionStatus, subscribeError } = useSignalSubscription(signalNames);

  const dataRef: Record<string, readonly any[]> = {};

  signalNames.forEach((signalName) => {
    dataRef[signalName] = signalDataStore[signalName];
  });

  return {
    subscriptionStatus,
    subscribeError,

    dataRef,
  } as const;
};

/**
 * Hook that subscribes to signal data updates and applies a reducer function to accumulate data.
 *
 * The returned array reference never changes - it's mutated in place when data updates occur.
 *
 * @param signalNames - Array of signal names to subscribe to.
 * @param reducer - Reducer function to apply to incoming data.
 * @param initialValue - Initial value for the reducer.
 * @returns An object containing subscription status, errors, and the accumulated data reference.
 */
const useSignalDataRefWithReducers = <U, Reducer extends SignalDataReducer<U>>(
  signalNames: string[],
  reducer: Reducer,
  initialValue: U
) => {
  const { subscriptionStatus, subscribeError } = useSignalSubscription(signalNames);

  let dataRef = initialValue;

  useEffect(() => {
    const reducerCallback = (newData: ParsedSignalPayload) => {
      dataRef = reducer(newData, dataRef);
    };

    signalNames.forEach((signalName) => {
      registerSignalDataCallback(signalName, reducerCallback);
    });

    return () => {
      signalNames.forEach((signalName) => {
        unregisterSignalDataCallback(signalName, reducerCallback);
        // TODO(evan): Present an error to the user if unsubscription fails because the component
        //             is unmounting and they won't see it in the UI.
      });
    };
  }, []);

  return {
    subscriptionStatus,
    subscribeError,

    dataRef,
  } as const;
};

export type { ParsedSignalPayload };

export { useSignalDataRef, useSignalDataRefWithReducers };
