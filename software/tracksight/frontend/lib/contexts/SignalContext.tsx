// Centralized subscription management - single effect to handle all subscription changes
// TODO unsubscribe all signals for sockets which are disconnected on backend
// TODO all alerts must be broadcast to all listeners

"use client";

import { useDisplayControl } from "@/components/PausePlayControl";
import { BACKEND_URL, DataPoint, DEBUG, SignalType } from "@/lib/SignalConfig";
import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useRef,
  useState,
  useSyncExternalStore,
} from "react";

import { useSignalData } from "@/lib/hooks/signals/useSignalData";
import { useSocket } from "@/lib/hooks/signals/useSocket";
import { useSubscribers } from "@/lib/hooks/signals/useSubscribers";

// TODO move this to the specific components which need signals
// function useAvailableSignals() {
//   return useQuery({
//     queryKey: ["availableSignals"],
//     queryFn: async () => {
//       if (DEBUG) console.log("Fetching signal metadata");
//       const res = await fetch(`${BACKEND_URL}/signal`);
//       if (!res.ok) throw new Error(`Failed to fetch signals: ${res.status}`);
//       const list = await res.json();

//       const metas: SignalMeta[] = list.map((s: any) => ({
//         name: s.name,
//         unit: s.unit,
//         cycle_time_ms: s.cycle_time_ms,
//         msg_name: s.msg_name,
//         msg_id: s.id,
//         enum: s.enum
//       }));

//       return metas;
//     },
//     retry: MAX_RECONNECT_ATTEMPTS,
//   })
// }

function handleData(
  data: any,
  isSubscribed: (signal_name: string) => boolean,
  addDataPoint: (dataPoint: DataPoint) => void
): DataPoint | null {
  DEBUG && console.log("Received data:", data);

  if (data.value === undefined) {
    DEBUG && console.error("Invalid data received:", data);
    return null;
  }

  const { name }: { name: string } = data;

  // Check ref count at time of data arrival
  if (!isSubscribed(name)) {
    DEBUG && console.log(`Ignoring data for ${name}`);
    return null;
  }

  data.time ||= Date.now();

  // Handle enum signals (convert numeric values to strings for display)
  // if (isEnum) {
  // if (typeof inc.value === "number") {
  //   // Get the current enum metadata from ref (not closure)
  //   const currentEnumMetadata = enumMetadata;
  //   const hasEnumMetadata = currentEnumMetadata[name] && Object.keys(currentEnumMetadata[name]).length > 0;

  //   if (DEBUG) {
  //     console.log(`${name}: hasEnumMetadata=${hasEnumMetadata}, available keys:`,
  //       hasEnumMetadata ? Object.keys(currentEnumMetadata[name]) : 'none');
  //   }

  //   if (hasEnumMetadata) {
  //     // Convert numeric enum value to string using metadata
  //     const enumStringValue = currentEnumMetadata[name][String(inc.value)];
  //     if (enumStringValue) {
  //       inc.value = enumStringValue;
  //       if (DEBUG) console.log(`✓ Converted ${name} enum value to string: "${enumStringValue}"`);
  //     } else {
  //       // Use fallback conversion for unmapped values
  //       const stringValue = `State_${inc.value}`;
  //       inc.value = stringValue;
  //       if (DEBUG) console.log(`⚠ No mapping found for ${name} value ${inc.value}, using fallback: "${stringValue}"`);
  //     }
  //   } else {
  //     // No metadata available, use fallback string conversion
  //     const stringValue = `State_${inc.value}`;
  //     inc.value = stringValue;
  //     if (DEBUG) console.log(`⚠ No metadata for enum signal ${name}, using fallback: "${stringValue}"`);
  //   }
  // } else if (typeof inc.value === "string") {
  //   // String enum value, keep as-is
  //   if (DEBUG) console.log(`✓ Keeping ${name} as string enum value: "${inc.value}"`);
  // }
  // NOTE: DO NOT WORRY ABOUT HAVING TO CONVERT, it is all good :)
  // } else {
  // NOTE: please have SOME faith in your backend devs
  // Non-enum signal - convert strings to numbers if possible
  // if (typeof inc.value === "string") {
  //   const numValue = parseFloat(inc.value);
  //   if (!isNaN(numValue)) {
  //     inc.value = numValue;
  //     if (DEBUG) console.log(`Converted ${name} value to number: ${numValue}`);
  //   }
  // }
  // }

  // Use batched data addition
  addDataPoint(data);

  return data as DataPoint;
}

type SignalListener = (dataPoint: DataPoint) => void;

// Context interface
type SignalContextType = {
  // availableSignalQuery: UseQueryResult<SignalMeta[], Error>;
  socketConnected: boolean;
  reconnectSocket: () => void;
  activeSignals: string[];
  // alertSignals: Record<keyof typeof signalPatterns, SignalMeta[]>;
  subscribeToSignal: (signalName: string, type?: SignalType) => void;
  unsubscribeFromSignal: (signalName: string, clearData?: boolean) => void;

  // Imperative callback path for high-performance consumers (avoids React rerenders)
  addSignalListener: (signalName: string, cb: SignalListener) => () => void;

  // Data accessors exposed to consumers
  getSignalData: (signalName: string) => DataPoint[];
  getAllData: () => DataPoint[];
  getNumericalData: () => DataPoint[];
  getEnumData: () => DataPoint[];
  // clearAllData: () => void;
  // pruneData: (maxPoints?: number) => void;
  // pruneSignalData: (signalName: string) => void;
  // getSignalRefCount: (signalName: string) => number;
  // clearAllSubscriptions: () => void;
  // getEnumValues: (signalName: string) => string[];
  // mapEnumValue: (signalName: string, value: number | string) => string | undefined;
  // Optimized query methods
  // getDataInTimeRange: (startTime: number, endTime: number) => DataPoint[];
  // getSignalData: (signalName: string) => DataPoint[];
  // getDataCount: () => number;
};
// Lightweight data version store to avoid context-wide re-renders
const dataVersionStore = (() => {
  let version = 0;
  const listeners = new Set<() => void>();
  return {
    getSnapshot: () => version,
    subscribe: (fn: () => void) => {
      listeners.add(fn);
      return () => listeners.delete(fn);
    },
    bump: () => {
      version++;
      listeners.forEach((fn) => fn());
    },
  };
})();

export function useDataVersion() {
  return useSyncExternalStore(dataVersionStore.subscribe, dataVersionStore.getSnapshot);
}

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined);
// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  // const availableSignalQuery = useAvailableSignals();
  // const enumMetadata = useMemo(() => {
  //   const enumMap: Record<string, Record<string, string>> = {};
  //   if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
  //     return enumMap;
  //   }
  //   availableSignalQuery.data.forEach(s => {
  //     if (s.enum?.items) {
  //       enumMap[s.name] = s.enum.items;
  //     }
  //   });
  //   return enumMap;
  // }, [availableSignalQuery.data])
  // const alertSignals = useMemo(() => {
  //   const result: Record<keyof typeof signalPatterns, SignalMeta[]> = {
  //     warning: [], warning_count: [],
  //     info: [], info_count: [],
  //     fault: [], fault_count: [],
  //   };
  //   if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
  //     return result;
  //   }
  //   // Categorize each signal
  //   availableSignalQuery.data.forEach((signal) => {
  //     for (const [type, pattern] of Object.entries(signalPatterns)) {
  //       if (pattern.test(signal.name)) {
  //         result[type as keyof typeof signalPatterns].push(signal);
  //         break; // Move to next signal after first match
  //       }
  //     }
  //   });
  //   return result;
  // }, [availableSignalQuery.data]);
  // const signalTypes = useMemo(() => {
  //   const types: Record<string, SignalType> = {};
  //   if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
  //     return types;
  //   }
  //   availableSignalQuery.data.forEach((signal) => {
  //     types[signal.name] = signal.enum !== undefined ? SignalType.Enumeration : SignalType.Numerical
  //   });
  //   return types;
  // }, [availableSignalQuery.data]);
  // Incrementing version triggers re-render of any consumer that uses data via context (by reading a hidden span attr)
  const { dataStore, addDataPoint, pruneSignalData, clearAllData } = useSignalData(() => {
    // notify only subscribers who opt-in via useDataVersion()
    dataVersionStore.bump();
  });

  const signalListenersRef = useRef<Map<string, Set<SignalListener>>>(new Map());

  const addSignalListener = useCallback((signalName: string, cb: SignalListener) => {
    const name = signalName.trim();
    let set = signalListenersRef.current.get(name);
    if (!set) {
      set = new Set();
      signalListenersRef.current.set(name, set);
    }
    set.add(cb);

    return () => {
      const current = signalListenersRef.current.get(name);
      if (!current) return;
      current.delete(cb);
      if (current.size === 0) {
        signalListenersRef.current.delete(name);
      }
    };
  }, []);

  const notifySignalListeners = useCallback((dataPoint: DataPoint) => {
    const name = dataPoint.name;
    if (!name) return;
    const listeners = signalListenersRef.current.get(name);
    if (!listeners || listeners.size === 0) return;
    listeners.forEach((cb) => {
      try {
        cb(dataPoint);
      } catch (e) {
        DEBUG && console.warn(`Signal listener for ${name} threw`, e);
      }
    });
  }, []);
  const socket = useSocket();
  // Connection status needs a state
  // Important: start as disconnected for deterministic SSR/CSR markup
  // We'll flip to true on the first "connect" event from the browser.
  const [socketConnected, setSocketConnected] = useState<boolean>(false);

  useEffect(() => {
    const handleConnect = () => setSocketConnected(true);
    const handleDisconnect = () => setSocketConnected(false);
    socket.on("connect", handleConnect);
    socket.on("disconnect", handleDisconnect);
    return () => {
      socket.off("connect", handleConnect);
      socket.off("disconnect", handleDisconnect);
    };
  }, [socket]);
  const reconnectSocket = useCallback(() => {
    if (!socket.connected) socket.connect();
  }, [socket]);
  const { activeSignals, subscribeToSignal, unsubscribeFromSignal, isSubscribed } = useSubscribers(
    socket,
    pruneSignalData,
    clearAllData
  );
  // handling new data
  useEffect(() => {
    const data_handler = (data: any) => {
      const dataPoint = handleData(data, isSubscribed, addDataPoint);
      if (dataPoint) {
        notifySignalListeners(dataPoint);
      }
    };
    socket.on("data", data_handler);
    return () => {
      socket.off("data", data_handler);
    };
  }, [socket, isSubscribed, addDataPoint, notifySignalListeners]);
  // handling pause/resume via HTTP API (per-SID)
  const { isPaused } = useDisplayControl();

  useEffect(() => {
    // Socket must be connected to have a SID
    if (!socket.connected) return;
    const sid = socket.id;

    const send_command = isPaused ? "pause" : "play";
    const endpoint = `${BACKEND_URL}/${sid}/${send_command}`;
    // can somebody shoot me with a gun? thanks - jaden
    if (DEBUG) console.log(`Sending ${send_command} command to backend for sid ${sid}`);
    fetch(endpoint, { method: "POST" }).catch(
      (e) => DEBUG && console.warn("Pause/Play request failed", e)
    );
  }, [socket.connected, socket.id, isPaused]);
  // DOWN BADDDDD
  // const getEnumValues = useCallback((name: string) => enumMetadata[name] ? Object.values(enumMetadata[name]) : [], [enumMetadata]);
  // const mapEnumValue = useCallback((name: string, val: number | string) => enumMetadata[name]?.[String(val)], [enumMetadata]);
  // const getDataInTimeRange = useCallback((startTime: number, endTime: number): DataPoint[] => {
  //   return dataStore.current.getDataInTimeRange(startTime, endTime);
  // }, [dataStore.current]);
  // const getSignalData = useCallback((signalName: string): DataPoint[] => {
  //   return dataStore.current.getSignalData(signalName);
  // }, [dataStore.current]);
  // const getDataCount = useCallback((): number => {
  //   return dataStore.current.getDataCount();
  // }, [dataStore.current]);
  return (
    <SignalContext.Provider
      value={useMemo(() => {
        // stabilize data accessors; they read from refs and never need to change
        const getSignalData = (signalName: string) => dataStore.current.getSignalData(signalName);
        const getAllData = () => dataStore.current.getAllData();
        const getNumericalData = () => dataStore.current.getNumericalData();
        const getEnumData = () => dataStore.current.getEnumData();
        return {
          // availableSignalQuery,
          socketConnected,
          reconnectSocket,
          activeSignals,
          // alertSignals,
          subscribeToSignal,
          unsubscribeFromSignal,
          addSignalListener,
          getSignalData,
          getAllData,
          getNumericalData,
          getEnumData,
          // clearAllData,
          // pruneData,
          // pruneSignalData,
          // getSignalRefCount,
          // clearAllSubscriptions,
          // getEnumValues,
          // mapEnumValue,
          // getDataInTimeRange,
          // getSignalData,
          // getDataCount,
        };
      }, [
        socketConnected,
        reconnectSocket,
        activeSignals,
        subscribeToSignal,
        unsubscribeFromSignal,
        addSignalListener,
      ])}
    >
      {children}
    </SignalContext.Provider>
  );
}

export function useSignals() {
  const context = useContext(SignalContext);
  if (!context) throw new Error("useSignals must be used within a SignalProvider");
  return context;
}
