"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"

export default function UserGuidePage() {
  return (
    <div className="p-6 text-gray-800 dark:text-white">
      <h1 className="text-2xl font-bold mb-4">User Guide</h1>

      <div className="grid gap-6">
        <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
          <CardHeader>
            <CardTitle>Getting Started</CardTitle>
          </CardHeader>
          <CardContent>
            <p className="mb-4">
              Welcome to the UBC Formula Electric monitoring system. This guide will help you understand how to use the
              dashboard effectively.
            </p>
            <h3 className="text-lg font-medium mb-2">Basic Navigation</h3>
            <ul className="list-disc pl-5 space-y-1">
              <li>Use the sidebar to navigate between different sections</li>
              <li>The Dashboard provides an overview of all systems</li>
              <li>Live Data shows real-time telemetry from the vehicle</li>
              <li>Logging allows you to view and download historical data</li>
              <li>Settings lets you configure the dashboard and alerts</li>
            </ul>
          </CardContent>
        </Card>

        <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
          <CardHeader>
            <CardTitle>Dashboard Overview</CardTitle>
          </CardHeader>
          <CardContent>
            <p className="mb-2">The dashboard is divided into several key components:</p>
            <ul className="list-disc pl-5 space-y-1">
              <li>
                <strong>System Metrics:</strong> Real-time charts of key performance indicators
              </li>
              <li>
                <strong>Operational Status:</strong> Current state of all vehicle systems
              </li>
              <li>
                <strong>BMS Cells:</strong> Battery management system cell voltages
              </li>
              <li>
                <strong>Errors:</strong> Active errors and warnings from the vehicle
              </li>
            </ul>
          </CardContent>
        </Card>
      </div>
    </div>
  )
}
