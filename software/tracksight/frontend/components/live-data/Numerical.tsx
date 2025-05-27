'use client'

import React, { useState, useCallback, useMemo } from 'react'
import { SignalProvider, useSignals } from '@/lib/contexts/SignalContext'
import { usePausePlay } from '@/components/shared/pause-play-control'
import { AreaChart, Area, XAxis, YAxis, Tooltip } from 'recharts'

// Define a palette for signal lines
const signalColors = [
  '#ff4d4f', '#ffa940', '#36cfc9', '#597ef7', '#73d13d',
  '#ff85c0', '#9254de', '#ffc069', '#69c0ff', '#ffec3d'
]

interface GraphInstanceProps {
  onDelete: () => void
}

const GraphInstance: React.FC<GraphInstanceProps> = ({ onDelete }) => {
  const { isPaused } = usePausePlay()
  const {
    availableSignals,
    activeSignals,
    numericalData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isNumericalSignal
  } = useSignals()

  const [selectedSignal, setSelectedSignal] = useState<string>('')
  const [chartHeight, setChartHeight] = useState<number>(256)
  const [scaleFactor, setScaleFactor] = useState<number>(100)

  // Aggregate numerical data by timestamp
  const chartData = useMemo(() => {
    const filtered = numericalData
      .filter(d => activeSignals.includes(d.name as string) && isNumericalSignal(d.name as string))
    if (!filtered.length) return []
    const map: Record<number, any> = {}
    filtered.forEach(d => {
      const t = typeof d.time === 'number' ? d.time : new Date(d.time).getTime()
      if (!map[t]) map[t] = { time: t }
      map[t][d.name as string] = d.value
    })
    return Object.values(map).sort((a, b) => a.time - b.time)
  }, [numericalData, activeSignals, isNumericalSignal, isPaused])

  // Compute which active signals are numerical
  const numericalSignals = useMemo(
    () => activeSignals.filter(sig => isNumericalSignal(sig)),
    [activeSignals, isNumericalSignal]
  )

  // Options for subscription dropdown
  const availableOptions = useMemo(
    () => availableSignals.filter(s => !activeSignals.includes(s.name)),
    [availableSignals, activeSignals]
  )

  // Version 1 width logic
  const pixelPerDataPoint = 50
  const chartWidth = Math.max(chartData.length * pixelPerDataPoint, 100)
  const finalChartWidth = chartWidth * (scaleFactor / 100)

  const handleSubscribe = useCallback(() => {
    if (!selectedSignal) return
    subscribeToSignal(selectedSignal)
    setSelectedSignal('')
  }, [selectedSignal, subscribeToSignal])

  const handleUnsubscribe = useCallback(
    (sig: string) => unsubscribeFromSignal(sig),
    [unsubscribeFromSignal]
  )

  return (
    <div className="mb-6 border p-4 rounded relative">
      {/* Pause indicator */}
      {isPaused && (
        <div className="absolute top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10">
          PAUSED
        </div>
      )}
      
      <button
        onClick={onDelete}
        className="absolute top-2 right-2 text-red-600 hover:text-red-800"
      >
        Delete
      </button>

      {/* Controls for sizing */}
      <div className="flex flex-wrap gap-4 mb-4">
        <div className="flex flex-col">
          <label className="text-sm">Vertical Scale: {chartHeight}px</label>
          <input
            type="range"
            min={100}
            max={600}
            step={50}
            value={chartHeight}
            onChange={e => setChartHeight(Number(e.target.value))}
          />
        </div>
        <div className="flex flex-col">
          <label className="text-sm">Horizontal Scale: {scaleFactor}%</label>
          <input
            type="range"
            min={1}
            max={1000}
            value={scaleFactor}
            onChange={e => setScaleFactor(Number(e.target.value))}
          />
        </div>
      </div>

      {/* Subscription controls */}
      <div className="flex flex-wrap gap-2 mb-4">
        <select
          value={selectedSignal}
          onChange={e => setSelectedSignal(e.target.value)}
          className="flex-grow p-2 bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 rounded"
          disabled={isLoadingSignals}
        >
          <option value="">Select a signal...</option>
          {availableOptions.map((s, i) => (
            <option key={`${s.name}-${i}`} value={s.name}>
              {s.name}
            </option>
          ))}
        </select>
        <button
          onClick={handleSubscribe}
          className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600 disabled:bg-gray-400"
          disabled={!selectedSignal}
        >
          Subscribe
        </button>
      </div>

      {/* Unsubscribe buttons */}
      <div className="flex flex-wrap gap-2 mb-4">
        {numericalSignals.map(sig => (
          <button
            key={sig}
            onClick={() => handleUnsubscribe(sig)}
            className="px-2 py-1 bg-red-100 text-red-700 rounded hover:bg-red-200"
          >
            {sig} ×
          </button>
        ))}
      </div>

      {/* Chart area with version1 width and CSS transition */}
      {chartData.length === 0 ? (
        <div
          className="w-full flex items-center justify-center text-gray-500"
          style={{ height: chartHeight }}
        >
          No numerical data to display
        </div>
      ) : (
        <div
          style={{
            width: finalChartWidth,
            height: chartHeight,
            transition: 'width 100ms ease-out'
          }}
        >
          <AreaChart width={finalChartWidth} height={chartHeight} data={chartData}>
            <XAxis
              dataKey="time"
              tickFormatter={t => new Date(t).toLocaleTimeString()}
              interval={Math.ceil(chartData.length / 10)}
            />
            <YAxis domain={[0, 'auto']} />
            <Tooltip 
              isAnimationActive={false} 
              animationDuration={0}
              labelFormatter={(time) => new Date(time).toLocaleString()}
            />
            {numericalSignals.map((sig, idx) => (
              <Area
                key={sig}
                type="monotone"
                dataKey={sig}
                stroke={signalColors[idx % signalColors.length]}
                fill={signalColors[idx % signalColors.length]}
                strokeWidth={1}
                isAnimationActive={false}
                animationDuration={0}
              />
            ))}
          </AreaChart>
        </div>
      )}
    </div>
  )
}

export default function Numerical() {
  const [graphIds, setGraphIds] = useState<number[]>([0])

  const addGraph = useCallback(
    () => setGraphIds(prev => [...prev, prev.length]),
    []
  )

  const removeGraph = useCallback(
    (id: number) => setGraphIds(prev => prev.filter(gid => gid !== id)),
    []
  )

  return (
    <>
      {graphIds.map(id => (
        <SignalProvider key={id}>
          <GraphInstance onDelete={() => removeGraph(id)} />
        </SignalProvider>
      ))}
      <div className="mt-4">
        <button
          onClick={addGraph}
          className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600"
        >
          Add Chart
        </button>
      </div>
    </>
  )
}
