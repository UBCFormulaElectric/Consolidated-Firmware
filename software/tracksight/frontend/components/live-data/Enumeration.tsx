'use client'

import React, { useState, useMemo, useCallback, useEffect } from 'react'
import { useSignals, SignalType } from '@/lib/contexts/SignalContext'
import { usePausePlay } from '@/components/shared/pause-play-control'

interface StateHistoryItem {
  state: string
  startTime: number
}

interface StateBar {
  state: string
  startTime: number
  width: number
  xOffset: number
}

// Colors for the states, mapping to indices
const stateColors = [
  '#FF3B2F',
  '#FFCC02',
  '#FF9500',
  '#35C759',
  '#007AFF',
  '#5856D6',
  '#AF52DE',
  '#FF2D55',
]

export default function EnumerationGraph() {
  const { isPaused } = usePausePlay()
  const {
    availableSignals,
    activeSignals,
    enumData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isEnumSignal,
    getEnumValues,
    mapEnumValue,
  } = useSignals()

  // Track the time when we were paused to freeze the timeline
  const [pausedTime, setPausedTime] = useState<number | null>(null)
  
  // Update paused time when pause state changes
  useEffect(() => {
    if (isPaused && pausedTime === null) {
      setPausedTime(Date.now())
    } else if (!isPaused) {
      setPausedTime(null)
    }
  }, [isPaused, pausedTime])

  // all enum-type signals
  const enumerationOptions = useMemo(
    () => availableSignals.filter(s => isEnumSignal(s.name)),
    [availableSignals, isEnumSignal]
  )

  const [selectedEnum, setSelectedEnum] = useState<string>('')

  // only the enum signals we're subscribed to
  const activeEnums = useMemo(
    () => activeSignals.filter(name => isEnumSignal(name)),
    [activeSignals, isEnumSignal]
  )

  // subscribe handler
  const handleSubscribe = useCallback(() => {
    if (!selectedEnum) return
    subscribeToSignal(selectedEnum, SignalType.Enumeration)
    setSelectedEnum('')
  }, [selectedEnum, subscribeToSignal])

  // build history per signal
  const historyMap: Record<string, StateHistoryItem[]> = {}
  activeEnums.forEach(signalName => {
    const raw = enumData
      .filter(dp => dp.name === signalName && dp.value != null)
      .map(dp => ({
        state: String(dp.value),
        startTime:
          typeof dp.time === 'number' ? dp.time : new Date(dp.time).getTime(),
      }))

    const items: StateHistoryItem[] = []
    raw.forEach(pt => {
      const last = items[items.length - 1]
      if (!last || last.state !== pt.state) {
        items.push(pt)
      }
    })
    historyMap[signalName] = items
  })

  // compute bars without scrolling window: blocks never move, container grows
  const computeBars = (history: StateHistoryItem[]): StateBar[] => {
    const timeWindowMs = 5_000
    const containerWidth = typeof window !== 'undefined' ? window.innerWidth : 1000
    const pxPerMs = containerWidth / timeWindowMs
    // Use paused time if paused, otherwise use current time
    const now = isPaused && pausedTime ? pausedTime : Date.now()

    const rawBars = history
      .map((item, i) => {
        const next = history[i + 1]
        const end = next ? next.startTime : now
        return {
          state: item.state,
          startTime: item.startTime,
          width: (end - item.startTime) * pxPerMs,
        }
      })
      .filter(bar => bar.width > 0)

    let offset = 0
    return rawBars.map(bar => {
      const barWithOffset: StateBar = {
        ...bar,
        xOffset: offset,
      }
      offset += bar.width
      return barWithOffset
    })
  }

  return (
    <div className="space-y-6">
      {/* Pause indicator */}
      {isPaused && (
        <div className="bg-red-500 text-white px-3 py-2 rounded text-sm font-medium">
          ⏸️ Data updates are paused
        </div>
      )}
      
      {/* subscribe controls */}
      <div className="flex gap-2 items-center">
        <select
          className="flex-grow p-2 border rounded"
          value={selectedEnum}
          disabled={isLoadingSignals}
          onChange={e => setSelectedEnum(e.target.value)}
        >
          <option value="">Select enumeration…</option>
          {enumerationOptions
            .filter(opt => !activeEnums.includes(opt.name))
            .map(opt => (
              <option key={opt.name} value={opt.name}>
                {opt.name}
              </option>
            ))}
        </select>
        <button
          className="px-4 py-2 bg-blue-500 text-white rounded disabled:opacity-50"
          disabled={!selectedEnum}
          onClick={handleSubscribe}
        >
          Subscribe
        </button>
      </div>

      {/* one timeline + legend per subscribed enum */}
      {activeEnums.map(signalName => {
        const enumVals = getEnumValues(signalName)
        const history = historyMap[signalName] || []
        const bars = computeBars(history)

        const lastCode = history.length > 0 ? history[history.length - 1].state : undefined
        const lastLabel = lastCode != null ? mapEnumValue(signalName, lastCode) ?? lastCode : undefined

        return (
          <div key={signalName} className="border rounded p-4 space-y-3">
            {/* header */}
            <div className="flex justify-between items-center">
              <h3 className="font-semibold">{signalName}</h3>
              <button className="text-red-500 hover:text-red-700" onClick={() => unsubscribeFromSignal(signalName)}>
                Unsubscribe
              </button>
            </div>

            {/* legend */}
            <div className="flex flex-wrap gap-4 text-xs">
              {enumVals.map((stateLabel, i) => {
                const color = stateColors[i % stateColors.length]
                const isActive = stateLabel === lastLabel
                return (
                  <div key={stateLabel} className="inline-flex items-center gap-1">
                    <span className="w-3 h-3 block rounded" style={{ backgroundColor: color }} />
                    <span className={`${isActive ? 'font-bold text-black' : 'text-gray-500'} transition`}>
                      {stateLabel}
                    </span>
                  </div>
                )
              })}
            </div>

            {/* timeline blocks + time axis */}
            <div className="inline-block">
              <div className="relative">
                <div className="h-6 flex flex-row flex-nowrap">
                  {bars.map((bar, idx) => {
                    const label = mapEnumValue(signalName, bar.state) ?? bar.state
                    const enumIndex = enumVals.indexOf(label)
                    const historyIndex = history.findIndex(item => (mapEnumValue(signalName, item.state) ?? item.state) === label)
                    const idxForColor = enumIndex >= 0 ? enumIndex : historyIndex
                    const color = stateColors[idxForColor % stateColors.length]
                    return (
                      <div
                        key={idx}
                        className="h-6 shrink-0"
                        style={{ width: `${bar.width}px`, backgroundColor: color }}
                        title={`${label} @ ${new Date(bar.startTime).toLocaleTimeString()}`}
                      />
                    )
                  })}
                </div>
                <div className="absolute top-full left-0 h-4">
                  {bars.map((bar, idx) => (
                    <div
                      key={idx}
                      className="absolute whitespace-nowrap text-xs"
                      style={{ left: `${bar.xOffset}px` }}
                    >
                      {new Date(bar.startTime).toLocaleTimeString()}
                    </div>
                  ))}
                </div>
              </div>
            </div>
          </div>
        )
      })}
    </div>
  )
}
