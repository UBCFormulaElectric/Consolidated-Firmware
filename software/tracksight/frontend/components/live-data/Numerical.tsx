'use client'

import React, { useState, useEffect, useMemo } from "react";
import { useSignals } from '@/lib/contexts/SignalContext';
import {
  AreaChart,
  Area,
  XAxis,
  YAxis,
  Tooltip,
} from "recharts";

const NumericalGraph: React.FC = () => {
  const { activeSignals, numericalData, isNumericalSignal } = useSignals();
  const updateInterval = 200; // Update every 200 milliseconds

  const colors = ["#ff4d4f", "#ffa940", "#36cfc9", "#597ef7", "#73d13d"];

  // Only numerical signals from active signals
  const numericalSignals = activeSignals.filter(isNumericalSignal);

  const [scaleFactor, setScaleFactor] = useState("100");
  const [chartHeight, setChartHeight] = useState("256");

  interface ChartDataPoint {
    time: number | string;
    [key: string]: number | string | undefined;
  }

  const processedData = useMemo(() => {
    if (!numericalData?.length) return [];

    // Pre-filter valid objects
    const validDataPoints = numericalData.filter(point => point && typeof point === 'object');

    const transformed = validDataPoints.map((dataPoint): ChartDataPoint | null => {
      let timeValue: number;
      const rawTime = (dataPoint as any).time ?? (dataPoint as any).timestamp;
      timeValue = rawTime != null
        ? (typeof rawTime === 'string' ? Date.parse(rawTime) : Number(rawTime))
        : Date.now();
      if (isNaN(timeValue)) timeValue = Date.now();

      const processed: ChartDataPoint = { time: timeValue };
      let hasValue = false;

      for (const signal of numericalSignals) {
        let val: number | undefined;
        const dp = dataPoint as any;

        if (dp[signal] != null) {
          const candidate = dp[signal];
          if (typeof candidate === 'string') {
            const parsed = parseFloat(candidate);
            if (!isNaN(parsed)) val = parsed;
          } else if (typeof candidate === 'number' && isFinite(candidate)) {
            val = candidate;
          }
        } else if (dp.name === signal && dp.value != null) {
          const raw = dp.value;
          if (typeof raw === 'string') {
            const parsed = parseFloat(raw);
            if (!isNaN(parsed)) val = parsed;
          } else if (typeof raw === 'number' && isFinite(raw)) {
            val = raw;
          }
        }

        if (val != null) {
          processed[signal] = val;
          hasValue = true;
        }
      }

      return hasValue ? processed : null;
    });

    return transformed
      .filter((pt): pt is ChartDataPoint => pt !== null)
      .filter(pt => Object.keys(pt).length > 1);
  }, [numericalData, numericalSignals]);

  const pixelPerDataPoint = 50;
  const chartWidth = Math.max((processedData.length || 0) * pixelPerDataPoint, 500);
  const safeScale = parseInt(scaleFactor) || 100;
  const finalChartWidth = chartWidth * (safeScale / 100);

  if (!numericalSignals.length) {
    return (
      <div className="w-full p-4 h-64 flex flex-col items-center justify-center text-gray-500 border border-gray-300 rounded">
        <p className="mb-4">
          No numerical signals are active. Subscribe to signals using the Live Telemetry Data controls above.
        </p>
        {activeSignals.length > 0 && (
          <p className="text-sm text-yellow-500">
            Note: You have {activeSignals.length} active signal(s) but they appear to be enum/state signals.
            Try subscribing to numerical signals like speeds, voltages, or temperatures.
          </p>
        )}
      </div>
    );
  }

  if (!numericalData?.length) {
    return (
      <div className="w-full h-64 flex items-center justify-center text-gray-500 border border-gray-300 rounded">
        <p>Waiting for data from active signals: {numericalSignals.join(", ")}...</p>
      </div>
    );
  }

  return (
    <div className="w-full h-64">
      <div className="ml-24 mt-4 text-xs flex gap-4">
        {numericalSignals.map((sig, i) => (
          <div key={sig + i} className="flex items-center mb-1">
            <div
              className="w-3 h-3 mr-2"
              style={{ backgroundColor: colors[i % colors.length] }}
            />
            <span className="text-gray-500">{sig}</span>
          </div>
        ))}
      </div>

      <div className="ml-24 mb-2 flex items-center gap-2 text-xs">
        <label htmlFor="scaleSlider">Horizontal Scale (%):</label>
        <input
          id="scaleSlider"
          type="range"
          min="1"
          max="1000"
          value={scaleFactor}
          onChange={e => setScaleFactor(e.target.value)}
        />
        <span>{scaleFactor}%</span>
      </div>

      <div className="ml-24 mb-2 flex items-center gap-2 text-xs">
        <label htmlFor="heightSlider">Vertical Scale (px):</label>
        <input
          id="heightSlider"
          type="range"
          min="100"
          max="1000"
          value={chartHeight}
          onChange={e => setChartHeight(e.target.value)}
        />
        <span>{chartHeight}px</span>
      </div>

      <div
        style={{
          width: `${finalChartWidth}px`,
          height: `${chartHeight}px`,
          transition: "width 0.3s ease-out",
          overflow: "auto",
        }}
      >
        {processedData.length === 0 ? (
          <div className="w-full h-full flex items-center justify-center text-gray-500">
            <p>No valid data points available for the selected signals. Waiting for numerical data...</p>
          </div>
        ) : (
          <AreaChart
            width={Math.max(finalChartWidth, 1)}
            height={Math.max(parseInt(chartHeight), 1)}
            data={processedData}
          >
            <defs>
              {numericalSignals.map((sig, i) => (
                <linearGradient key={sig + i} id={`color${sig}`} x1="0" y1="0" x2="0" y2="1">
                  <stop offset="5%" stopColor={colors[i % colors.length]} stopOpacity={0.8} />
                  <stop offset="95%" stopColor={colors[i % colors.length]} stopOpacity={0} />
                </linearGradient>
              ))}
            </defs>
            <XAxis dataKey="time" type="number" domain={["dataMin", "dataMax"]} hide />
            <YAxis />
            <Tooltip
              labelFormatter={val =>
                typeof val === 'number' ? new Date(val).toLocaleTimeString() : val.toString()
              }
              formatter={(val, name) =>
                typeof val === 'number' && !isNaN(val) ? [`${val}`, `${name}`] : ['N/A', name]
              }
              isAnimationActive={false}
            />
            {numericalSignals.map((sig, i) =>
              processedData.some(pt => pt[sig] != null) ? (
                <Area
                  key={sig + i}
                  type="monotone"
                  dataKey={sig}
                  stroke={colors[i % colors.length]}
                  fillOpacity={1}
                  fill={`url(#color${sig})`}
                  isAnimationActive={false}
                  animationDuration={updateInterval}
                  connectNulls
                />
              ) : null
            )}
          </AreaChart>
        )}
      </div>
    </div>
  );
};

export default NumericalGraph;