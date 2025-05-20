"use client"

import { useEffect, useState, useMemo } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { useSignals, DataPoint } from "@/lib/contexts/SignalContext"

type TelemetryData = {
  id: string
  parameter: string
  value: string | number
  unit: string
  status: "normal" | "warning" | "critical"
  timestamp: number
}

export default function LiveDataTable() {
  const { data, activeSignals, availableSignals } = useSignals()
  const [lastUpdate, setLastUpdate] = useState(Date.now())

  // Update timestamp periodically to refresh the table
  useEffect(() => {
    const interval = setInterval(() => {
      setLastUpdate(Date.now())
    }, 1000)
    return () => clearInterval(interval)
  }, [])

  // Process the data from SignalContext
  const tableData = useMemo(() => {
    if (!data || data.length === 0) {
      return []
    }

    // Get the most recent data point
    const latestDataPoint = data[data.length - 1]
    const timestamp = Number(latestDataPoint.time)

    // Create a row for each active signal
    return activeSignals.map((signalName): TelemetryData => {
      // Find metadata for this signal
      const signalMeta = availableSignals.find(meta => meta.name === signalName)
      const value = latestDataPoint[signalName]
      const unit = signalMeta?.unit || ""

      // Determine status based on parameter and value
      // This is a placeholder - you would implement your own logic based on your requirements
      let status: "normal" | "warning" | "critical" = "normal"
      const numValue = typeof value === 'number' ? value : parseFloat(String(value))
      
      if (!isNaN(numValue)) {
        // Example rules - customize based on your actual signal ranges
        if (signalName.includes("TEMP") && numValue > 70) {
          status = numValue > 80 ? "critical" : "warning"
        } else if (signalName.includes("VOLTAGE") && (numValue < 10 || numValue > 14)) {
          status = numValue < 9 || numValue > 15 ? "critical" : "warning"
        }
      }

      return {
        id: signalName,
        parameter: signalName,
        value: value !== undefined ? value : "N/A",
        unit,
        status,
        timestamp
      }
    })
  }, [data, activeSignals, availableSignals, lastUpdate])

  const getStatusColor = (status: string) => {
    switch (status) {
      case "critical":
        return "text-red-500"
      case "warning":
        return "text-yellow-500"
      default:
        return "text-green-500"
    }
  }

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
        <div className="overflow-x-auto">
          <table className="w-full text-sm">
            <thead>
              <tr className="border-b border-gray-300 dark:border-gray-700">
                <th className="text-left py-2 px-4">Parameter</th>
                <th className="text-left py-2 px-4">Value</th>
                <th className="text-left py-2 px-4">Unit</th>
                <th className="text-left py-2 px-4">Status</th>
                <th className="text-left py-2 px-4">Timestamp</th>
              </tr>
            </thead>
            <tbody>
              {tableData.map((row) => (
                <tr key={row.id} className="border-b border-gray-200 dark:border-gray-700">
                  <td className="py-2 px-4">{row.parameter}</td>
                  <td className="py-2 px-4">{String(row.value)}</td>
                  <td className="py-2 px-4">{row.unit}</td>
                  <td className={`py-2 px-4 ${getStatusColor(row.status)}`}>
                    {row.status.charAt(0).toUpperCase() + row.status.slice(1)}
                  </td>
                  <td className="py-2 px-4 text-gray-500 dark:text-gray-400">
                    {new Date(row.timestamp).toLocaleTimeString()}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </CardContent>
    </Card>
  )
}
