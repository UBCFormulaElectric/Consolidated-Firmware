"use client"

import { useState } from "react"
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select"

type Fault = {
  id: string
  name: string
  status: "critical" | "warning" | "normal"
}

export default function FaultSelector() {
  const [selectedFault, setSelectedFault] = useState<string>("")

  const faults: Fault[] = [
    { id: "fault1", name: "Motor Fault", status: "critical" },
    { id: "fault2", name: "Battery Fault", status: "critical" },
    { id: "fault3", name: "Power Fault", status: "warning" },
    { id: "fault4", name: "Sensor Fault", status: "normal" },
    { id: "fault5", name: "BMS Fault", status: "critical" },
    { id: "fault6", name: "Connection Fault", status: "warning" },
  ]

  const getStatusColor = (status: Fault["status"]) => {
    switch (status) {
      case "critical":
        return "bg-red-500"
      case "warning":
        return "bg-yellow-500"
      case "normal":
        return "bg-green-500"
      default:
        return "bg-gray-500"
    }
  }

  return (
    <Select value={selectedFault} onValueChange={setSelectedFault}>
      <SelectTrigger className="w-full bg-gray-100 dark:bg-gray-800 border-gray-300 dark:border-gray-700 text-gray-700 dark:text-gray-300">
        <SelectValue placeholder="Select fault to display..." />
      </SelectTrigger>
      <SelectContent className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
        {faults.map((fault) => (
          <SelectItem key={fault.id} value={fault.id} className="flex items-center">
            <div className="flex items-center">
              <span className={`h-2 w-2 rounded-full mr-2 ${getStatusColor(fault.status)}`}></span>
              <span>{fault.name}</span>
            </div>
          </SelectItem>
        ))}
      </SelectContent>
    </Select>
  )
}
