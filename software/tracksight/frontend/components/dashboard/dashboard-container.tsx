"use client"

import { useState } from "react"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import FaultSelector from "@/components/dashboard/fault-selector"
import SystemOverview from "@/components/dashboard/system-overview"
import BmsPanel from "@/components/dashboard/bms-panel"
import ErrorsPanel from "@/components/dashboard/errors-panel"
import SensorPanel from "@/components/dashboard/sensor-panel"
import TimeSeriesChart from "@/components/dashboard/time-series-chart"
import FileViewer from "@/components/dashboard/file-viewer"

interface DashboardProps {
  activePage: string
}

export default function Dashboard({ activePage }: DashboardProps) {
  const [activeTab, setActiveTab] = useState("overview")

  // Only render dashboard content for the dashboard page
  if (activePage !== "dashboard") {
    return (
      <div className="p-6 text-gray-800 dark:text-white">
        <h1 className="text-2xl font-bold mb-4">
          {activePage.replace("-", " ").replace(/\b\w/g, (l) => l.toUpperCase())}
        </h1>
        <p>This page is under construction.</p>
      </div>
    )
  }

  return (
    <div className="p-4 text-gray-800 dark:text-white">
      <div className="mb-4 max-w-md">
        {/* <FaultSelector /> */}
      </div>

      <Tabs value={activeTab} onValueChange={setActiveTab} className="w-full">
        <div className="border-b border-gray-300 dark:border-gray-700 mb-4">
          {/* <TabsList className="bg-white dark:bg-gray-800">
            <TabsTrigger value="overview">Overview</TabsTrigger>
            <TabsTrigger value="dashboard">Dashboard</TabsTrigger>
          </TabsList> */}
          <h1>Go to Live Data</h1>
        </div>

        <TabsContent value="overview" className="space-y-4">
          <div className="grid grid-cols-1 lg:grid-cols-2 gap-4">
            {/* <TimeSeriesChart /> */}
            {/* <SystemOverview /> */}
          </div>

          <div className="grid grid-cols-1 lg:grid-cols-2 gap-4">
            {/* <ErrorsPanel />
            <BmsPanel /> */}
          </div>
        </TabsContent>

        <TabsContent value="dashboard" className="space-y-4">
          <div className="grid grid-cols-1 lg:grid-cols-2 gap-4">
            {/* <SensorPanel />
            <FileViewer /> */}
          </div>

          <div className="grid grid-cols-1 gap-4">
            {/* <TimeSeriesChart height={300} /> */}
          </div>
        </TabsContent>
      </Tabs>
    </div>
  )
}
