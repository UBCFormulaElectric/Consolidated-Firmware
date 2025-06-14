"use client";

import { PausePlayContext } from "@/components/shared/pause-play-control";
import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useEffect,
  useRef,
  useState,
} from "react";
import io, { Socket } from "socket.io-client";
import {
  BACKEND_URL,
  DataPoint,
  DEBUG,
  DEFAULT_MAX_DATA_POINTS,
  MAX_RECONNECT_ATTEMPTS,
  RECONNECT_INTERVAL,
  SignalMeta,
  SignalType,
} from "./SignalConfig";

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

export const getSignalType = (name: string): AlertSignalType | null => {
  for (const [patternKey, pattern] of Object.entries(signalPatterns)) {
    if (pattern.test(name)) {
      return patternToTypeMap[patternKey as keyof typeof signalPatterns];
    }
  }
  return null;
};
// Context interface
type SignalContextType = {
  availableSignals: SignalMeta[];
  isLoadingSignals: boolean;
  signalError: string | null;
  socket: Socket | null;
  isConnected: boolean;
  activeSignals: string[];
  data: DataPoint[];
  numericalData: DataPoint[];
  enumData: DataPoint[];
  currentTime: number;
  subscribeToSignal: (signalName: string, type?: SignalType) => void;
  unsubscribeFromSignal: (signalName: string, clearData?: boolean) => void;
  clearData: () => void;
  pruneData: (maxPoints?: number) => void;
  pruneSignalData: (signalName: string) => void;
  getSignalRefCount: (signalName: string) => number;
  clearAllSubscriptions: () => void;
  reconnect: () => void;
  isEnumSignal: (signalName: string) => boolean;
  isNumericalSignal: (signalName: string) => boolean;
  getEnumValues: (signalName: string) => string[];
  mapEnumValue: (
    signalName: string,
    value: number | string
  ) => string | undefined;
  getAlertSignals: () => Record<
    | "warning"
    | "warning_count"
    | "info"
    | "info_count"
    | "fault"
    | "fault_count",
    SignalMeta[]
  >;
  signalPatterns: typeof signalPatterns;
};

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined);

// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  const { isPaused = false } = useContext(PausePlayContext) || {};

  const [availableSignals, setAvailableSignals] = useState<SignalMeta[]>([]);
  const [isLoadingSignals, setIsLoadingSignals] = useState(true);
  const [signalError, setSignalError] = useState<string | null>(null);
  const [socket, setSocket] = useState<Socket | null>(null);
  const [isConnected, setIsConnected] = useState(false);
  const [reconnectAttempts, setReconnectAttempts] = useState(0);
  const [activeSignals, setActiveSignals] = useState<string[]>([]);
  const [data, setData] = useState<DataPoint[]>([]);
  const [numericalData, setNumericalData] = useState<DataPoint[]>([]);
  const [enumData, setEnumData] = useState<DataPoint[]>([]);
  const [currentTime, setCurrentTime] = useState(Date.now());
  const [enumMetadata, setEnumMetadata] = useState<
    Record<string, Record<string, string>>
  >({});

  const signalSubscribers = useRef<Record<string, number>>({});
  const signalTypes = useRef<Record<string, SignalType>>({});

  // Race condition prevention
  const pendingUnsubscriptions = useRef<Set<string>>(new Set());
  const isMounted = useRef(true);

  // Cleanup on unmount
  useEffect(() => {
    return () => {
      isMounted.current = false;
      pendingUnsubscriptions.current.clear();
    };
  }, []);

  const isEnumSignal = useCallback((name: string) => {
    const type = signalTypes.current[name];
    if (type) return type === SignalType.Enumeration;
    const l = name.toLowerCase();
    return /state|mode|enum|status/.test(l);
  }, []);

  const isNumericalSignal = useCallback((name: string) => {
    const type = signalTypes.current[name];
    return type ? type === SignalType.Numerical : !isEnumSignal(name);
  }, []);

  const getSignalRefCount = useCallback(
    (name: string) => signalSubscribers.current[name] || 0,
    []
  );

  const getAlertSignals = () => {
    const warning: SignalMeta[] = [];
    const warning_count: SignalMeta[] = [];
    const info: SignalMeta[] = [];
    const info_count: SignalMeta[] = [];
    const fault: SignalMeta[] = [];
    const fault_count: SignalMeta[] = [];

    const result: Record<keyof typeof signalPatterns, SignalMeta[]> = {
      warning: [],
      warning_count: [],
      info: [],
      info_count: [],
      fault: [],
      fault_count: [],
    };

    // Categorize each signal
    availableSignals.forEach((signal) => {
      for (const [type, pattern] of Object.entries(signalPatterns)) {
        if (pattern.test(signal.name)) {
          result[type as keyof typeof signalPatterns].push(signal);
          break; // Move to next signal after first match
        }
      }
    });

    return result;
  };

  // subscript all alerts
  useEffect(() => {
    const alerts = getAlertSignals();
    console.log(alerts);
    Object.values(alerts).forEach((alertArray) => {
      alertArray.forEach((alert) => {
        subscribeToSignal(alert.name);
      });
    });
  }, [availableSignals]);

  const logSubscriptionState = useCallback(() => {
    if (!DEBUG) return;
    console.log("Current subscription state:");
    console.log("Active signals:", activeSignals);
    const refs =
      Object.entries(signalSubscribers.current)
        .filter(([, c]) => c > 0)
        .map(([s, c]) => `${s}: ${c}`)
        .join(", ") || "None";
    console.log("Signal ref counts:", refs);
  }, [activeSignals]);

  const pruneSignalData = useCallback((name: string) => {
    if (DEBUG) console.log(`Pruning all data for signal: ${name}`);
    setData((d) => d.filter((p) => p.name !== name));
    setNumericalData((d) => d.filter((p) => p.name !== name));
    setEnumData((d) => d.filter((p) => p.name !== name));
    delete signalTypes.current[name];
    delete signalSubscribers.current[name];
  }, []);

  const pruneData = useCallback((max = DEFAULT_MAX_DATA_POINTS) => {
    if (DEBUG) console.log(`Pruning data to last ${max} points`);
    setData((d) => (d.length > max ? d.slice(-max) : d));
    setNumericalData((d) => (d.length > max ? d.slice(-max) : d));
    setEnumData((d) => (d.length > max ? d.slice(-max) : d));
  }, []);

  const fetchSignalMetadata = useCallback(async () => {
    if (DEBUG) console.log("Fetching signal metadata");
    try {
      setIsLoadingSignals(true);
      setSignalError(null);
      const res = await fetch(`${BACKEND_URL}/api/signal`);
      if (!res.ok) throw new Error(`Failed to fetch signals: ${res.status}`);
      const list = await res.json();
      const metas: SignalMeta[] = list.map((s: any) => ({
        name: s.name,
        unit: s.unit,
        cycle_time_ms: s.cycle_time_ms,
      }));
      setAvailableSignals(metas);

      const enumMap: Record<string, Record<string, string>> = {};
      list.forEach(
        (s: any) => s.enum?.items && (enumMap[s.name] = s.enum.items)
      );
      setEnumMetadata(enumMap);

      if (DEBUG) {
        console.log("Fetched enumerations and states:");
        Object.entries(enumMap).forEach(([s, m]) =>
          console.log(`  ${s}: [${Object.values(m).join(", ")}]`)
        );
        console.log("Fetched signals:", metas);
      }

      setIsLoadingSignals(false);
    } catch (err: any) {
      DEBUG && console.log("Error fetching signals:", err);
      setSignalError(err.message);
      setIsLoadingSignals(false);
      if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        setTimeout(() => {
          setReconnectAttempts((a) => a + 1);
          fetchSignalMetadata();
        }, RECONNECT_INTERVAL);
      }
    }
  }, [reconnectAttempts]);

  const initializeSocket = useCallback(() => {
    DEBUG && console.log("Initializing socket");
    socket?.disconnect();
    const s = io(BACKEND_URL, {
      reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
      timeout: 10000,
    });

    s.on("connect", () => {
      DEBUG && console.log("Socket connected");
      setIsConnected(true);
      setReconnectAttempts(0);
      if (!isPaused)
        activeSignals.forEach((sig) => {
          if ((signalSubscribers.current[sig] || 0) > 0) {
            if (DEBUG) {
              console.log(`Resubscribing to ${sig} on reconnect`);
            }
            s.emit("sub", sig);
          }
        });
    });
    s.on("connect_error", (e) => {
      DEBUG && console.log("Socket error:", e);
      setIsConnected(false);
    });
    s.on("disconnect", (r) => {
      DEBUG && console.log("Socket disconnected:", r);
      setIsConnected(false);
    });

    const handler = (inc: any) => {
      DEBUG && console.log("Received data:", inc);
      const { name, value } = inc;
      if (!name) return;
      if ((signalSubscribers.current[name] || 0) === 0)
        return DEBUG && console.log(`Ignoring data for ${name}`);
      inc.time ||= Date.now();
      if (typeof inc.value === "string") inc.value = parseFloat(inc.value);
      
      // Optimized array operations - avoid spread operator memory allocation
      // No data point limiting - preserve all data points as requested
      
      setData((d) => {
        const newData = d.slice(); // Shallow copy (much faster than spread)
        newData.push(inc);
        return newData;
      });
      
      if (isEnumSignal(name)) {
        setEnumData((d) => {
          const newData = d.slice();
          newData.push(inc);
          return newData;
        });
      } else if (!isNaN(inc.value as number)) {
        setNumericalData((d) => {
          const newData = d.slice();
          newData.push(inc);
          return newData;
        });
      }
      setCurrentTime(Date.now());
    };

    s.on("data", handler);
    s.on("sub_ack", () => DEBUG && console.log("Subscription acknowledged"));
    setSocket(s);
    console.log("Socket init complete");
    return () => {
      s.off("data", handler);
      s.disconnect();
    };
  }, [activeSignals, isEnumSignal, isPaused, socket]);

  const reconnect = useCallback(() => {
    DEBUG && console.log("Reconnecting...");
    setReconnectAttempts(0);
    fetchSignalMetadata();
    initializeSocket();
  }, [fetchSignalMetadata, initializeSocket]);

  useEffect(() => {
    fetchSignalMetadata();
    const clean = initializeSocket();
    return clean;
  }, []);

  useEffect(() => {
    const id = setInterval(() => !isPaused && setCurrentTime(Date.now()), 100);
    return () => clearInterval(id);
  }, [isPaused]);

  // Pause/play effect with race condition prevention
  const pausedRef = useRef(isPaused);

  useEffect(() => {
    if (!socket || !isConnected || !isMounted.current) return;

    // Only act on actual pause state changes, not activeSignals changes
    const pauseStateChanged = pausedRef.current !== isPaused;
    if (!pauseStateChanged) return;

    pausedRef.current = isPaused;

    const action = isPaused ? "unsub" : "sub";
    if (DEBUG) {
      console.log(
        `SignalProvider - ${
          isPaused ? "Pausing" : "Playing"
        } - isPaused: ${isPaused}`
      );
    }

    // Use current activeSignals value from state, not dependency
    const currentActiveSignals = activeSignals;

    // Only process signals that have ref count > 0 and are not pending unsubscription
    const signalsToProcess = currentActiveSignals.filter((sig) => {
      const refCount = signalSubscribers.current[sig] || 0;
      const notPending = !pendingUnsubscriptions.current.has(sig);
      return refCount > 0 && notPending;
    });

    if (DEBUG && signalsToProcess.length > 0) {
      console.log(
        `Processing ${
          signalsToProcess.length
        } signals: [${signalsToProcess.join(", ")}]`
      );
    }

    signalsToProcess.forEach((sig) => {
      if (DEBUG) {
        console.log(
          `${
            isPaused ? "Pausing data stream" : "Resuming data stream"
          } for ${sig}`
        );
      }
      socket.emit(action, sig);
    });
  }, [isPaused, socket, isConnected]); // Removed activeSignals dependency

  const subscribeToSignal = useCallback(
    (signalName: string, type: SignalType = SignalType.Any) => {
      const name = signalName.trim();

      // Skip if unmounted or already pending unsubscription
      if (!isMounted.current || pendingUnsubscriptions.current.has(name)) {
        if (DEBUG)
          console.log(
            `Skipping subscribe for ${name} - unmounted or pending unsubscription`
          );
        return;
      }

      const assigned =
        type === SignalType.Any
          ? isEnumSignal(name)
            ? SignalType.Enumeration
            : SignalType.Numerical
          : type;
      signalTypes.current[name] = assigned;
      signalSubscribers.current[name] =
        (signalSubscribers.current[name] || 0) + 1;

      if (DEBUG) {
        console.log(
          `Subscribing to ${name} (${
            assigned === SignalType.Enumeration ? "enum" : "numerical"
          }) - ref count: ${signalSubscribers.current[name]}`
        );
        // Inline logging to avoid dependency on logSubscriptionState
        console.log("Current subscription state:");
        const refs =
          Object.entries(signalSubscribers.current)
            .filter(([, c]) => c > 0)
            .map(([s, c]) => `${s}: ${c}`)
            .join(", ") || "None";
        console.log("Signal ref counts:", refs);
      }

      // Only add to activeSignals and emit subscription if this is the first subscriber
      const isFirstSubscriber = signalSubscribers.current[name] === 1;

      if (isFirstSubscriber) {
        setActiveSignals((current) => {
          // Double-check it's not already in the array to prevent duplicates
          if (current.includes(name)) return current;
          return [...current, name];
        });

        // Only emit subscription if connected and not paused
        if (socket && isConnected && !isPaused) {
          socket.emit("sub", name);
        }
      }
    },
    [isConnected, isEnumSignal, isPaused, socket]
  );

  const unsubscribeFromSignal = useCallback(
    (signalName: string, clearData: boolean = true) => {
      const name = signalName.trim();

      // Prevent race conditions during deletion
      if (pendingUnsubscriptions.current.has(name) || !isMounted.current) {
        if (DEBUG)
          console.log(
            `Skipping unsubscribe for ${name} - already pending or unmounted`
          );
        return;
      }

      pendingUnsubscriptions.current.add(name);

      const count = (signalSubscribers.current[name] || 0) - 1;
      if (count < 0) {
        pendingUnsubscriptions.current.delete(name);
        return (
          DEBUG && console.log(`Warning: Unsubscribe ${name} ref count <= 0`)
        );
      }

      signalSubscribers.current[name] = count;
      if (DEBUG) {
        console.log(`Unsubscribing from ${name} - new ref count: ${count}`);
        // Inline logging to avoid dependency on logSubscriptionState
        console.log("Current subscription state:");
        const refs =
          Object.entries(signalSubscribers.current)
            .filter(([, c]) => c > 0)
            .map(([s, c]) => `${s}: ${c}`)
            .join(", ") || "None";
        console.log("Signal ref counts:", refs);
      }

      if (count === 0) {
        // Remove from active signals first
        setActiveSignals((a) => a.filter((s) => s !== name));

        // Always emit unsubscription if connected, regardless of pause state
        // This ensures clean disconnection from the signal
        if (socket && isConnected) {
          socket.emit("unsub", name);
        }

        // Only clean up data if explicitly requested (for true unsubscription, not pause)
        if (clearData) {
          pruneSignalData(name);
        }

        // Clear pending flag immediately for final unsubscription
        pendingUnsubscriptions.current.delete(name);
      } else {
        // Clear pending flag after a brief delay for partial unsubscription
        setTimeout(() => {
          if (isMounted.current) {
            pendingUnsubscriptions.current.delete(name);
          }
        }, 50);
      }
    },
    [isConnected, pruneSignalData, socket]
  );

  const clearData = () => {
    if (DEBUG) console.log("Clearing data");
    setData([]);
    setNumericalData([]);
    setEnumData([]);
  };

  const clearAllSubscriptions = useCallback(() => {
    if (!isMounted.current) return;

    DEBUG && console.log("Clearing all subscriptions and data");

    // Clear pending operations first
    pendingUnsubscriptions.current.clear();

    // Batch unsubscribe all active signals
    if (socket && isConnected) {
      activeSignals.forEach((sig) => socket.emit("unsub", sig));
    }

    // Clear all state
    setActiveSignals([]);
    signalSubscribers.current = {};
    signalTypes.current = {};
    clearData();
  }, [activeSignals, isConnected, socket]);

  const getEnumValues = useCallback(
    (name: string) =>
      enumMetadata[name] ? Object.values(enumMetadata[name]) : [],
    [enumMetadata]
  );
  const mapEnumValue = useCallback(
    (name: string, val: number | string) => enumMetadata[name]?.[String(val)],
    [enumMetadata]
  );

  return (
    <SignalContext.Provider
      value={{
        availableSignals,
        isLoadingSignals,
        signalError,
        socket,
        isConnected,
        activeSignals,
        data,
        numericalData,
        enumData,
        currentTime,
        signalPatterns,
        subscribeToSignal,
        unsubscribeFromSignal,
        clearData,
        pruneData,
        pruneSignalData,
        getSignalRefCount,
        clearAllSubscriptions,
        reconnect,
        isEnumSignal,
        isNumericalSignal,
        getEnumValues,
        mapEnumValue,
        getAlertSignals,
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

export { SignalType } from "./SignalConfig";
export type { DataPoint } from "./SignalConfig";
