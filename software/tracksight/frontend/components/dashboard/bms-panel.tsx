"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"

export default function BmsPanel() {
  // Generate a grid of cells with random values between 3.0 and 4.2
  const generateCells = (count: number) => {
    return Array.from({ length: count }, (_, i) => {
      const value = 3.0 + Math.random() * 1.2
      // Determine color based on value
      let bgColor = "bg-green-500"
      if (value < 3.3) bgColor = "bg-red-500"
      else if (value < 3.6) bgColor = "bg-yellow-500"

      return { id: i, value: value.toFixed(2), color: bgColor }
    })
  }

  const cells = generateCells(24)

  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <CardTitle className="text-lg font-medium">BMS Cells</CardTitle>
      </CardHeader>
      <CardContent>
        <div className="grid grid-cols-6 gap-1">
          {cells.map((cell) => (
            <div key={cell.id} className={`${cell.color} p-1 text-center text-xs rounded`}>
              {cell.value}V
            </div>
          ))}
        </div>

        <div className="mt-4 grid grid-cols-2 gap-4">
          <div className="space-y-2">
            <div className="text-sm text-gray-400">Power Distribution</div>
            <div className="space-y-1">
              {[1, 2, 3, 4, 5, 6].map((i) => (
                <div key={i} className="flex justify-between">
                  <span className="text-sm">50.5V</span>
                  <span className="text-sm">0.95A</span>
                </div>
              ))}
            </div>
          </div>
          <div className="space-y-2">
            <div className="text-sm text-gray-400">LV Power Distribution</div>
            <div className="border border-green-500 rounded-lg p-2 h-32 flex items-center justify-center">
              <div className="text-center">
                <div className="text-xs text-gray-400">Distribution Loop</div>
              </div>
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  )
}
