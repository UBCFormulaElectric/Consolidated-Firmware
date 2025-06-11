// numerical.tsx
"use client";

import { usePausePlay } from "@/components/shared/pause-play-control";
import { PlusButton } from "@/components/shared/PlusButton";
import { SignalType, useSignals } from "@/lib/contexts/SignalContext";
import React, { useCallback, useEffect, useState, useRef } from "react";
import { Area, AreaChart, Tooltip, XAxis, YAxis } from "recharts";

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

// Palette for numerical signals
const signalColors = [
  "#ff4d4f",
  "#ffa940",
  "#36cfc9",
  "#597ef7",
  "#73d13d",
  "#ff85c0",
  "#9254de",
  "#ffc069",
  "#69c0ff",
  "#ffec3d",
];

const NumericalGraphComponent: React.FC<DynamicSignalGraphProps> = ({
  signalName,
  onDelete,
}) => {
  const { isPaused } = usePausePlay();
  const {
    availableSignals,
    activeSignals,
    numericalData,
    subscribeToSignal,
    unsubscribeFromSignal,
    isLoadingSignals,
    isNumericalSignal,
  } = useSignals();

  const [chartHeight, setChartHeight] = useState(256);
  const [scaleFactor, setScaleFactor] = useState(100);
  const [searchTerm, setSearchTerm] = useState("");
  const [isSearchOpen, setIsSearchOpen] = useState(false);
  const [dropdownPosition, setDropdownPosition] = useState({ top: 0, left: 0 });
  const buttonRef = useRef<HTMLDivElement>(null);
  
  // Track signals this component instance subscribed to for proper cleanup
  const componentSubscriptions = useRef<Set<string>>(new Set());
  // Force re-render when subscriptions change
  const [subscriptionVersion, setSubscriptionVersion] = useState(0);

  // Get only the signals that this specific graph instance has subscribed to
  const thisGraphSignals = React.useMemo(() => {
    // When paused, show all signals this component has subscribed to regardless of global activeSignals
    // When not paused, filter by both component subscriptions and global activeSignals
    const result = Array.from(componentSubscriptions.current).filter((sig) => 
      (isPaused || activeSignals.includes(sig)) && isNumericalSignal(sig)
    );
    return result;
  }, [activeSignals, isNumericalSignal, subscriptionVersion, isPaused]);

  const chartData = React.useMemo(() => {
    const windowMs = 100;
    const filteredData = numericalData.filter(
      (d) =>
        thisGraphSignals.includes(d.name as string) &&
        isNumericalSignal(d.name as string)
    );

    if (filteredData.length === 0) return [];

    // Create time windows from all data points
    const timeWindows = new Set<number>();
    filteredData.forEach((d) => {
      const t = typeof d.time === "number" ? d.time : new Date(d.time).getTime();
      const r = Math.floor(t / windowMs) * windowMs;
      timeWindows.add(r);
    });

    const sortedTimes = Array.from(timeWindows).sort((a, b) => a - b);
    
    // Track last known value for each signal to fill gaps
    const lastKnownValues: Record<string, number> = {};
    
    // Group data by signal and time for efficient lookup
    const dataBySignal: Record<string, Record<number, number>> = {};
    filteredData.forEach((d) => {
      const signalName = d.name as string;
      const t = typeof d.time === "number" ? d.time : new Date(d.time).getTime();
      const r = Math.floor(t / windowMs) * windowMs;
      
      if (!dataBySignal[signalName]) {
        dataBySignal[signalName] = {};
      }
      dataBySignal[signalName][r] = d.value !== undefined ? Number(d.value) : 0;
    });

    // Build chart data with forward-filled values
    return sortedTimes.map((time) => {
      const dataPoint: any = { time };
      
      // For each signal this graph has subscribed to, get the value or carry forward the last known value
      thisGraphSignals.forEach((signalName) => {
        if (!isNumericalSignal(signalName)) return;
        
        const signalData = dataBySignal[signalName];
        if (signalData && signalData[time] !== undefined) {
          // We have data for this time point
          lastKnownValues[signalName] = signalData[time];
          dataPoint[signalName] = signalData[time];
        } else if (lastKnownValues[signalName] !== undefined) {
          // No data for this time point, use last known value
          dataPoint[signalName] = lastKnownValues[signalName];
        }
        // If no last known value exists, don't add the property (signal hasn't started yet)
      });
      
      return dataPoint;
    });
  }, [numericalData, thisGraphSignals, isNumericalSignal]);

  const numericalSignals = thisGraphSignals;

  const availableOptions = React.useMemo(
    () =>
      availableSignals.filter(
        (s) => !thisGraphSignals.includes(s.name) && isNumericalSignal(s.name)
      ),
    [availableSignals, thisGraphSignals, isNumericalSignal]
  );

  const filteredSignals = React.useMemo(() => {
    if (!searchTerm) return availableOptions;
    return availableOptions.filter((s) =>
      s.name.toLowerCase().includes(searchTerm.toLowerCase())
    );
  }, [availableOptions, searchTerm]);

  const pixelPerPoint = 50;
  const width =
    Math.max(chartData.length * pixelPerPoint, 100) * (scaleFactor / 100);

  const handleSelect = useCallback(
    (name: string) => {
      subscribeToSignal(name, SignalType.Numerical);
      componentSubscriptions.current.add(name);
      setSubscriptionVersion(v => v + 1);
      setSearchTerm("");
      setIsSearchOpen(false);
    },
    [subscribeToSignal]
  );

  const handleUnsub = useCallback(
    (name: string) => {
      unsubscribeFromSignal(name);
      componentSubscriptions.current.delete(name);
      setSubscriptionVersion(v => v + 1);
    },
    [unsubscribeFromSignal]
  );

  const handleDelete = useCallback(() => {
    numericalSignals.forEach((n) => {
      unsubscribeFromSignal(n);
      componentSubscriptions.current.delete(n);
    });
    onDelete();
  }, [numericalSignals, unsubscribeFromSignal, onDelete]);

  // Cleanup effect to prevent memory leaks and race conditions on unmount
  useEffect(() => {
    // Subscribe to the initial signal if provided
    if (signalName && !componentSubscriptions.current.has(signalName)) {
      subscribeToSignal(signalName, SignalType.Numerical);
      componentSubscriptions.current.add(signalName);
      setSubscriptionVersion(v => v + 1);
    }
    
    return () => {
      // Only cleanup signals that this specific component instance subscribed to
      componentSubscriptions.current.forEach((signal) => {
        unsubscribeFromSignal(signal);
      });
      componentSubscriptions.current.clear();
    };
  }, [signalName]); // Only depend on signalName, not the functions

  return (
    <div className="mb-6 p-4 inline-block w-min-[100vm] relative">
      {isPaused && (
        <div className="top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10 block">
          PAUSED
        </div>
      )}
      <div className=" sticky left-0 block w-[50vw] animate-none overscroll-contain relative z-40">
        <div className="flex items-center gap-2 mb-4">
          <h3 className="font-semibold">Numerical Graph</h3>
          <button
            onClick={handleDelete}
            className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
            title="Remove this graph and unsubscribe all"
          >
            ×
          </button>
        </div>

        <div className="flex flex-wrap gap-4 mb-4">
          <div className="flex flex-col">
            <label className="text-sm">Vertical Scale: {chartHeight}px</label>
            <input
              type="range"
              min={100}
              max={600}
              step={50}
              value={chartHeight}
              onChange={(e) => setChartHeight(+e.target.value)}
            />
          </div>
          <div className="flex flex-col">
            <label className="text-sm">Horizontal Scale: {scaleFactor}%</label>
            <input
              type="range"
              min={1}
              max={1000}
              value={scaleFactor}
              onChange={(e) => setScaleFactor(+e.target.value)}
            />
          </div>
        </div>

        <div className="flex flex-wrap items-center gap-3 mb-4">
          {numericalSignals.map((sig, i) => {
            const meta = availableSignals.find((s) => s.name === sig);
            const color = signalColors[i % signalColors.length];
            return (
              <button
                key={sig}
                onClick={() => handleUnsub(sig)}
                className="flex items-center gap-2 px-3 py-1.5 rounded-full border hover:opacity-80 transition-opacity"
                style={{ backgroundColor: color + "20" }}
                title={`Unsubscribe ${sig}`}
              >
                <div
                  className="w-3 h-3 rounded-full"
                  style={{ backgroundColor: color }}
                />
                <span className="font-medium">{sig}</span>
                {meta?.unit && <span>({meta.unit})</span>}
                <span className="ml-1 text-red-600">×</span>
              </button>
            );
          })}
          {availableOptions.length > 0 && (
            <div ref={buttonRef} className="relative z-50">
              <PlusButton
                onClick={() => {
                  if (buttonRef.current) {
                    const rect = buttonRef.current.getBoundingClientRect();
                    setDropdownPosition({
                      top: rect.bottom + window.scrollY + 4,
                      left: rect.left + window.scrollX
                    });
                  }
                  setIsSearchOpen((o) => !o);
                }}
                variant="rowSide"
              />
              {isSearchOpen && (
                <div className="fixed bg-white border rounded shadow w-64 p-2 z-[9999]" 
                     style={{
                       top: dropdownPosition.top,
                       left: dropdownPosition.left
                     }}>
                  <input
                    type="text"
                    placeholder="Search numerical signals..."
                    value={searchTerm}
                    onChange={(e) => setSearchTerm(e.target.value)}
                    autoFocus
                    className="w-full mb-2 px-2 py-1 border rounded"
                  />
                  <div className="max-h-32 overflow-auto">
                    {isLoadingSignals ? (
                      <div className="px-2 py-1 text-gray-500">Loading...</div>
                    ) : filteredSignals.length === 0 ? (
                      <div className="px-2 py-1 text-gray-500">No signals</div>
                    ) : (
                      filteredSignals.map((s) => (
                        <div
                          key={s.name}
                          onClick={() => handleSelect(s.name)}
                          className="px-2 py-1 hover:bg-gray-100 cursor-pointer"
                        >
                          <div className="font-medium">{s.name}</div>
                          <div className="text-xs text-gray-500">{s.unit}</div>
                        </div>
                      ))
                    )}
                  </div>
                </div>
              )}
            </div>
          )}
        </div>
      </div>

      {chartData.length === 0 ? (
        <div className="w-full h-[256px] flex items-center justify-center text-gray-500">
          No data
        </div>
      ) : (
        <div
          style={{
            width,
            height: chartHeight,
            transition: "width 100ms ease-out",
          }}
        >
          <AreaChart width={width} height={chartHeight} data={chartData}>
            <XAxis
              dataKey="time"
              tickFormatter={(t) => new Date(t).toLocaleTimeString()}
              interval={10}
            />
            <YAxis domain={[0, "auto"]} />
            <Tooltip
              isAnimationActive={false}
              labelFormatter={(time) => new Date(time).toLocaleString()}
            />
            {numericalSignals.map((sig, i) => (
              <Area
                key={sig}
                type="monotone"
                dataKey={sig}
                stroke={signalColors[i % signalColors.length]}
                fill={signalColors[i % signalColors.length]}
                strokeWidth={1}
                isAnimationActive={false}
              />
            ))}
          </AreaChart>
        </div>
      )}
    </div>
  );
};

export default NumericalGraphComponent;
