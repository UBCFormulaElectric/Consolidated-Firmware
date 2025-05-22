"use client"

import { useEffect, useState, useMemo, useCallback } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { useSignals, DataPoint } from "@/lib/contexts/SignalContext"

type TelemetryData = {
  id: string
  parameter: string
  value: string | number
  unit: string
  timestamp: number
}

export default function LiveDataTable() {
  const {
    data,
    activeSignals,
    availableSignals,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals
  } = useSignals()
  const [lastUpdate, setLastUpdate] = useState(Date.now())
  const [selectedSignal, setSelectedSignal] = useState<string>("")

  // Update timestamp periodically to refresh the table
  useEffect(() => {
    const interval = setInterval(() => {
      setLastUpdate(Date.now())
    }, 1000)
    return () => clearInterval(interval)
  }, [])

  // Process the data from SignalContext
 const tableData = useMemo((): TelemetryData[] => {
  // if we don't even know what signals exist yet, bail
  if (!availableSignals) return []

  return activeSignals.map((signalName) => {
    const signalMeta = availableSignals.find(meta => meta.name === signalName)
    // pick out all DataPoint objects whose `name` matches this signal
    const entries = data.filter(d => (d as any).name === signalName)
    // grab the very last one, if it exists
    const latest = entries.length > 0
      ? entries[entries.length - 1]
      : null

    return {
      id:        signalName,
      parameter: signalName,
      // unwrap if it exists, otherwise "N/A"
      value:     latest?.value    ?? "N/A",
      unit:      signalMeta?.unit ?? "N/A",
      // use the timestamp if present (ensuring it's a number), otherwise our lastUpdate fallback
      timestamp: latest?.timestamp ? latest.timestamp : lastUpdate
    }
  })
}, [data, activeSignals, availableSignals, lastUpdate])



  // Handle subscription to a new signal
  const handleSubscribe = useCallback(() => {
    if (selectedSignal) {
      console.log(`LiveDataTable - Subscribing to: ${selectedSignal}`)
      subscribeToSignal(selectedSignal)
      setSelectedSignal("")
    }
  }, [selectedSignal, subscribeToSignal])
  
  // Handle unsubscribing from a signal
  const handleUnsubscribe = useCallback((signalName: string) => {
    console.log(`LiveDataTable - Unsubscribing from: ${signalName}`)
    unsubscribeFromSignal(signalName)
  }, [unsubscribeFromSignal])

  // Show message if no signals are active
  if (activeSignals.length === 0) {
    return (
      <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
        <CardHeader>
          <CardTitle>Live Telemetry Data</CardTitle>
        </CardHeader>
        <CardContent>
          <div className="py-8 text-center text-gray-500">
            No signals are active. Subscribe to signals to view telemetry data.
          </div>
          <div className="mt-4">
            <div className="flex flex-wrap gap-2">
              <select
                value={selectedSignal}
                onChange={(e) => setSelectedSignal(e.target.value)}
                className="flex-grow p-2 bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 rounded"
                disabled={isLoadingSignals}
              >
                <option value="">Select a signal...</option>
                {availableSignals.map((signal, index) => (
                  <option key={`${signal.name}-${index}`} value={signal.name}>
                    {signal.name}
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
          </div>
        </CardContent>
      </Card>
    )
  }

  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader>
        <CardTitle>Live Telemetry Data</CardTitle>
      </CardHeader>
      <CardContent>
        <div className="overflow-x-auto mb-6">
          <table className="w-full text-sm">
            <thead>
              <tr className="border-b border-gray-300 dark:border-gray-700">
                <th className="text-left py-2 px-4">Parameter</th>
                <th className="text-left py-2 px-4">Value</th>
                <th className="text-left py-2 px-4">Unit</th>
                <th className="text-left py-2 px-4">Timestamp</th>
                <th className="text-left py-2 px-4">Actions</th>
              </tr>
            </thead>
            <tbody>
              {tableData.map((row) => (
                <tr key={row.id} className="border-b border-gray-200 dark:border-gray-700">
                  <td className="py-2 px-4">{row.parameter}</td>
                  <td className="py-2 px-4">{String(row.value)}</td>
                  <td className="py-2 px-4">{row.unit}</td>
                  <td className="py-2 px-4 text-gray-500 dark:text-gray-400">
                    {new Date(row.timestamp).toLocaleTimeString()}
                  </td>
                  <td className="py-2 px-4">
                    <button
                      onClick={() => handleUnsubscribe(row.parameter)}
                      className="text-red-500 hover:text-red-700"
                    >
                      Unsubscribe
                    </button>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>

        <div className="mt-4 pt-4 border-t border-gray-200 dark:border-gray-700">
          <div className="flex flex-wrap gap-2">
            <select
              value={selectedSignal}
              onChange={(e) => setSelectedSignal(e.target.value)}
              className="flex-grow p-2 bg-white dark:bg-gray-700 border border-gray-300 dark:border-gray-600 rounded"
              disabled={isLoadingSignals}
            >
              <option value="">Select a signal...</option>
              {availableSignals
                .filter(signal => 
                  !activeSignals.includes(signal.name)
                )
                .map((signal, index) => (
                  <option key={`${signal.name}-${index}`} value={signal.name}>
                    {signal.name}
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
        </div>
      </CardContent>
    </Card>
  )
}