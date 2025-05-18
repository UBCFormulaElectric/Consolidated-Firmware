"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"

type Error = {
  id: string
  message: string
  timestamp: string
  severity: "high" | "medium" | "low"
}

export default function ErrorsPanel() {
  const errors: Error[] = [
    { id: "err1", message: "Power supply voltage out of range", timestamp: "10:23:45", severity: "high" },
    { id: "err2", message: "Communication timeout with sensor node", timestamp: "10:22:30", severity: "medium" },
    { id: "err3", message: "Battery cell 4 temperature high", timestamp: "10:20:15", severity: "high" },
    { id: "err4", message: "Fan speed below threshold", timestamp: "10:18:22", severity: "low" },
    { id: "err5", message: "Data logging service restarted", timestamp: "10:15:10", severity: "medium" },
  ]

  const getSeverityColor = (severity: Error["severity"]) => {
    switch (severity) {
      case "high":
        return "bg-red-500"
      case "medium":
        return "bg-yellow-500"
      case "low":
        return "bg-blue-500"
      default:
        return "bg-gray-500"
    }
  }

  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <div className="flex items-center">
          <div className="h-2 w-2 rounded-full bg-red-500 mr-2"></div>
          <CardTitle className="text-lg font-medium">Errors</CardTitle>
        </div>
      </CardHeader>
      <CardContent>
        <div className="space-y-2">
          {errors.map((error) => (
            <div
              key={error.id}
              className={`${getSeverityColor(error.severity)} rounded-md p-2 text-white text-sm flex justify-between`}
            >
              <span>{error.message}</span>
              <span className="text-xs opacity-80">{error.timestamp}</span>
            </div>
          ))}
        </div>
      </CardContent>
    </Card>
  )
}
