// Centralized subscription management - single effect to handle all subscription changes
// TODO unsubscribe all signals for sockets which are disconnected on backend
// TODO handle pause
// TODO subscribe when activeSignals change w/diff algo or callbacks :)
// TODO all alerts must be broadcast to all listeners

"use client";

import { useDisplayControl } from "@/components/shared/PausePlayControl";
import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useEffect,
  useMemo,
} from "react";
import {
  BACKEND_URL,
  DataPoint,
  DEBUG,
  MAX_RECONNECT_ATTEMPTS,
  SignalMeta,
  SignalType,
} from "./SignalConfig";

import { useQuery, UseQueryResult } from "@tanstack/react-query";
import { useSocket } from "./socket";
import { useSignalData } from "./useSignalData";
import { useSubscribers } from "./useSubscribers";

export const signalPatterns = {
  warning: /^.+_Warning_(?!.*Count$).+$/, // Warning messages (not counts)
  warning_count: /^.+_Warning_.+Count$/, // Warning counts
  info: /^.+Info_(?!.*Count$).+$/, // Info messages (not counts)
  info_count: /^.+_Info_.+Count$/, // Info counts
  fault: /^.+Fault_(?!.*Count$).+$/, // Fault messages (not counts)
  fault_count: /^.+_Fault_.+Count$/, // Fault counts
};

export type AlertType = "Fault" | "Warning" | "Info";
export type AlertSignalType =
  | AlertType
  | "FaultCount"
  | "WarningCount"
  | "InfoCount";

const patternToTypeMap: Record<keyof typeof signalPatterns, AlertSignalType> = {
  warning: "Warning",
  warning_count: "WarningCount",
  info: "Info",
  info_count: "InfoCount",
  fault: "Fault",
  fault_count: "FaultCount",
};

export const getAlertSignalType = (name: string): AlertSignalType | null => {
  for (const [patternKey, pattern] of Object.entries(signalPatterns)) {
    if (pattern.test(name)) {
      return patternToTypeMap[patternKey as keyof typeof signalPatterns];
    }
  }
  return null;
};

function useAvailableSignals() {
  return useQuery({
    queryKey: ["availableSignals"],
    queryFn: async () => {
      if (DEBUG) console.log("Fetching signal metadata");
      const res = await fetch(`${BACKEND_URL}/api/signal`);
      if (!res.ok) throw new Error(`Failed to fetch signals: ${res.status}`);
      const list = await res.json();

      const metas: SignalMeta[] = list.map((s: any) => ({
        name: s.name,
        unit: s.unit,
        cycle_time_ms: s.cycle_time_ms,
        msg_name: s.msg_name,
        msg_id: s.id,
        enum: s.enum
      }));

      return metas;
    },
    retry: MAX_RECONNECT_ATTEMPTS,
  })
}

function handleData(
  inc: any,
  isSubscribed: (signal_name: string) => boolean,
  addDataPoint: (dataPoint: DataPoint) => void,
  enumMetadata: Record<string, Record<string, string>>
) {
  DEBUG && console.log("Received data:", inc);

  if (!inc || typeof inc !== "object" || !inc.name || !inc.value) {
    DEBUG && console.error("Invalid data received:", inc);
    return;
  }

  const { name }: { name: string } = inc;

  // Check ref count at time of data arrival
  if (!isSubscribed(name)) {
    return DEBUG && console.log(`Ignoring data for ${name}`);
  }

  inc.time ||= Date.now();

  // Check if this signal is an enumeration signal using pattern matching
  const isEnum = /state|mode|enum|status/.test(name.toLowerCase());

  if (DEBUG) {
    console.log(`Signal ${name}: isEnum=${isEnum}, value=${inc.value} (${typeof inc.value})`);
  }

  // Handle enum signals (convert numeric values to strings for display)
  if (isEnum) {
    if (typeof inc.value === "number") {
      // Get the current enum metadata from ref (not closure)
      const currentEnumMetadata = enumMetadata;
      const hasEnumMetadata = currentEnumMetadata[name] && Object.keys(currentEnumMetadata[name]).length > 0;

      if (DEBUG) {
        console.log(`${name}: hasEnumMetadata=${hasEnumMetadata}, available keys:`,
          hasEnumMetadata ? Object.keys(currentEnumMetadata[name]) : 'none');
      }

      if (hasEnumMetadata) {
        // Convert numeric enum value to string using metadata
        const enumStringValue = currentEnumMetadata[name][String(inc.value)];
        if (enumStringValue) {
          inc.value = enumStringValue;
          if (DEBUG) console.log(`✓ Converted ${name} enum value to string: "${enumStringValue}"`);
        } else {
          // Use fallback conversion for unmapped values
          const stringValue = `State_${inc.value}`;
          inc.value = stringValue;
          if (DEBUG) console.log(`⚠ No mapping found for ${name} value ${inc.value}, using fallback: "${stringValue}"`);
        }
      } else {
        // No metadata available, use fallback string conversion
        const stringValue = `State_${inc.value}`;
        inc.value = stringValue;
        if (DEBUG) console.log(`⚠ No metadata for enum signal ${name}, using fallback: "${stringValue}"`);
      }
    } else if (typeof inc.value === "string") {
      // String enum value, keep as-is
      if (DEBUG) console.log(`✓ Keeping ${name} as string enum value: "${inc.value}"`);
    }
  } else {
    // Non-enum signal - convert strings to numbers if possible
    if (typeof inc.value === "string") {
      const numValue = parseFloat(inc.value);
      if (!isNaN(numValue)) {
        inc.value = numValue;
        if (DEBUG) console.log(`Converted ${name} value to number: ${numValue}`);
      }
    }
  }

  // Use batched data addition
  addDataPoint(inc);
}


// Context interface
type SignalContextType = {
  availableSignalQuery: UseQueryResult<SignalMeta[], Error>;
  activeSignals: string[];
  alertSignals: Record<keyof typeof signalPatterns, SignalMeta[]>;
  subscribeToSignal: (signalName: string, type?: SignalType) => void;
  unsubscribeFromSignal: (signalName: string, clearData?: boolean) => void;
  clearAllData: () => void;
  pruneData: (maxPoints?: number) => void;
  pruneSignalData: (signalName: string) => void;
  getSignalRefCount: (signalName: string) => number;
  clearAllSubscriptions: () => void;
  getEnumValues: (signalName: string) => string[];
  mapEnumValue: (
    signalName: string,
    value: number | string
  ) => string | undefined;
  // Optimized query methods
  getDataInTimeRange: (startTime: number, endTime: number) => DataPoint[];
  getSignalData: (signalName: string) => DataPoint[];
  getDataCount: () => number;
};
// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined);
// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  const availableSignalQuery = useAvailableSignals();
  const enumMetadata = useMemo(() => {
    const enumMap: Record<string, Record<string, string>> = {};
    if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
      return enumMap;
    }
    availableSignalQuery.data.forEach(s => {
      if (s.enum?.items) {
        enumMap[s.name] = s.enum.items;
      }
    });
    return enumMap;
  }, [availableSignalQuery.data])
  const alertSignals = useMemo(() => {
    const result: Record<keyof typeof signalPatterns, SignalMeta[]> = {
      warning: [], warning_count: [],
      info: [], info_count: [],
      fault: [], fault_count: [],
    };
    if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
      return result;
    }
    // Categorize each signal
    availableSignalQuery.data.forEach((signal) => {
      for (const [type, pattern] of Object.entries(signalPatterns)) {
        if (pattern.test(signal.name)) {
          result[type as keyof typeof signalPatterns].push(signal);
          break; // Move to next signal after first match
        }
      }
    });
    return result;
  }, [availableSignalQuery.data]);
  const signalTypes = useMemo(() => {
    const types: Record<string, SignalType> = {};
    if (!availableSignalQuery.data || availableSignalQuery.data.length === 0) {
      return types;
    }
    availableSignalQuery.data.forEach((signal) => {
      types[signal.name] = signal.enum !== undefined ? SignalType.Enumeration : SignalType.Numerical
    });
    return types;
  }, [availableSignalQuery.data]);
  const { dataStore, addDataPoint, pruneSignalData, pruneData, clearAllData } = useSignalData();
  const socket = useSocket();
  const { activeSignals, subscribeToSignal, unsubscribeFromSignal, getSignalRefCount, isSubscribed, clearAllSubscriptions } = useSubscribers(socket, pruneSignalData, clearAllData);
  // handling new data
  useEffect(() => {
    const data_handler = (data: any) => handleData(data, isSubscribed, addDataPoint, enumMetadata);
    socket.on("data", data_handler);
    return () => {
      socket.off("data", data_handler);
    };
  }, [socket, isSubscribed, addDataPoint, enumMetadata])
  // handling pause/resume
  const { isPaused } = useDisplayControl();
  useEffect(() => {
    // Emit pause event to backend
    if (socket.active) {
      throw new Error("Socket is not active, cannot pause signals");
    }
    const send_cmd = isPaused ? "resume" : "pause";
    if (DEBUG) console.log(`Sending ${send_cmd} command to backend`);
    socket.emit(send_cmd);
  }, [socket, isPaused]);
  // DOWN BADDDDD
  const getEnumValues = useCallback((name: string) => enumMetadata.current[name] ? Object.values(enumMetadata.current[name]) : [], []);
  const mapEnumValue = useCallback((name: string, val: number | string) => enumMetadata[name]?.[String(val)], [enumMetadata]);
  const getDataInTimeRange = useCallback((startTime: number, endTime: number): DataPoint[] => {
    return dataStore.current.getDataInTimeRange(startTime, endTime);
  }, [dataStore.current]);
  const getSignalData = useCallback((signalName: string): DataPoint[] => {
    return dataStore.current.getSignalData(signalName);
  }, [dataStore.current]);
  const getDataCount = useCallback((): number => {
    return dataStore.current.getDataCount();
  }, [dataStore.current]);
  return (
    <SignalContext.Provider
      value={{
        availableSignalQuery,
        activeSignals,
        alertSignals,
        subscribeToSignal,
        unsubscribeFromSignal,
        clearAllData,
        pruneData,
        pruneSignalData,
        getSignalRefCount,
        clearAllSubscriptions,
        getEnumValues,
        mapEnumValue,
        getDataInTimeRange,
        getSignalData,
        getDataCount,
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