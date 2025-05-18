"use client"

import { useEffect, useState } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import dynamic from "next/dynamic"
import { useTheme } from "next-themes"

// Dynamically import Plotly to avoid SSR issues
const Plot = dynamic(() => import("react-plotly.js"), { ssr: false })

interface LiveDataChartProps {
  title: string
  dataKey: string
  height?: number
}

export default function LiveDataChart({ title, dataKey, height = 250 }: LiveDataChartProps) {
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

      // Create data based on the dataKey
      let values: number[] = []

      switch (dataKey) {
        case "motorTemp":
          values = times.map(() => 60 + Math.random() * 10)
          break
        case "batteryVoltage":
          values = times.map(() => 375 + Math.random() * 10)
          break
        case "motorRpm":
          values = times.map(() => 3000 + Math.random() * 500)
          break
        case "vehicleSpeed":
          values = times.map(() => 40 + Math.random() * 10)
          break
        default:
          values = times.map(() => Math.random() * 100)
      }

      return [
        {
          x: times,
          y: values,
          type: "scatter",
          mode: "lines",
          name: title,
          line: { color: "#4ade80" },
        },
      ]
    }

    setData(generateData())

    // Update data every 1 second
    const interval = setInterval(() => {
      setData(generateData())
    }, 1000)

    return () => clearInterval(interval)
  }, [dataKey, title])

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
    showlegend: false,
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
          <CardTitle className="text-lg font-medium">{title}</CardTitle>
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
        <CardTitle className="text-lg font-medium">{title}</CardTitle>
      </CardHeader>
      <CardContent>
        {data.length > 0 && <Plot data={data} layout={layout} config={config} style={{ width: "100%" }} />}
      </CardContent>
    </Card>
  )
}
