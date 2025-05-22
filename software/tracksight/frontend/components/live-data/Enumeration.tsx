// EnumerationGraph.tsx
'use client'

import React, { useState, useMemo, useCallback } from 'react'
import { SignalProvider, useSignals, SignalType } from '@/lib/contexts/SignalContext'

interface StateHistoryItem {
  state: string
  startTime: number
}

const stateColors: string[] = [
  '#FF3B2F', '#FFCC02', '#FF9500', '#35C759',
  '#007AFF', '#5856D6', '#AF52DE', '#FF2D55'
]

function getStateColor(state: string): string {
  const index = Math.abs(Number(state)) % stateColors.length
  return stateColors[index]
}

export default function EnumerationGraph() {
  const {
    availableSignals,
    activeSignals,
    enumData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isEnumSignal
  } = useSignals()

  // Options for enumeration signals
  const enumerationOptions = useMemo(
    () => availableSignals.filter(s => isEnumSignal(s.name)),
    [availableSignals, isEnumSignal]
  )

  const [selectedEnum, setSelectedEnum] = useState<string>('')

  // Currently subscribed enumeration signals
  const activeEnums = useMemo(
    () => activeSignals.filter(name => isEnumSignal(name)),
    [activeSignals, isEnumSignal]
  )

  // Subscribe to a signal
  const handleSubscribe = useCallback(() => {
    if (selectedEnum) {
      subscribeToSignal(selectedEnum, SignalType.Enumeration)
      setSelectedEnum('')
    }
  }, [selectedEnum, subscribeToSignal])

  // Build history per active enumeration
  const historyMap = useMemo(() => {
    const map: Record<string, StateHistoryItem[]> = {}
    activeEnums.forEach(signalName => {
      const points = enumData
        .filter(dp => dp[signalName] !== undefined)
        .map(dp => ({
          state: String(dp[signalName]),
          startTime: typeof dp.time === 'number' ? dp.time : new Date(dp.time).getTime()
        }))
      const items: StateHistoryItem[] = []
      points.forEach(pt => {
        if (!items.length || items[items.length - 1].state !== pt.state) {
          items.push(pt)
        }
      })
      map[signalName] = items
    })
    return map
  }, [enumData, activeEnums])

  // Compute bars for a given history
  function computeBars(history: StateHistoryItem[]) {
    const windowMs = 10_000
    const widthPx = typeof window !== 'undefined' ? window.innerWidth - 100 : 1000
    const pxPerMs = widthPx / windowMs
    const now = Date.now()
    return history
      .map((item, i) => {
        const next = history[i + 1]
        const end = next ? next.startTime : now
        const dur = Math.max(0, end - item.startTime)
        return {
          state: item.state,
          startTime: item.startTime,
          width: Math.min(dur * pxPerMs, widthPx)
        }
      })
      .filter(bar => now - bar.startTime <= windowMs && bar.width > 0)
  }

  return (
    <div className="space-y-4">
      {/* Subscription controls */}
      <div className="flex items-center gap-2">
        <select
          value={selectedEnum}
          onChange={e => setSelectedEnum(e.target.value)}
          disabled={isLoadingSignals}
          className="p-2 border rounded flex-grow"
        >
          <option value="">Select enumeration...</option>
          {enumerationOptions
            .filter(opt => !activeEnums.includes(opt.name))
            .map(opt => (
              <option key={opt.name} value={opt.name}>
                {opt.name}
              </option>
            ))}
        </select>
        <button
          onClick={handleSubscribe}
          disabled={!selectedEnum}
          className="px-4 py-2 bg-blue-500 text-white rounded disabled:opacity-50"
        >
          Subscribe
        </button>
      </div>

      {/* Render timelines for each active enumeration */}
      {activeEnums.map(signalName => {
        const history = historyMap[signalName] || []
        const bars = computeBars(history)
        return (
          <div key={signalName} className="border p-4 rounded">
            <div className="flex justify-between items-center mb-2">
              <h3 className="font-semibold">{signalName}</h3>
              <button
                onClick={() => unsubscribeFromSignal(signalName)}
                className="text-red-500 hover:text-red-700"
              >
                Unsubscribe
              </button>
            </div>
            <div className="h-6 bg-gray-100 overflow-hidden">
              <div className="flex">
                {bars.map((bar, idx) => (
                  <div
                    key={idx}
                    className="h-6"
                    style={{ width: bar.width, backgroundColor: getStateColor(bar.state) }}
                    title={`${bar.state} @ ${new Date(bar.startTime).toLocaleTimeString()}`}  
                  />
                ))}
              </div>
            </div>
          </div>
        )
      })}
    </div>
  )
}
