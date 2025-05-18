"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"

export default function SensorPanel() {
  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <CardTitle className="text-lg font-medium">Sensors</CardTitle>
      </CardHeader>
      <CardContent>
        <div className="grid grid-cols-3 gap-4 p-4 bg-gray-200 dark:bg-gray-900 rounded-lg">
          <SensorButton label="Up" position="top-center" />
          <SensorButton label="Down" position="bottom-center" />
          <SensorButton label="Left" position="middle-left" />
          <SensorButton label="Right" position="middle-right" />
          <SensorButton label="Center" position="center" />
          <SensorButton label="10Ohm" position="bottom-right" />
          <SensorButton label="50Ohm" position="bottom-left" />
          <SensorButton label="100Ohm" position="top-left" />
          <SensorButton label="1kOhm" position="top-right" />
        </div>
      </CardContent>
    </Card>
  )
}

function SensorButton({ label, position }: { label: string; position: string }) {
  // Position mapping for grid placement
  const positionClasses: Record<string, string> = {
    "top-left": "col-start-1 row-start-1",
    "top-center": "col-start-2 row-start-1",
    "top-right": "col-start-3 row-start-1",
    "middle-left": "col-start-1 row-start-2",
    center: "col-start-2 row-start-2",
    "middle-right": "col-start-3 row-start-2",
    "bottom-left": "col-start-1 row-start-3",
    "bottom-center": "col-start-2 row-start-3",
    "bottom-right": "col-start-3 row-start-3",
  }

  return (
    <div className={`${positionClasses[position]} flex items-center justify-center`}>
      <button className="bg-gray-300 dark:bg-gray-700 hover:bg-gray-400 dark:hover:bg-gray-600 text-gray-800 dark:text-white rounded-md p-2 text-xs w-full">
        {label}
      </button>
    </div>
  )
}
