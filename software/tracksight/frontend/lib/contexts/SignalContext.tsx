'use client'

import React, { createContext, useContext, useState, useEffect, ReactNode, useCallback, useRef } from 'react'
import io, { Socket } from 'socket.io-client'
import { PausePlayContext } from '@/components/shared/pause-play-control'


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
  pruneSignalData: (signalName: string) => void
  getSignalRefCount: (signalName: string) => number
  clearAllSubscriptions: () => void
  reconnect: () => void
  isEnumSignal: (signalName: string) => boolean
  isNumericalSignal: (signalName: string) => boolean
  getEnumValues: (signalName: string) => string[]
  mapEnumValue: (signalName: string, value: number | string) => string | undefined
}

// Default maximum number of data points to keep
const DEFAULT_MAX_DATA_POINTS = 1000;
const BACKEND_URL = "http://localhost:5000";
const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_INTERVAL = 3000; // 3 seconds
const DEBUG = true; // Set to false to disable debug logs

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined)

// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  // Get pause state from context
  const pausePlayContext = useContext(PausePlayContext)
  const isPaused = pausePlayContext?.isPaused || false
  
  // Debug the pause state
  useEffect(() => {
    if (DEBUG) console.log('SignalProvider - isPaused:', isPaused)
  }, [isPaused])
  
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
  // Holds enum code-to-label maps per signal
  const [enumMetadata, setEnumMetadata] = useState<Record<string, Record<string, string>>>({})

  const signalSubscribers = useRef<Record<string, number>>({})
  const signalTypes = useRef<Record<string, SignalType>>({})

  const isEnumSignal = useCallback((signalName: string): boolean => {
    if (signalTypes.current[signalName]) {
      return signalTypes.current[signalName] === SignalType.Enumeration
    }
    const lower = signalName.toLowerCase()
    return lower.includes('state') || lower.includes('mode') || lower.includes('enum') || lower.includes('status')
  }, [])

  const isNumericalSignal = useCallback((signalName: string): boolean => {
    if (signalTypes.current[signalName]) {
      return signalTypes.current[signalName] === SignalType.Numerical
    }
    return !isEnumSignal(signalName)
  }, [isEnumSignal])

  // Get reference count for a signal
  const getSignalRefCount = useCallback((signalName: string): number => {
    return signalSubscribers.current[signalName] || 0
  }, [])

  // Debug helper to log all current subscriptions
  const logSubscriptionState = useCallback(() => {
    if (DEBUG) {
      console.log('Current subscription state:')
      console.log('Active signals:', activeSignals)
      console.log('Signal ref counts:', Object.entries(signalSubscribers.current)
        .filter(([_, count]) => count > 0)
        .map(([signal, count]) => `${signal}: ${count}`)
        .join(', ') || 'None')
    }
  }, [activeSignals])

  // Prune data for a specific signal
  const pruneSignalData = useCallback((signalName: string) => {
    if (DEBUG) console.log(`Pruning all data for signal: ${signalName}`)
    
    // Remove from all data arrays
    setData(prev => prev.filter(point => point.name !== signalName))
    setNumericalData(prev => prev.filter(point => point.name !== signalName))
    setEnumData(prev => prev.filter(point => point.name !== signalName))
    
    // Clean up signal metadata
    delete signalTypes.current[signalName]
    delete signalSubscribers.current[signalName]
  }, [])

  // Manual prune helper
  const pruneData = useCallback((maxPoints: number = DEFAULT_MAX_DATA_POINTS) => {
    if (DEBUG) console.log(`Pruning data to last ${maxPoints} points`)
    setData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev)
    setNumericalData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev)
    setEnumData(prev => prev.length > maxPoints ? prev.slice(-maxPoints) : prev)
  }, [])

  const fetchSignalMetadata = useCallback(async () => {
    if (DEBUG) console.log('Fetching signal metadata')
    try {
      setIsLoadingSignals(true)
      setSignalError(null)
      const response = await fetch(`${BACKEND_URL}/api/signal`)
      if (!response.ok) throw new Error(`Failed to fetch signals: ${response.status}`)
      const signalsData = await response.json()

      // Extract basic metas
      const metas: SignalMeta[] = signalsData.map((s: any) => ({
        name: s.name,
        unit: s.unit,
        cycle_time_ms: s.cycle_time_ms
      }))
      setAvailableSignals(metas)

      // Build enumMetadata
      const enumMap: Record<string, Record<string, string>> = {}
      signalsData.forEach((s: any) => {
        if (s.enum && s.enum.items) {
          enumMap[s.name] = s.enum.items
        }
      })
      setEnumMetadata(enumMap)

      // Print enumerations and their states
      if (DEBUG) {
        console.log('Fetched enumerations and states:')
        Object.entries(enumMap).forEach(([signal, map]) => {
          console.log(`  ${signal}: [${Object.values(map).join(', ')}]`)
        })
      }

      if (DEBUG) console.log('Fetched signals:', metas)
      setIsLoadingSignals(false)
    } catch (err: any) {
      if (DEBUG) console.log('Error fetching signals:', err)
      setSignalError(err.message)
      setIsLoadingSignals(false)
      if (reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        setTimeout(() => {
          setReconnectAttempts(a => a + 1)
          fetchSignalMetadata()
        }, RECONNECT_INTERVAL)
      }
    }
  }, [reconnectAttempts])

  const initializeSocket = useCallback(() => {
    if (DEBUG) console.log('Initializing socket')
    socket?.disconnect()
    const newSocket = io(BACKEND_URL, {
      reconnectionAttempts: MAX_RECONNECT_ATTEMPTS,
      timeout: 10000
    })

    newSocket.on('connect', () => {
      if (DEBUG) console.log('Socket connected')
      setIsConnected(true)
      setReconnectAttempts(0)
      // Only subscribe to signals if not paused and they have active subscriptions
      if (!pausePlayContext?.isPaused) {
        activeSignals.forEach(sig => {
          const refCount = signalSubscribers.current[sig] || 0
          if (refCount > 0) {
            if (DEBUG) console.log(`Resubscribing to ${sig} on reconnect (ref count: ${refCount})`)
            newSocket.emit('sub', sig)
          }
        })
      }
    })
    newSocket.on('connect_error', err => {
      if (DEBUG) console.log('Socket error:', err)
      setIsConnected(false)
    })
    newSocket.on('disconnect', reason => {
      if (DEBUG) console.log('Socket disconnected:', reason)
      setIsConnected(false)
    })

    const dataHandler = (incoming: any) => {
      if (DEBUG) console.log('Received data:', incoming)
      
      const { name, value } = incoming
      if (!name) return
      
      // Check if we still have subscribers for this signal
      const refCount = signalSubscribers.current[name] || 0
      if (refCount === 0) {
        if (DEBUG) console.log(`Ignoring data for ${name} - no active subscribers`)
        return
      }
      
      if (!incoming.time) incoming.time = Date.now()
      if (typeof value === 'string') incoming.value = parseFloat(value)

      setData(prev => [...prev, incoming])
      if (isEnumSignal(name)) setEnumData(prev => [...prev, incoming])
      else if (!isNaN(incoming.value as number)) setNumericalData(prev => [...prev, incoming])

      setCurrentTime(Date.now())
    }

    newSocket.on('data', dataHandler)
    newSocket.on('sub_ack', () => DEBUG && console.log('Subscription acknowledged'))
    setSocket(newSocket)
    return () => { newSocket.off('data', dataHandler); newSocket.disconnect() }
  }, [activeSignals, isEnumSignal, pausePlayContext?.isPaused])

  const reconnect = useCallback(() => {
    if (DEBUG) console.log('Reconnecting...')
    setReconnectAttempts(0)
    fetchSignalMetadata()
    initializeSocket()
  }, [fetchSignalMetadata, initializeSocket])

  // Initial fetch and socket setup
  useEffect(() => {
    fetchSignalMetadata()
    const cleanup = initializeSocket()
    return cleanup
  }, [])

  // Tick currentTime every 100ms, but only when not paused
  useEffect(() => {
    const id = setInterval(() => {
      if (!isPaused) {
        setCurrentTime(Date.now())
      }
    }, 100)
    return () => clearInterval(id)
  }, [isPaused])

  // Manage subscriptions based on pause state
  useEffect(() => {
    if (!socket || !isConnected) return

    if (isPaused) {
      // Unsubscribe from all signals when paused (but keep ref counts)
      if (DEBUG) console.log('Pausing - unsubscribing from all active signals:', activeSignals)
      activeSignals.forEach(sig => {
        const refCount = signalSubscribers.current[sig] || 0
        if (refCount > 0) {
          socket.emit('unsub', sig)
        }
      })
    } else {
      // Resubscribe to all signals with active subscribers when playing
      if (DEBUG) console.log('Playing - subscribing to signals with active subscribers')
      activeSignals.forEach(sig => {
        const refCount = signalSubscribers.current[sig] || 0
        if (refCount > 0) {
          if (DEBUG) console.log(`Resubscribing to ${sig} (ref count: ${refCount})`)
          socket.emit('sub', sig)
        }
      })
    }
  }, [isPaused, socket, isConnected, activeSignals])

  const subscribeToSignal = useCallback((signalName: string, type: SignalType = SignalType.Any) => {
    const name = signalName.trim()
    
    // Determine signal type
    const assigned = type === SignalType.Any
      ? (isEnumSignal(name) ? SignalType.Enumeration : SignalType.Numerical)
      : type
    
    signalTypes.current[name] = assigned
    
    // Increment reference count
    const currentRefCount = signalSubscribers.current[name] || 0
    signalSubscribers.current[name] = currentRefCount + 1
    
    if (DEBUG) {
      console.log(`Subscribing to ${name} as ${assigned} (ref count: ${signalSubscribers.current[name]})`)
      logSubscriptionState()
    }
    
    // Add to active signals if this is the first subscription
    if (currentRefCount === 0 && !activeSignals.includes(name)) {
      setActiveSignals(prev => [...prev, name])
      
      // Only emit subscription if not paused and socket is connected
      if (socket && isConnected && !isPaused) {
        if (DEBUG) console.log(`Emitting subscription for ${name}`)
        socket.emit('sub', name)
      }
    }
  }, [activeSignals, isConnected, isEnumSignal, socket, isPaused, logSubscriptionState])

  const unsubscribeFromSignal = useCallback((signalName: string) => {
    const name = signalName.trim()
    const currentRefCount = signalSubscribers.current[name] || 0
    
    if (currentRefCount <= 0) {
      if (DEBUG) console.log(`Warning: Attempting to unsubscribe from ${name} but ref count is already 0`)
      return
    }
    
    // Decrement reference count
    const newRefCount = currentRefCount - 1
    signalSubscribers.current[name] = newRefCount
    
    if (DEBUG) {
      console.log(`Unsubscribing from ${name} (ref count: ${newRefCount})`)
      logSubscriptionState()
    }
    
    // If no more subscribers, remove from active signals and prune data
    if (newRefCount === 0) {
      setActiveSignals(prev => prev.filter(sig => sig !== name))
      
      // Always emit unsubscribe regardless of pause state
      if (socket && isConnected) {
        if (DEBUG) console.log(`Emitting unsubscription for ${name}`)
        socket.emit('unsub', name)
      }
      
      // Prune all data for this signal
      pruneSignalData(name)
    }
  }, [socket, isConnected, pruneSignalData, logSubscriptionState])

  const clearData = () => {
    if (DEBUG) console.log('Clearing data')
    setData([])
    setNumericalData([])
    setEnumData([])
  }

  // Clear all subscriptions and data
  const clearAllSubscriptions = useCallback(() => {
    if (DEBUG) console.log('Clearing all subscriptions and data')
    
    // Unsubscribe from all active signals
    activeSignals.forEach(sig => {
      if (socket && isConnected) {
        socket.emit('unsub', sig)
      }
    })
    
    // Clear all state
    setActiveSignals([])
    signalSubscribers.current = {}
    signalTypes.current = {}
    clearData()
  }, [activeSignals, socket, isConnected])

  const getEnumValues = useCallback((signalName: string): string[] => {
    return enumMetadata[signalName]
      ? Object.values(enumMetadata[signalName])
      : []
  }, [enumMetadata])

  const mapEnumValue = useCallback((signalName: string, value: number | string): string | undefined => {
    const items = enumMetadata[signalName]
    return items ? items[String(value)] : undefined
  }, [enumMetadata])

  return (
    <SignalContext.Provider value={{
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
      mapEnumValue
    }}>
      {children}
    </SignalContext.Provider>
  )
}

export function useSignals() {
  const context = useContext(SignalContext)
  if (!context) throw new Error('useSignals must be used within a SignalProvider')
  return context
}