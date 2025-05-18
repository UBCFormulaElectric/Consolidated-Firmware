"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { Badge } from "@/components/ui/badge"

export default function SystemOverview() {
  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <div className="flex justify-between items-center">
          <CardTitle className="text-lg font-medium">Operational Status</CardTitle>
          <Badge className="bg-green-500 text-white">Online</Badge>
        </div>
      </CardHeader>
      <CardContent>
        <div className="grid grid-cols-2 gap-4">
          <div className="space-y-2">
            <div className="text-sm text-gray-400">System Components</div>
            <div className="grid grid-cols-2 gap-2">
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-green-500"></span>
                <span className="text-sm">BMS</span>
              </div>
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-green-500"></span>
                <span className="text-sm">VC</span>
              </div>
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-green-500"></span>
                <span className="text-sm">DAM</span>
              </div>
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-red-500"></span>
                <span className="text-sm">CRT</span>
              </div>
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-green-500"></span>
                <span className="text-sm">FSM</span>
              </div>
              <div className="flex items-center space-x-2">
                <span className="h-2 w-2 rounded-full bg-green-500"></span>
                <span className="text-sm">RSM</span>
              </div>
            </div>
          </div>
          <div className="space-y-2">
            <div className="text-sm text-gray-400">System Metrics</div>
            <div className="space-y-1">
              <div className="flex justify-between">
                <span className="text-sm">Uptime:</span>
                <span className="text-sm">5d 02h 03m</span>
              </div>
              <div className="flex justify-between">
                <span className="text-sm">Temperature:</span>
                <span className="text-sm">42°C</span>
              </div>
              <div className="flex justify-between">
                <span className="text-sm">Load:</span>
                <span className="text-sm">68%</span>
              </div>
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  )
}
