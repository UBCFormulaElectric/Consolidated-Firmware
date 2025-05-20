'use client'

import React, { createContext, useContext, useState, useEffect, ReactNode, useCallback, useRef } from 'react'
import io, { Socket } from 'socket.io-client'

// Signal metadata interface
export interface SignalMeta {
  name: string
  unit: string
  cycle_time_ms?: number
}

// Data point interface
export interface DataPoint {
  time: number | string
  [signalName: string]: number | string | undefined
}

// Context interface
interface SignalContextType {
  // Signal metadata
  availableSignals: SignalMeta[]
  isLoadingSignals: boolean
  signalError: string | null
  
  // WebSocket and data handling
  socket: Socket | null
  isConnected: boolean
  activeSignals: string[]
  data: DataPoint[]
  currentTime: number
  
  // Methods for signal subscription management
  subscribeToSignal: (signalName: string) => void
  unsubscribeFromSignal: (signalName: string) => void
  clearData: () => void
  pruneData: (maxPoints?: number) => void
  reconnect: () => void
}

// Default maximum number of data points to keep
const DEFAULT_MAX_DATA_POINTS = 1000;
const BACKEND_URL = "http://localhost:5000";
const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_INTERVAL = 3000; // 3 seconds

// Create the context with a default value
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
  const [currentTime, setCurrentTime] = useState(Date.now())
  
  // Track subscribers for each signal to handle multiple components using the same signal
  const signalSubscribers = useRef<Record<string, number>>({});
  
  // Function to prune data to prevent memory issues
  const pruneData = useCallback((maxPoints: number = DEFAULT_MAX_DATA_POINTS) => {
    setData(prevData => {
      if (prevData.length > maxPoints) {
        console.log(`Pruning data from ${prevData.length} to ${maxPoints} points`);
        return prevData.slice(-maxPoints);
      }
      return prevData;
    });
  }, []);
  
  // Fetch signal metadata
  const fetchSignalMetadata = useCallback(async () => {
    try {
      setIsLoadingSignals(true);
      setSignalError(null);
      
      const response = await fetch(`${BACKEND_URL}/api/signal`);
      
      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.status}`);
      }
      
      const signals = await response.json();
      console.log("API Response from /api/signal:", signals);
      
      const signalsMeta: SignalMeta[] = signals.map((signal: any) => ({
        name: signal.name,
        unit: signal.unit,
        cycle_time_ms: signal.cycle_time_ms,
      }));
      
      console.log("Processed signal metadata:", signalsMeta);
      setAvailableSignals(signalsMeta);
      setIsLoadingSignals(false);
    } catch (error: any) {
      console.error("Error fetching signals:", error);
      setSignalError(error.message);
      setIsLoadingSignals(false);
      
      // Try again after a delay if there was an error
      if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        setTimeout(() => {
          setReconnectAttempts(prev => prev + 1);
          fetchSignalMetadata();
        }, RECONNECT_INTERVAL);
      }
    }
  }, [reconnectAttempts]);
  
  // Initialize or reconnect socket
  const initializeSocket = useCallback(() => {
    if (socket) {
      console.log("Closing existing socket connection before reconnecting");
      socket.disconnect();
    }
    
    console.log("Initializing socket connection...");
    const newSocket = io(BACKEND_URL, {
      reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
      timeout: 10000, // 10 seconds timeout
    });
    
    newSocket.on("connect", () => {
      console.log("Connected to WebSocket server");
      setIsConnected(true);
      setReconnectAttempts(0);
      
      // Resubscribe to any active signals
      activeSignals.forEach(signal => {
        console.log(`Resubscribing to signal: ${signal}`);
        newSocket.emit("sub", signal);
      });
    });
    
    newSocket.on("connect_error", (error) => {
      console.error("WebSocket connection error:", error.message);
      setIsConnected(false);
    });
    
    newSocket.on("disconnect", (reason) => {
      console.log(`Disconnected from WebSocket server: ${reason}`);
      setIsConnected(false);
    });
    
    // Handle incoming data
    const dataHandler = (incomingData: any) => {
      console.log("Received data:", incomingData);
      setData(prevData => {
        const newData = [...prevData, incomingData];
        // Auto-prune if data exceeds the limit
        if (newData.length > DEFAULT_MAX_DATA_POINTS) {
          console.log(`Auto-pruning data: ${newData.length} > ${DEFAULT_MAX_DATA_POINTS}`);
          return newData.slice(-DEFAULT_MAX_DATA_POINTS);
        }
        return newData;
      });
      setCurrentTime(Date.now());
    };
    
    newSocket.on("data", dataHandler);
    
    // Socket acknowledgment handlers
    newSocket.on("sub_ack", (ack: { signal: string; status: string }) => {
      console.log(`Subscription acknowledgment: Signal ${ack.signal} - Status: ${ack.status}`);
    });
    
    setSocket(newSocket);
    
    return () => {
      newSocket.off("data", dataHandler);
      newSocket.off("sub_ack");
      newSocket.disconnect();
    };
  }, [activeSignals]);
  
  // Manual reconnect function
  const reconnect = useCallback(() => {
    setReconnectAttempts(0);
    fetchSignalMetadata();
    initializeSocket();
  }, [fetchSignalMetadata, initializeSocket]);
  
  // Initial setup
  useEffect(() => {
    fetchSignalMetadata();
    const cleanup = initializeSocket();
    
    return cleanup;
  }, []);
  
  // Update current time regularly
  useEffect(() => {
    const timeInterval = setInterval(() => {
      setCurrentTime(Date.now());
    }, 100);
    
    return () => {
      clearInterval(timeInterval);
    };
  }, []);
  
  // Subscribe to a signal
  const subscribeToSignal = useCallback((signalName: string) => {
    // Normalize signal name to handle case sensitivity issues
    const normalizedName = signalName.trim();
    
    // Check if already in active signals (case insensitive)
    const isAlreadyActive = activeSignals.some(
      signal => signal.toLowerCase() === normalizedName.toLowerCase()
    );
    
    if (!isAlreadyActive) {
      console.log(`Subscribing to signal: ${normalizedName}`);
      setActiveSignals(prev => [...prev, normalizedName]);
      
      if (socket && isConnected) {
        socket.emit("sub", normalizedName);
      } else {
        console.warn(`Cannot subscribe to ${normalizedName}: socket not connected`);
      }
    }
    
    // Increment subscriber count
    signalSubscribers.current[normalizedName] = (signalSubscribers.current[normalizedName] || 0) + 1;
    console.log(`Signal ${normalizedName} now has ${signalSubscribers.current[normalizedName]} subscribers`);
  }, [socket, isConnected, activeSignals]);
  
  // Unsubscribe from a signal
  const unsubscribeFromSignal = useCallback((signalName: string) => {
    // Normalize signal name
    const normalizedName = signalName.trim();
    
    console.log(`Request to unsubscribe from signal: ${normalizedName}`);
    
    // Decrement subscriber count
    if (signalSubscribers.current[normalizedName]) {
      signalSubscribers.current[normalizedName] -= 1;
    }
    
    console.log(`Signal ${normalizedName} now has ${signalSubscribers.current[normalizedName] || 0} subscribers`);
    
    // Only remove from active signals if no subscribers left
    if (!signalSubscribers.current[normalizedName] || signalSubscribers.current[normalizedName] <= 0) {
      console.log(`Removing ${normalizedName} from active signals as it has no subscribers left`);
      
      // Find the actual case-preserved signal name in the activeSignals array
      const signalToRemove = activeSignals.find(
        signal => signal.toLowerCase() === normalizedName.toLowerCase()
      );
      
      if (signalToRemove) {
        // Remove from active signals
        setActiveSignals(prev => prev.filter(signal => signal !== signalToRemove));
        
        // Unsubscribe from the WebSocket
        if (socket && isConnected) {
          console.log(`Emitting 'unsub' for ${signalToRemove} to the WebSocket`);
          socket.emit("unsub", signalToRemove);
        }
        
        // Reset subscriber count to be safe
        signalSubscribers.current[normalizedName] = 0;
      } else {
        console.warn(`Could not find signal ${normalizedName} in active signals to remove`);
      }
    } else {
      console.log(`Not removing ${normalizedName} from active signals as it still has ${signalSubscribers.current[normalizedName]} subscribers`);
      console.log(`Current subscribers list:`, signalSubscribers.current);
    }
  }, [socket, isConnected, activeSignals]);
  
  // Clear all data points
  const clearData = () => {
    setData([]);
  };
  
  // Context value
  const contextValue: SignalContextType = {
    availableSignals,
    isLoadingSignals,
    signalError,
    socket,
    isConnected,
    activeSignals,
    data,
    currentTime,
    subscribeToSignal,
    unsubscribeFromSignal,
    clearData,
    pruneData,
    reconnect
  };
  
  return (
    <SignalContext.Provider value={contextValue}>
      {children}
    </SignalContext.Provider>
  );
}

// Custom hook for using the context
export function useSignals() {
  const context = useContext(SignalContext);
  
  if (context === undefined) {
    throw new Error('useSignals must be used within a SignalProvider');
  }
  
  return context
}
