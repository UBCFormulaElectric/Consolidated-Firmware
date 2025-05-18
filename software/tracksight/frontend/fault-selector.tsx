"use client"

import { useState } from "react"
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select"

export default function FaultSelector() {
  const [selectedFault, setSelectedFault] = useState<string>("")

  // This would typically be populated from an API or database
  const faults = [
    { id: "fault1", name: "Power Supply Failure" },
    { id: "fault2", name: "Network Connection Error" },
    { id: "fault3", name: "Sensor Malfunction" },
    { id: "fault4", name: "System Overload" },
    { id: "fault5", name: "Data Corruption" },
  ]

  return (
    <div className="w-full max-w-md">
      <Select value={selectedFault} onValueChange={setSelectedFault}>
        <SelectTrigger className="w-full bg-gray-50 border-gray-300 text-gray-500 h-9">
          <SelectValue placeholder="Select fault to display..." />
        </SelectTrigger>
        <SelectContent>
          {faults.map((fault) => (
            <SelectItem key={fault.id} value={fault.id}>
              {fault.name}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    </div>
  )
}
