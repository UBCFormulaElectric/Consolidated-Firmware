'use client'

import React, { createContext, useContext, useState, useEffect, ReactNode, useCallback, useRef } from 'react'
import io, { Socket } from 'socket.io-client'
import { PausePlayContext } from '@/components/shared/pause-play-control'
import {
  SignalType,
  SignalMeta,
  DataPoint,
  DEFAULT_MAX_DATA_POINTS,
  BACKEND_URL,
  MAX_RECONNECT_ATTEMPTS,
  RECONNECT_INTERVAL,
  DEBUG
} from './SignalConfig'

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

// Create the context
const SignalContext = createContext<SignalContextType | undefined>(undefined)

// Provider component
export function SignalProvider({ children }: { children: ReactNode }) {
  const { isPaused = false } = useContext(PausePlayContext) || {}
  useEffect(() => { if (DEBUG) console.log('SignalProvider - isPaused:', isPaused) }, [isPaused])

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
  const [enumMetadata, setEnumMetadata] = useState<Record<string, Record<string, string>>>({})

  const signalSubscribers = useRef<Record<string, number>>({})
  const signalTypes = useRef<Record<string, SignalType>>({})

  const isEnumSignal = useCallback((name: string) => {
    const type = signalTypes.current[name]
    if (type) return type === SignalType.Enumeration
    const l = name.toLowerCase()
    return /state|mode|enum|status/.test(l)
  }, [])

  const isNumericalSignal = useCallback((name: string) => {
    const type = signalTypes.current[name]
    return type ? type === SignalType.Numerical : !isEnumSignal(name)
  }, [isEnumSignal])

  const getSignalRefCount = useCallback((name: string) => signalSubscribers.current[name] || 0, [])

  const logSubscriptionState = useCallback(() => {
    if (!DEBUG) return
    console.log('Current subscription state:')
    console.log('Active signals:', activeSignals)
    const refs = Object.entries(signalSubscribers.current)
      .filter(([, c]) => c > 0)
      .map(([s, c]) => `${s}: ${c}`)
      .join(', ') || 'None'
    console.log('Signal ref counts:', refs)
  }, [activeSignals])

  const pruneSignalData = useCallback((name: string) => {
    if (DEBUG) console.log(`Pruning all data for signal: ${name}`)
    setData(d => d.filter(p => p.name !== name))
    setNumericalData(d => d.filter(p => p.name !== name))
    setEnumData(d => d.filter(p => p.name !== name))
    delete signalTypes.current[name]
    delete signalSubscribers.current[name]
  }, [])

  const pruneData = useCallback((max = DEFAULT_MAX_DATA_POINTS) => {
    if (DEBUG) console.log(`Pruning data to last ${max} points`)
    setData(d => d.length > max ? d.slice(-max) : d)
    setNumericalData(d => d.length > max ? d.slice(-max) : d)
    setEnumData(d => d.length > max ? d.slice(-max) : d)
  }, [])

  const fetchSignalMetadata = useCallback(async () => {
    if (DEBUG) console.log('Fetching signal metadata')
    try {
      setIsLoadingSignals(true)
      setSignalError(null)
      const res = await fetch(`${BACKEND_URL}/api/signal`)
      if (!res.ok) throw new Error(`Failed to fetch signals: ${res.status}`)
      const list = await res.json()
      const metas: SignalMeta[] = list.map((s: any) => ({ name: s.name, unit: s.unit, cycle_time_ms: s.cycle_time_ms }))
      setAvailableSignals(metas)

      const enumMap: Record<string, Record<string, string>> = {}
      list.forEach((s: any) => s.enum?.items && (enumMap[s.name] = s.enum.items))
      setEnumMetadata(enumMap)

      if (DEBUG) {
        console.log('Fetched enumerations and states:')
        Object.entries(enumMap).forEach(([s, m]) => console.log(`  ${s}: [${Object.values(m).join(', ')}]`))
        console.log('Fetched signals:', metas)
      }

      setIsLoadingSignals(false)
    } catch (err: any) {
      DEBUG && console.log('Error fetching signals:', err)
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
    DEBUG && console.log('Initializing socket')
    socket?.disconnect()
    const s = io(BACKEND_URL, { reconnectionAttempts: MAX_RECONNECT_ATTEMPTS, timeout: 10000 })

    s.on('connect', () => {
      DEBUG && console.log('Socket connected')
      setIsConnected(true)
      setReconnectAttempts(0)
      if (!isPaused) activeSignals.forEach(sig => {
        if ((signalSubscribers.current[sig] || 0) > 0) {
          if (DEBUG) {
            console.log(`Resubscribing to ${sig} on reconnect`)
          }
          s.emit('sub', sig)
        }
      })
    })
    s.on('connect_error', e => { DEBUG && console.log('Socket error:', e); setIsConnected(false) })
    s.on('disconnect', r => { DEBUG && console.log('Socket disconnected:', r); setIsConnected(false) })

    const handler = (inc: any) => {
      DEBUG && console.log('Received data:', inc)
      const { name, value } = inc
      if (!name) return
      if ((signalSubscribers.current[name] || 0) === 0) return DEBUG && console.log(`Ignoring data for ${name}`)
      inc.time ||= Date.now()
      if (typeof inc.value === 'string') inc.value = parseFloat(inc.value)
      setData(d => [...d, inc])
      isEnumSignal(name) ? setEnumData(d => [...d, inc]) : !isNaN(inc.value as number) && setNumericalData(d => [...d, inc])
      setCurrentTime(Date.now())
    }

    s.on('data', handler)
    s.on('sub_ack', () => DEBUG && console.log('Subscription acknowledged'))
    setSocket(s)
    return () => { s.off('data', handler); s.disconnect() }
  }, [activeSignals, isEnumSignal, isPaused, socket])

  const reconnect = useCallback(() => { DEBUG && console.log('Reconnecting...'); setReconnectAttempts(0); fetchSignalMetadata(); initializeSocket() }, [fetchSignalMetadata, initializeSocket])

  useEffect(() => { fetchSignalMetadata(); const clean = initializeSocket(); return clean }, [])

  useEffect(() => {
    const id = setInterval(() => !isPaused && setCurrentTime(Date.now()), 100)
    return () => clearInterval(id)
  }, [isPaused])

  useEffect(() => {
    if (!socket || !isConnected) return
    const action = isPaused ? 'unsub' : 'sub'
    DEBUG && console.log(isPaused ? 'Pausing' : 'Playing')
    activeSignals.forEach(sig => {
      if ((signalSubscribers.current[sig] || 0) > 0) {
        DEBUG && console.log(`${isPaused ? 'Emitting unsubscription' : 'Resubscribing'} for ${sig}`)
        socket.emit(action, sig)
      }
    })
  }, [isPaused, socket, isConnected, activeSignals])

  const subscribeToSignal = useCallback((signalName: string, type: SignalType = SignalType.Any) => {
    const name = signalName.trim()
    const assigned = type === SignalType.Any ? (isEnumSignal(name) ? SignalType.Enumeration : SignalType.Numerical) : type
    signalTypes.current[name] = assigned
    signalSubscribers.current[name] = (signalSubscribers.current[name] || 0) + 1
    if (DEBUG) {
      console.log(`Subscribing to ${name}`)
      logSubscriptionState()
    }
    if (!activeSignals.includes(name) && signalSubscribers.current[name] === 1) {
      setActiveSignals(a => [...a, name])
      if (socket && isConnected && !isPaused) socket.emit('sub', name)
    }
  }, [activeSignals, isConnected, isEnumSignal, isPaused, logSubscriptionState, socket])

  const unsubscribeFromSignal = useCallback((signalName: string) => {
    const name = signalName.trim()
    const count = (signalSubscribers.current[name] || 0) - 1
    if (count < 0) return DEBUG && console.log(`Warning: Unsubscribe ${name} ref count <= 0`)
    signalSubscribers.current[name] = count
    if (DEBUG) {
      console.log(`Unsubscribing from ${name}`)
      logSubscriptionState()
    }
    if (count === 0) {
      setActiveSignals(a => a.filter(s => s !== name))
      socket && isConnected && socket.emit('unsub', name)
      pruneSignalData(name)
    }
  }, [isConnected, pruneSignalData, logSubscriptionState, socket])

  const clearData = () => {
    if (DEBUG) console.log('Clearing data');
    setData([]);
    setNumericalData([]);
    setEnumData([]);
  }

  const clearAllSubscriptions = useCallback(() => {
    DEBUG && console.log('Clearing all subscriptions and data')
    activeSignals.forEach(sig => socket && isConnected && socket.emit('unsub', sig))
    setActiveSignals([])
    signalSubscribers.current = {}
    signalTypes.current = {}
    clearData()
  }, [activeSignals, isConnected, socket])

  const getEnumValues = useCallback((name: string) => enumMetadata[name] ? Object.values(enumMetadata[name]) : [], [enumMetadata])
  const mapEnumValue = useCallback((name: string, val: number | string) => enumMetadata[name]?.[String(val)], [enumMetadata])

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

export { SignalType } from './SignalConfig';
export type { DataPoint } from './SignalConfig';