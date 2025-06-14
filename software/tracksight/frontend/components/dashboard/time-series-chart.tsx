"use client"

import { useEffect, useState, useMemo } from "react"
import React from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { useTheme } from "next-themes"
import { useSignals } from "@/lib/contexts/SignalContext"
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer
} from "recharts"
import { format } from "date-fns"

interface TimeSeriesChartProps {
  height?: number
  maxDataPoints?: number
}

const TimeSeriesChart = React.memo<TimeSeriesChartProps>(({ height = 250, maxDataPoints = 100 }) => {
  const { resolvedTheme } = useTheme()
  const [mounted, setMounted] = useState(false)
  const { data, activeSignals } = useSignals()

  // Only render after mounting to avoid hydration mismatch
  useEffect(() => {
    setMounted(true)
  }, [])

  // Debug
  useEffect(() => {
    console.log("TimeSeriesChart - activeSignals:", activeSignals);
    console.log("TimeSeriesChart - data sample:", data?.slice(-3));
  }, [activeSignals, data]);

  // Prepare data for the chart
  const chartData = useMemo(() => {
    if (!data || data.length === 0) {
      return [];
    }
    
    // Use only the most recent data points up to maxDataPoints
    return data.slice(-maxDataPoints).map(point => {
      // Convert time to a readable format
      const timeFormatted = format(new Date(Number(point.time)), "HH:mm:ss");
      
      // Add time and all signal values to each data point
      const formattedPoint: any = { timeRaw: Number(point.time), time: timeFormatted };
      
      // Add all active signal values
      activeSignals.forEach(signal => {
        formattedPoint[signal] = point[signal] !== undefined ? point[signal] : null;
      });
      
      return formattedPoint;
    });
  }, [data, activeSignals, maxDataPoints]);

  // Chart colors
  const colors = ["#4ade80", "#60a5fa", "#f472b6", "#facc15", "#f97316", "#a855f7", "#ec4899", "#14b8a6"];

  if (!mounted) return null;

  const isDark = resolvedTheme === "dark";

  // Render a loading or empty state if needed
  if (!activeSignals || activeSignals.length === 0) {
    return (
      <Card className="w-full">
        <CardHeader className="pb-2">
          <CardTitle className="text-lg">Live Data</CardTitle>
        </CardHeader>
        <CardContent className="h-64 flex items-center justify-center">
          <p className="text-gray-500">
            No signals are active. Please subscribe to signals from the Live Data page.
          </p>
        </CardContent>
      </Card>
    );
  }

  // Render the line chart instead of Plotly
  return (
    <Card className="w-full">
      <CardHeader className="pb-2">
        <CardTitle className="text-lg">Live Data</CardTitle>
      </CardHeader>
      <CardContent>
        <div style={{ height: `${height}px`, width: "100%" }}>
          {chartData.length > 0 ? (
            <ResponsiveContainer width="100%" height="100%">
              <LineChart data={chartData} margin={{ top: 5, right: 20, left: 10, bottom: 5 }}>
                <CartesianGrid strokeDasharray="3 3" stroke={isDark ? "#374151" : "#e5e7eb"} />
                <XAxis 
                  dataKey="time" 
                  stroke={isDark ? "#e5e7eb" : "#111827"} 
                  tick={{ fill: isDark ? "#e5e7eb" : "#111827" }} 
                />
                <YAxis stroke={isDark ? "#e5e7eb" : "#111827"} tick={{ fill: isDark ? "#e5e7eb" : "#111827" }} />
                <Tooltip 
                  contentStyle={{ 
                    backgroundColor: isDark ? "#1f2937" : "#ffffff",
                    color: isDark ? "#e5e7eb" : "#111827",
                    border: isDark ? "1px solid #374151" : "1px solid #e5e7eb"
                  }} 
                />
                <Legend />
                {activeSignals.map((signal, index) => (
                  <Line
                    key={signal}
                    type="monotone"
                    dataKey={signal}
                    stroke={colors[index % colors.length]}
                    activeDot={{ r: 4 }}
                    isAnimationActive={false}
                    dot={false}
                    name={signal}
                  />
                ))}
              </LineChart>
            </ResponsiveContainer>
          ) : (
            <div className="h-full flex items-center justify-center">
              <p className="text-gray-500">Waiting for data...</p>
            </div>
          )}
        </div>
      </CardContent>
    </Card>
  )
});

// Add display name for better debugging
TimeSeriesChart.displayName = 'TimeSeriesChart';

export default TimeSeriesChart;
