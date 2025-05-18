"use client"

import { useEffect, useState } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import dynamic from "next/dynamic"
import { useTheme } from "next-themes"

// Dynamically import Plotly to avoid SSR issues
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false })

interface TimeSeriesChartProps {
  height?: number
}

export default function TimeSeriesChart({ height = 250 }: TimeSeriesChartProps) {
  const [data, setData] = useState<any[]>([])
  const { resolvedTheme } = useTheme()
  const [mounted, setMounted] = useState(false)

  // Only render after mounting to avoid hydration mismatch
  useEffect(() => {
    setMounted(true)
  }, [])

  useEffect(() => {
    // Generate sample data
    const generateData = () => {
      const now = new Date()
      const times = Array.from({ length: 100 }, (_, i) => {
        const time = new Date(now.getTime() - (99 - i) * 1000)
        return time
      })

      // Create sine wave with some noise
      const values1 = times.map((_, i) => Math.sin(i / 10) + Math.random() * 0.2)
      const values2 = times.map((_, i) => Math.cos(i / 10) + Math.random() * 0.2)
      const values3 = times.map((_, i) => Math.sin(i / 20) + Math.random() * 0.2)

      return [
        {
          x: times,
          y: values1,
          type: "scatter",
          mode: "lines",
          name: "Signal 1",
          line: { color: "#4ade80" },
        },
        {
          x: times,
          y: values2,
          type: "scatter",
          mode: "lines",
          name: "Signal 2",
          line: { color: "#60a5fa" },
        },
        {
          x: times,
          y: values3,
          type: "scatter",
          mode: "lines",
          name: "Signal 3",
          line: { color: "#f97316" },
        },
      ]
    }

    setData(generateData())

    // Update data every 5 seconds
    const interval = setInterval(() => {
      setData(generateData())
    }, 5000)

    return () => clearInterval(interval)
  }, [])

  const isDark = resolvedTheme === "dark"

  const layout = {
    height: height,
    margin: { l: 40, r: 20, t: 10, b: 30 },
    paper_bgcolor: "rgba(0,0,0,0)",
    plot_bgcolor: "rgba(0,0,0,0)",
    font: {
      color: isDark ? "#e5e7eb" : "#374151",
    },
    xaxis: {
      gridcolor: isDark ? "#374151" : "#e5e7eb",
      zerolinecolor: isDark ? "#374151" : "#e5e7eb",
    },
    yaxis: {
      gridcolor: isDark ? "#374151" : "#e5e7eb",
      zerolinecolor: isDark ? "#374151" : "#e5e7eb",
    },
    legend: {
      orientation: "h",
      y: 1.1,
    },
    showlegend: true,
    autosize: true,
  }

  const config = {
    responsive: true,
    displayModeBar: false,
  }

  if (!mounted) {
    return (
      <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
        <CardHeader className="pb-2">
          <CardTitle className="text-lg font-medium">System Metrics</CardTitle>
        </CardHeader>
        <CardContent>
          <div className="h-[250px] flex items-center justify-center">
            <p className="text-gray-800 dark:text-white">Loading chart...</p>
          </div>
        </CardContent>
      </Card>
    )
  }

  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <div className="flex justify-between items-center">
          <CardTitle className="text-lg font-medium">System Metrics</CardTitle>
          <div className="flex space-x-2">
            <button className="bg-gray-300 dark:bg-gray-700 hover:bg-gray-400 dark:hover:bg-gray-600 text-gray-800 dark:text-white text-xs px-2 py-1 rounded">
              sync1
            </button>
            <button className="bg-gray-300 dark:bg-gray-700 hover:bg-gray-400 dark:hover:bg-gray-600 text-gray-800 dark:text-white text-xs px-2 py-1 rounded">
              sync2
            </button>
            <button className="bg-gray-300 dark:bg-gray-700 hover:bg-gray-400 dark:hover:bg-gray-600 text-gray-800 dark:text-white text-xs px-2 py-1 rounded">
              sync3
            </button>
          </div>
        </div>
      </CardHeader>
      <CardContent>
        {data.length > 0 && <Plot data={data} layout={layout} config={config} style={{ width: "100%" }} />}
      </CardContent>
    </Card>
  )
}
