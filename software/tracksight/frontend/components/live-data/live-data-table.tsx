"use client"

import { useEffect, useState } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"

type TelemetryData = {
  id: number
  timestamp: string
  parameter: string
  value: string
  unit: string
  status: "normal" | "warning" | "critical"
}

export default function LiveDataTable() {
  const [data, setData] = useState<TelemetryData[]>([])

  useEffect(() => {
    // Generate sample data
    const generateData = (): TelemetryData[] => {
      const parameters = [
        { name: "Motor Temperature", unit: "°C", baseValue: 65, variance: 5 },
        { name: "Battery Voltage", unit: "V", baseValue: 380, variance: 10 },
        { name: "Motor RPM", unit: "rpm", baseValue: 3200, variance: 300 },
        { name: "Vehicle Speed", unit: "km/h", baseValue: 45, variance: 5 },
        { name: "Battery Temperature", unit: "°C", baseValue: 32, variance: 3 },
        { name: "Ambient Temperature", unit: "°C", baseValue: 22, variance: 1 },
        { name: "Throttle Position", unit: "%", baseValue: 65, variance: 10 },
        { name: "Brake Position", unit: "%", baseValue: 10, variance: 20 },
        { name: "Steering Angle", unit: "°", baseValue: 5, variance: 10 },
        { name: "Acceleration X", unit: "g", baseValue: 0.2, variance: 0.1 },
        { name: "Acceleration Y", unit: "g", baseValue: 0.1, variance: 0.1 },
        { name: "Acceleration Z", unit: "g", baseValue: 1.0, variance: 0.1 },
      ]

      return parameters.map((param, index) => {
        const value = param.baseValue + (Math.random() * 2 - 1) * param.variance
        let status: "normal" | "warning" | "critical" = "normal"

        // Determine status based on parameter and value
        if (param.name === "Motor Temperature" && value > 70) {
          status = value > 75 ? "critical" : "warning"
        } else if (param.name === "Battery Voltage" && (value < 360 || value > 400)) {
          status = value < 350 || value > 410 ? "critical" : "warning"
        }

        return {
          id: index,
          timestamp: new Date().toISOString(),
          parameter: param.name,
          value: value.toFixed(1),
          unit: param.unit,
          status,
        }
      })
    }

    setData(generateData())

    // Update data every second
    const interval = setInterval(() => {
      setData(generateData())
    }, 1000)

    return () => clearInterval(interval)
  }, [])

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
              {data.map((row) => (
                <tr key={row.id} className="border-b border-gray-200 dark:border-gray-700">
                  <td className="py-2 px-4">{row.parameter}</td>
                  <td className="py-2 px-4">{row.value}</td>
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
