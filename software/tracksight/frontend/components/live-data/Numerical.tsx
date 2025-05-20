'use client'

import React, { useState, useEffect, useCallback } from "react";
import { useSignals } from '@/lib/contexts/SignalContext';
import {
  AreaChart,
  Area,
  XAxis,
  YAxis,
  Tooltip,
} from "recharts";

const NumericalGraph: React.FC = () => {
  const { currentTime, activeSignals, data, unsubscribeFromSignal } = useSignals();
  const updateInterval = 200; // Update every 200 milliseconds

  const colors = ["#ff4d4f", "#ffa940", "#36cfc9", "#597ef7", "#73d13d"];

  // Add debugging for data and track active signals
  useEffect(() => {
    console.log("NumericalGraph - activeSignals:", activeSignals);
    console.log("NumericalGraph - data sample:", data.slice(-3));
  }, [activeSignals, data]);

  // Help function to check if a signal is enum type (for filtering)
  const isEnumSignal = useCallback((signalName: string) => {
    // Enum signals typically have "state" or "mode" in their name
    // This is a simple heuristic - you might want to improve it based on your actual signal naming conventions
    return signalName.toLowerCase().includes('state') || 
           signalName.toLowerCase().includes('mode') ||
           signalName.toLowerCase().includes('enum');
  }, []);
  
  // Filter to only show numerical signals (non-enum signals)
  const numericalSignals = activeSignals.filter(signal => !isEnumSignal(signal));

  const [scaleFactor, setScaleFactor] = useState("100");
  const [chartHeight, setChartHeight] = useState("256");

  // Default to a reasonable width even with no data
  const pixelPerDataPoint = 50;
  const chartWidth = Math.max((data?.length || 0) * pixelPerDataPoint, 500);
  const finalChartWidth = chartWidth * (parseInt(scaleFactor) / 100);

  // Render a message if no signals are active
  if (!numericalSignals || numericalSignals.length === 0) {
    return (
      <div className="w-full h-64 flex items-center justify-center text-gray-500">
        No numerical signals are active. Add signals using the controls below.
      </div>
    );
  }

  // Render a message if there's no data yet
  if (!data || data.length === 0) {
    return (
      <div className="w-full h-64 flex items-center justify-center text-gray-500">
        Waiting for data from active signals...
      </div>
    );
  }

  return (
    <div className="w-full h-64">
      <div className="ml-24 mt-4 text-xs flex gap-4">
        {numericalSignals.map((signalName, index) => (
          <div key={`${signalName}-${index}`} className="flex items-center mb-1">
            <div
              className="w-3 h-3 inline-block mr-2"
              style={{
                backgroundColor: colors[index % colors.length],
              }}
            ></div>
            <span className="text-gray-500">{signalName}</span>
          </div>
        ))}
      </div>
      <div className="ml-24 mb-2">
        <label htmlFor="scaleSlider" className="mr-2">
          Horizontal Scale (%):
        </label>
        <input
          id="scaleSlider"
          type="range"
          min="1"
          max="1000"
          value={scaleFactor}
          onChange={(e) => setScaleFactor(e.target.value)}
        />
        <span className="ml-2">{scaleFactor}%</span>
      </div>
      <div className="ml-24 mb-2">
        <label htmlFor="heightSlider" className="mr-2">
          Vertical Scale (px):
        </label>
        <input
          id="heightSlider"
          type="range"
          min="100"
          max="1000"
          value={chartHeight}
          onChange={(e) => setChartHeight(e.target.value)}
        />
        <span className="ml-2">{chartHeight}px</span>
      </div>
      <div
        style={{
          width: `${finalChartWidth}px`,
          height: `${chartHeight}px`,
          transition: "width 0.3s ease-out", // CSS transition to smooth width changes
          overflow: "auto"
        }}
      >
        <AreaChart
          width={finalChartWidth}
          height={parseInt(chartHeight)}
          data={data}
        >
          <defs>
            {numericalSignals.map((signalName, index) => (
              <linearGradient
                key={`${signalName}-${index}`}
                id={`color${signalName}`}
                x1="0"
                y1="0"
                x2="0"
                y2="1"
              >
                <stop
                  offset="5%"
                  stopColor={colors[index % colors.length]}
                  stopOpacity={0.8}
                />
                <stop
                  offset="95%"
                  stopColor={colors[index % colors.length]}
                  stopOpacity={0}
                />
              </linearGradient>
            ))}
          </defs>
          <XAxis
            dataKey="time"
            type="number"
            domain={["dataMin", "dataMax"]}
            hide
          />
          <YAxis />
          <Tooltip
            labelFormatter={(value) => 
              typeof value === 'number' ? new Date(value).toLocaleTimeString() : value.toString()
            }
            formatter={(value, name) => [`${value}`, `${name}`]}
          />
          {numericalSignals.map((signalName, index) => (
            <Area
              key={`${signalName}-${index}`}
              type="monotone"
              dataKey={signalName}
              stroke={colors[index % colors.length]}
              fillOpacity={1}
              fill={`url(#color${signalName})`}
              isAnimationActive={false}
              animationDuration={updateInterval}
            />
          ))}
        </AreaChart>
      </div>
    </div>
  );
};

export default NumericalGraph;
