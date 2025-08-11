"use client";

import { PausePlayContext } from "@/components/shared/PausePlayControl";
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
  SignalDataStore,
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
  // Optimized query methods
  getDataInTimeRange: (startTime: number, endTime: number) => DataPoint[];
  getSignalData: (signalName: string) => DataPoint[];
  getDataCount: () => number;
};

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined);

// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  const { isPaused = false } = useContext(PausePlayContext) || {};

  const [availableSignals, setAvailableSignals] = useState<SignalMeta[]>([]);
  const [isLoadingSignals, setIsLoadingSignals] = useState(true);
  const [signalError, setSignalError] = useState<string | null>(null);
  const [isConnected, setIsConnected] = useState(false);
  const [reconnectAttempts, setReconnectAttempts] = useState(0);
  const [activeSignals, setActiveSignals] = useState<string[]>([]);
  const [currentTime, setCurrentTime] = useState(Date.now());
  const [enumMetadata, setEnumMetadata] = useState<
    Record<string, Record<string, string>>
  >({});

  // Use ref for enumMetadata to avoid closure issues in socket handlers
  const enumMetadataRef = useRef<Record<string, Record<string, string>>>({});

  // Socket stored in ref to prevent re-initialization on dependency changes
  const socketRef = useRef<Socket | null>(null);

  // Optimized data storage
  const dataStore = useRef<SignalDataStore>(new SignalDataStore());
  
  // Backward compatibility - computed from optimized store
  const [data, setData] = useState<DataPoint[]>([]);
  const [numericalData, setNumericalData] = useState<DataPoint[]>([]);
  const [enumData, setEnumData] = useState<DataPoint[]>([]);

  const signalSubscribers = useRef<Record<string, number>>({});
  const signalTypes = useRef<Record<string, SignalType>>({});

  // Race condition prevention
  const pendingUnsubscriptions = useRef<Set<string>>(new Set());
  const isMounted = useRef(true);

  // OPTIMIZED BATCHING SYSTEM using data store
  const pendingDataUpdates = useRef<DataPoint[]>([]);
  const batchUpdateTimer = useRef<NodeJS.Timeout | null>(null);
  const rafUpdateTimer = useRef<number | null>(null);
  const BATCH_INTERVAL_MS = 50; // Fallback for setTimeout
  const MAX_BATCH_SIZE = 100; // Max data points per batch
  const currentTimeUpdateCounter = useRef(0);
  const CURRENT_TIME_UPDATE_FREQUENCY = 10; // Update currentTime every 10th batch
  const lastBatchTime = useRef(Date.now());
  const isPausedRef = useRef(isPaused);
  isPausedRef.current = isPaused;

  // Stable ref for addDataPoint to avoid socket re-initialization
  const addDataPointRef = useRef<(dataPoint: DataPoint) => void>(() => {});

  // Update compatibility arrays from data store
  const updateCompatibilityArrays = useCallback(() => {
    const allData = dataStore.current.getAllData();
    const numData = dataStore.current.getNumericalData();
    const enumDataPoints = dataStore.current.getEnumData();
    
    setData(allData);
    setNumericalData(numData);
    setEnumData(enumDataPoints);
  }, []);

  // Cleanup on unmount
  useEffect(() => {
    return () => {
      isMounted.current = false;
      pendingUnsubscriptions.current.clear();
      // Clear batch timers
      if (batchUpdateTimer.current) {
        clearTimeout(batchUpdateTimer.current);
      }
      if (rafUpdateTimer.current) {
        cancelAnimationFrame(rafUpdateTimer.current);
      }
      // Disconnect socket on unmount
      if (socketRef.current) {
        socketRef.current.disconnect();
      }
    };
  }, []);

  // Initialize socket exactly once
  useEffect(() => {
    DEBUG && console.log("Initializing socket - one time only");
    
    const s = io(BACKEND_URL, {
      reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
      timeout: 10000,
    });

    s.on("connect", () => {
      DEBUG && console.log("Socket connected");
      setIsConnected(true);
      setReconnectAttempts(0);
    });

    s.on("connect_error", (e) => {
      DEBUG && console.log("Socket error:", e);
      setIsConnected(false);
    });

    s.on("disconnect", (r) => {
      DEBUG && console.log("Socket disconnected:", r);
      setIsConnected(false);
    });

    // Data handler setup - this callback is stable and doesn't depend on volatile state
    const handleData = (inc: any) => {
      DEBUG && console.log("Received data:", inc);
      const { name, value } = inc;
      if (!name) return;
      
      // Check ref count at time of data arrival
      if ((signalSubscribers.current[name] || 0) === 0) {
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
          const currentEnumMetadata = enumMetadataRef.current;
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
      addDataPointRef.current(inc);
    };

    s.on("data", handleData);
    s.on("sub_ack", () => DEBUG && console.log("Subscription acknowledged"));

    socketRef.current = s;
    console.log("Socket init complete");

    return () => {
      s.off("data", handleData);
      s.disconnect();
    };
  }, []); // Empty dependency array - initialize only once

  const isEnumSignal = useCallback((name: string) => {
    const type = signalTypes.current[name];
    if (type) return type === SignalType.Enumeration;
    
    // Original detection: pattern matching only
    const l = name.toLowerCase();
    return /state|mode|enum|status/.test(l);
  }, []);

  const isNumericalSignal = useCallback((name: string) => {
    const type = signalTypes.current[name];
    return type ? type === SignalType.Numerical : !isEnumSignal(name);
  }, []);

  // OPTIMIZED BATCHED DATA UPDATE FUNCTION with RAF and smart batching
  const flushPendingDataUpdates = useCallback(() => {
    if (pendingDataUpdates.current.length === 0) return;

    const updates = [...pendingDataUpdates.current];
    pendingDataUpdates.current = [];

    // Track batching performance for adaptive behavior
    const now = Date.now();
    const timeSinceLastBatch = now - lastBatchTime.current;
    lastBatchTime.current = now;

    // Group updates by signal for more efficient processing
    const signalGroups = new Map<string, DataPoint[]>();
    updates.forEach((point) => {
      const signalName = point.name;
      if (signalName && !signalGroups.has(signalName)) {
        signalGroups.set(signalName, []);
      }
      if (signalName) {
        signalGroups.get(signalName)!.push(point);
      }
    });

    // Add all updates to the optimized data store
    signalGroups.forEach((points) => {
      points.forEach((point) => {
        dataStore.current.addDataPoint(point);
      });
    });

    // Update compatibility arrays once per batch
    updateCompatibilityArrays();

    // Adaptive currentTime updates based on data flow
    currentTimeUpdateCounter.current++;
    const isHighFrequency = timeSinceLastBatch < 100; // High frequency if batches < 100ms apart
    const updateFrequency = isHighFrequency ? CURRENT_TIME_UPDATE_FREQUENCY * 2 : CURRENT_TIME_UPDATE_FREQUENCY;
    
    if (currentTimeUpdateCounter.current >= updateFrequency) {
      currentTimeUpdateCounter.current = 0;
      setCurrentTime(now);
    }

    // Clear timers
    batchUpdateTimer.current = null;
    rafUpdateTimer.current = null;
  }, [updateCompatibilityArrays]);

  // ENHANCED BATCHED DATA ADDITION with RAF and size-based flushing
  const addDataPoint = useCallback((dataPoint: DataPoint) => {
    pendingDataUpdates.current.push(dataPoint);

    // Immediate flush if batch size exceeds threshold (prevents memory buildup)
    if (pendingDataUpdates.current.length >= MAX_BATCH_SIZE) {
      if (rafUpdateTimer.current) {
        cancelAnimationFrame(rafUpdateTimer.current);
        rafUpdateTimer.current = null;
      }
      if (batchUpdateTimer.current) {
        clearTimeout(batchUpdateTimer.current);
        batchUpdateTimer.current = null;
      }
      flushPendingDataUpdates();
      return;
    }

    // Prefer requestAnimationFrame for smooth 60fps updates
    if (!rafUpdateTimer.current && !batchUpdateTimer.current) {
      if (typeof requestAnimationFrame !== 'undefined') {
        rafUpdateTimer.current = requestAnimationFrame(flushPendingDataUpdates);
      } else {
        // Fallback to setTimeout for environments without RAF
        batchUpdateTimer.current = setTimeout(flushPendingDataUpdates, BATCH_INTERVAL_MS);
      }
    }
  }, [flushPendingDataUpdates]);

  // Set the ref to the stable callback
  addDataPointRef.current = addDataPoint;

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
    dataStore.current.removeSignalData(name);
    updateCompatibilityArrays();
    delete signalTypes.current[name];
    delete signalSubscribers.current[name];
  }, [updateCompatibilityArrays]);

  const pruneData = useCallback((max = DEFAULT_MAX_DATA_POINTS) => {
    if (DEBUG) console.log(`Pruning data to last ${max} points`);
    dataStore.current.pruneToLastN(max);
    updateCompatibilityArrays();
  }, [updateCompatibilityArrays]);

  // Fetch signal metadata separately from socket initialization
  useEffect(() => {
    let cancelled = false;
    
    const fetchSignalMetadata = async () => {
      if (DEBUG) console.log("Fetching signal metadata");
      try {
        setIsLoadingSignals(true);
        setSignalError(null);
        const res = await fetch(`${BACKEND_URL}/api/signal`);
        if (!res.ok) throw new Error(`Failed to fetch signals: ${res.status}`);
        const list = await res.json();
        
        if (cancelled) return; // Don't update state if component unmounted
        
        const metas: SignalMeta[] = list.map((s: any) => ({
          name: s.name,
          unit: s.unit,
          cycle_time_ms: s.cycle_time_ms,
          msg_name: s.msg_name,
          msg_id: s.id
        }));
        setAvailableSignals(metas);

        const enumMap: Record<string, Record<string, string>> = {};
        list.forEach((s: any) => {
          if (DEBUG && s.name === 'VC_State') {
            console.log(`VC_State signal metadata:`, s);
            console.log(`VC_State has enum?`, !!s.enum);
            if (s.enum) {
              console.log(`VC_State enum data:`, s.enum);
              console.log(`VC_State enum items:`, s.enum.items);
              console.log(`VC_State enum name:`, s.enum.name);
            }
          }
          if (s.enum?.items) {
            enumMap[s.name] = s.enum.items;
            if (DEBUG && s.name === 'VC_State') {
              console.log(`✓ Added VC_State to enumMap:`, s.enum.items);
            }
          }
        });
        setEnumMetadata(enumMap);
        
        // Update ref for socket handlers
        enumMetadataRef.current = enumMap;

        if (DEBUG) {
          console.log("Fetched enumerations and states:");
          Object.entries(enumMap).forEach(([s, m]) =>
            console.log(`  ${s}: [${Object.values(m).join(", ")}]`)
          );
          console.log(`Available enum signals: ${Object.keys(enumMap).length > 0 ? Object.keys(enumMap).join(', ') : 'None'}`);
          console.log("VC_State in enumMap?", !!enumMap['VC_State']);
        }

        setIsLoadingSignals(false);
      } catch (err: any) {
        if (cancelled) return;
        DEBUG && console.log("Error fetching signals:", err);
        setSignalError(err.message);
        setIsLoadingSignals(false);
        if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
          setTimeout(() => {
            setReconnectAttempts((a) => a + 1);
            // Retry will be handled by the dependency on reconnectAttempts
          }, RECONNECT_INTERVAL);
        }
      }
    };

    fetchSignalMetadata();
    
    return () => {
      cancelled = true;
    };
  }, [reconnectAttempts]); // Only depends on reconnect attempts

  // Centralized subscription management - single effect to handle all subscription changes
  useEffect(() => {
    const socket = socketRef.current;
    if (!socket || !isConnected || isPaused) return;

    // Subscribe to all active signals when they change or when reconnecting
    activeSignals.forEach((sig) => {
      if ((signalSubscribers.current[sig] || 0) > 0) {
        if (DEBUG) {
          console.log(`Subscribing to ${sig}`);
        }
        socket.emit("sub", sig);
      }
    });

    // Cleanup function - unsubscribe when dependencies change
    return () => {
      if (socket && isConnected) {
        activeSignals.forEach((sig) => {
          if (DEBUG) {
            console.log(`Unsubscribing from ${sig} due to dependency change`);
          }
          socket.emit("unsub", sig);
        });
      }
    };
  }, [activeSignals, isConnected, isPaused]);

  // Separate effect to handle pause/resume without affecting the subscription list
  useEffect(() => {
    const socket = socketRef.current;
    if (!socket || !isConnected) return;

    const action = isPaused ? "unsub" : "sub";
    const signalsToProcess = activeSignals.filter((sig) => {
      const refCount = signalSubscribers.current[sig] || 0;
      const notPending = !pendingUnsubscriptions.current.has(sig);
      return refCount > 0 && notPending;
    });

    if (DEBUG && signalsToProcess.length > 0) {
      console.log(
        `${isPaused ? "Pausing" : "Resuming"} ${signalsToProcess.length} signals: [${signalsToProcess.join(", ")}]`
      );
    }

    signalsToProcess.forEach((sig) => {
      if (DEBUG) {
        console.log(
          `${isPaused ? "Pausing data stream" : "Resuming data stream"} for ${sig}`
        );
      }
      socket.emit(action, sig);
    });
  }, [isPaused, isConnected]); // Removed activeSignals dependency to prevent loops

  const reconnect = useCallback(() => {
    DEBUG && console.log("Reconnecting...");
    setReconnectAttempts(0);
    // Socket will auto-reconnect due to socket.io config
    // Just trigger metadata refresh
    setReconnectAttempts((prev) => prev + 1);
  }, []);

  useEffect(() => {
    // REDUCED FREQUENCY TIMER - currentTime is now primarily updated by batching logic
    // This timer serves as a fallback when no data is flowing
    const id = setInterval(() => {
      if (!isPausedRef.current) {
        setCurrentTime(Date.now());
      }
    }, 5000); // Increased to 5 seconds - mainly for UI clock updates when no data flows
    return () => clearInterval(id);
  }, []); // Empty dependency array - only run once

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
      
      if (DEBUG) {
        const hasEnumMetadata = enumMetadata[name] && Object.keys(enumMetadata[name]).length > 0;
        console.log(`Subscribing to ${name}:`);
        console.log(`  - Type requested: ${type}`);
        console.log(`  - Assigned type: ${assigned === SignalType.Enumeration ? "enum" : "numerical"}`);
        console.log(`  - Has enum metadata: ${hasEnumMetadata}`);
        console.log(`  - Pattern match: ${/state|mode|enum|status/.test(name.toLowerCase())}`);
        if (hasEnumMetadata) {
          console.log(`  - Enum values: ${Object.values(enumMetadata[name]).join(', ')}`);
        }
      }
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

      // Only add to activeSignals if this is the first subscriber
      const isFirstSubscriber = signalSubscribers.current[name] === 1;

      if (isFirstSubscriber) {
        setActiveSignals((current) => {
          // Double-check it's not already in the array to prevent duplicates
          if (current.includes(name)) return current;
          return [...current, name];
        });

        // Socket subscription will be handled by the centralized subscription effect
      }
    },
    [isEnumSignal, enumMetadata]
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
        if (socketRef.current && isConnected) {
          socketRef.current.emit("unsub", name);
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
    [pruneSignalData]
  );

  const clearData = () => {
    if (DEBUG) console.log("Clearing data");
    dataStore.current.clear();
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
    if (socketRef.current && isConnected) {
      activeSignals.forEach((sig) => socketRef.current!.emit("unsub", sig));
    }

    // Clear all state
    setActiveSignals([]);
    signalSubscribers.current = {};
    signalTypes.current = {};
    dataStore.current.clear();
    setData([]);
    setNumericalData([]);
    setEnumData([]);
  }, [activeSignals, isConnected]);

  const getEnumValues = useCallback(
    (name: string) =>
      enumMetadataRef.current[name] ? Object.values(enumMetadataRef.current[name]) : [],
    []
  );
  const mapEnumValue = useCallback(
    (name: string, val: number | string) => enumMetadataRef.current[name]?.[String(val)],
    []
  );

  // Optimized query methods
  const getDataInTimeRange = useCallback((startTime: number, endTime: number): DataPoint[] => {
    return dataStore.current.getDataInTimeRange(startTime, endTime);
  }, []);

  const getSignalData = useCallback((signalName: string): DataPoint[] => {
    return dataStore.current.getSignalData(signalName);
  }, []);

  const getDataCount = useCallback((): number => {
    return dataStore.current.getDataCount();
  }, []);

  return (
    <SignalContext.Provider
      value={{
        availableSignals,
        isLoadingSignals,
        signalError,
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

export { SignalType } from "./SignalConfig";
export type { DataPoint } from "./SignalConfig";
