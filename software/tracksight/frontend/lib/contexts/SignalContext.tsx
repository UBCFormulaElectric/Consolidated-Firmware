'use client'

import React, { createContext, useContext, useState, useEffect, ReactNode, useCallback, useRef } from 'react'
import io, { Socket } from 'socket.io-client'

// Signal type classification
export enum SignalType {
  Numerical = 'numerical',
  Enumeration = 'enumeration',
  Any = 'any'  // For signals that could be either
}

// Signal metadata interface
export interface SignalMeta {
  name: string
  unit: string
  cycle_time_ms?: number
}

// Data point interface
export interface DataPoint {
  time: number | string
  name?: string
  value?: number | string
  [signalName: string]: number | string | undefined
}

// Context interface
type SignalContextType = {
  availableSignals: SignalMeta[]
  isLoadingSignals: boolean
  signalError: string | null
  socket: Socket | null
  isConnected: boolean
  activeSignals: string[]
  data: DataPoint[]
  numericalData: DataPoint[]
  enumData: DataPoint[]
  currentTime: number
  subscribeToSignal: (signalName: string, type?: SignalType) => void
  unsubscribeFromSignal: (signalName: string) => void
  clearData: () => void
  pruneData: (maxPoints?: number) => void
  reconnect: () => void
  isEnumSignal: (signalName: string) => boolean
  isNumericalSignal: (signalName: string) => boolean
}

// Default maximum number of data points to keep (unused for auto-prune now)
const DEFAULT_MAX_DATA_POINTS = 1000;
const BACKEND_URL = "http://localhost:5000";
const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_INTERVAL = 3000; // 3 seconds

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined)

// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  const [availableSignals, setAvailableSignals] = useState<SignalMeta[]>([])
  const [isLoadingSignals, setIsLoadingSignals] = useState(true)
  const [signalError, setSignalError] = useState<string | null>(null)
  const [socket, setSocket] = useState<Socket | null>(null)
  const [isConnected, setIsConnected] = useState(false)
  const [reconnectAttempts, setReconnectAttempts] = useState(0)
  const [activeSignals, setActiveSignals] = useState<string[]>([])
  const [data, setData] = useState<DataPoint[]>([])
  const [numericalData, setNumericalData] = useState<DataPoint[]>([])
  const [enumData, setEnumData] = useState<DataPoint[]>([])
  const [currentTime, setCurrentTime] = useState(Date.now())

  const signalSubscribers = useRef<Record<string, number>>({});
  const signalTypes = useRef<Record<string, SignalType>>({});

  const isEnumSignal = useCallback((signalName: string): boolean => {
    if (signalTypes.current[signalName]) {
      return signalTypes.current[signalName] === SignalType.Enumeration;
    }
    const lower = signalName.toLowerCase();
    return lower.includes('state') || lower.includes('mode') || lower.includes('enum') || lower.includes('status');
  }, []);

  const isNumericalSignal = useCallback((signalName: string): boolean => {
    if (signalTypes.current[signalName]) {
      return signalTypes.current[signalName] === SignalType.Numerical;
    }
    return !isEnumSignal(signalName);
  }, [isEnumSignal]);

  // Manual prune (unused for auto-prune)
  const pruneData = useCallback((maxPoints: number = DEFAULT_MAX_DATA_POINTS) => {
    setData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev);
    setNumericalData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev);
    setEnumData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev);
  }, []);

  const fetchSignalMetadata = useCallback(async () => {
    try {
      setIsLoadingSignals(true);
      setSignalError(null);
      const response = await fetch(`${BACKEND_URL}/api/signal`);
      if (!response.ok) throw new Error(`Failed to fetch signals: ${response.status}`);
      const signals = await response.json();
      const metas: SignalMeta[] = signals.map((s: any) => ({ name: s.name, unit: s.unit, cycle_time_ms: s.cycle_time_ms }));
      setAvailableSignals(metas);
      setIsLoadingSignals(false);
    } catch (err: any) {
      setSignalError(err.message);
      setIsLoadingSignals(false);
      if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        setTimeout(() => { setReconnectAttempts(a => a + 1); fetchSignalMetadata(); }, RECONNECT_INTERVAL);
      }
    }
  }, [reconnectAttempts]);

  const initializeSocket = useCallback(() => {
    if (socket) socket.disconnect();
    const newSocket = io(BACKEND_URL, { reconnectionAttempts: MAX_RECONNECT_ATTEMPTS, timeout: 10000 });

    newSocket.on('connect', () => {
      setIsConnected(true);
      setReconnectAttempts(0);
      activeSignals.forEach(sig => newSocket.emit('sub', sig));
    });
    newSocket.on('connect_error', () => setIsConnected(false));
    newSocket.on('disconnect', () => setIsConnected(false));

    const dataHandler = (incomingData: any) => {
      const name = incomingData.name;
      if (!name) return;
      if (!incomingData.time) incomingData.time = Date.now();
      if (typeof incomingData.value === 'string') incomingData.value = parseFloat(incomingData.value);

      // Always append data without auto-prune
      setData(prev => [...prev, incomingData]);

      if (isEnumSignal(name)) {
        setEnumData(prev => [...prev, incomingData]);
      } else {
        if (!isNaN(incomingData.value as number)) {
          setNumericalData(prev => [...prev, incomingData]);
        }
      }

      setCurrentTime(Date.now());
    };

    newSocket.on('data', dataHandler);
    newSocket.on('sub_ack', () => {});
    setSocket(newSocket);
    return () => { newSocket.off('data', dataHandler); newSocket.disconnect(); };
  }, [activeSignals, isEnumSignal, socket]);

  const reconnect = useCallback(() => {
    setReconnectAttempts(0);
    fetchSignalMetadata();
    initializeSocket();
  }, [fetchSignalMetadata, initializeSocket]);

  useEffect(() => { fetchSignalMetadata(); const cleanup = initializeSocket(); return cleanup; }, []);
  useEffect(() => { const id = setInterval(() => setCurrentTime(Date.now()), 100); return () => clearInterval(id); }, []);

  const subscribeToSignal = useCallback((signalName: string, type: SignalType = SignalType.Any) => {
    const name = signalName.trim();
    const assigned = type !== SignalType.Any ? type : (isEnumSignal(name) ? SignalType.Enumeration : SignalType.Numerical);
    signalTypes.current[name] = assigned;
    if (!activeSignals.some(sig => sig.toLowerCase() === name.toLowerCase())) {
      setActiveSignals(prev => [...prev, name]);
      if (socket && isConnected) socket.emit('sub', name);
    }
    signalSubscribers.current[name] = (signalSubscribers.current[name] || 0) + 1;
  }, [activeSignals, isConnected, socket, isEnumSignal]);

  const unsubscribeFromSignal = useCallback((signalName: string) => {
    const name = signalName.trim();
    signalSubscribers.current[name] = (signalSubscribers.current[name] || 1) - 1;
    if (signalSubscribers.current[name] <= 0) {
      setActiveSignals(prev => prev.filter(sig => sig !== name));
      if (socket && isConnected) socket.emit('unsub', name);
    }
  }, [activeSignals, isConnected, socket]);

  const clearData = () => { setData([]); setNumericalData([]); setEnumData([]); };

  return (
    <SignalContext.Provider value={{ availableSignals, isLoadingSignals, signalError, socket, isConnected, activeSignals, data, numericalData, enumData, currentTime, subscribeToSignal, unsubscribeFromSignal, clearData, pruneData, reconnect, isEnumSignal, isNumericalSignal }}>
      {children}
    </SignalContext.Provider>
  );
}

export function useSignals() {
  const context = useContext(SignalContext);
  if (!context) throw new Error('useSignals must be used within a SignalProvider');
  return context;
}
