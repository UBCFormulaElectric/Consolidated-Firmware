// Centralized subscription management - single effect to handle all subscription changes
// TODO unsubscribe all signals for sockets which are disconnected on backend
// TODO all alerts must be broadcast to all listeners

"use client";

import { useDisplayControl } from "@/components/shared/PausePlayControl";
import { createContext, ReactNode, useCallback, useContext, useEffect, } from "react";
import { BACKEND_URL, DataPoint, DEBUG, SignalType } from "./SignalConfig";

import { useSocket } from "./signals/useSocket";
import { useSignalData } from "./signals/useSignalData";
import { useSubscribers } from "./signals/useSubscribers";


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
  addDataPoint: (dataPoint: DataPoint) => void,
) {
  DEBUG && console.log("Received data:", data);

  if (!data || typeof data !== "object" || !data.name || !data.value) {
    DEBUG && console.error("Invalid data received:", data);
    return;
  }

  const { name }: { name: string } = data;

  // Check ref count at time of data arrival
  if (!isSubscribed(name)) {
    return DEBUG && console.log(`Ignoring data for ${name}`);
  }

  data.time ||= Date.now();

  // Check if this signal is an enumeration signal using pattern matching
  if (DEBUG) {
    const isEnum = /state|mode|enum|status/.test(name.toLowerCase());
    console.log(`Signal ${name}: isEnum=${isEnum}, value=${data.value} (${typeof data.value})`);
  }

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
}

// Context interface
type SignalContextType = {
  // availableSignalQuery: UseQueryResult<SignalMeta[], Error>;
  socketConnected: boolean;
  reconnectSocket: () => void;
  activeSignals: string[];
  // alertSignals: Record<keyof typeof signalPatterns, SignalMeta[]>;
  subscribeToSignal: (signalName: string, type?: SignalType) => void;
  unsubscribeFromSignal: (signalName: string, clearData?: boolean) => void;
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
  const { dataStore, addDataPoint, pruneSignalData, pruneData, clearAllData } = useSignalData();
  const socket = useSocket();
  const socketConnected = socket.connected;
  const reconnectSocket = useCallback(() => {
    if (!socket.connected) socket.connect();
  }, [socket]);
  const { activeSignals, subscribeToSignal, unsubscribeFromSignal, getSignalRefCount, isSubscribed, clearAllSubscriptions } = useSubscribers(socket, pruneSignalData, clearAllData);
  // handling new data
  useEffect(() => {
    const data_handler = (data: any) => handleData(data, isSubscribed, addDataPoint);
    socket.on("data", data_handler);
    return () => {
      socket.off("data", data_handler);
    };
  }, [socket, isSubscribed, addDataPoint]);
  // handling pause/resume via HTTP API (per-SID)
  const { isPaused } = useDisplayControl();

  useEffect(() => {
    // Socket must be connected to have a SID
    if (!socket.connected) return;
    const sid = socket.id;

    const send_command = isPaused ? "pause" : "play";
    const endpoint = `${BACKEND_URL}/${sid}/${send_command}`;
    if (DEBUG) console.log(`Sending ${send_command} command to backend for sid ${sid}`);
    fetch(endpoint, { method: "POST" }).catch((e) => DEBUG && console.warn("Pause/Play request failed", e));
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
      value={{
        // availableSignalQuery,
        socketConnected,
        reconnectSocket,
        activeSignals,
        // alertSignals,
        subscribeToSignal,
        unsubscribeFromSignal,
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
      }}
    >
      {children}
    </SignalContext.Provider>
  );
}

export function useSignals() {
  const context = useContext(SignalContext);
  if (!context)
    throw new Error("useSignals must be used within a SignalProvider");
  return context;
}