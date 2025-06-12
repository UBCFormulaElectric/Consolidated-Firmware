// enumeration.tsx
"use client";

import { usePausePlay } from "@/components/shared/pause-play-control";
import { SignalType, useSignals } from "@/lib/contexts/SignalContext";
import React, { useCallback, useEffect, useMemo, useRef } from "react";

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

// Colors for enumeration states
const stateColors = [
  "#FF3B2F",
  "#FFCC02",
  "#FF9500",
  "#35C759",
  "#007AFF",
  "#5856D6",
  "#AF52DE",
  "#FF2D55",
];

const EnumerationGraphComponent: React.FC<DynamicSignalGraphProps> = ({
  signalName,
  onDelete,
}) => {
  const { isPaused, horizontalScale, setHorizontalScale } = usePausePlay();
  const {
    enumData,
    subscribeToSignal,
    unsubscribeFromSignal,
    getEnumValues,
    mapEnumValue,
  } = useSignals();

  // Track if this component subscribed to the signal for proper cleanup
  const hasSubscribed = useRef<boolean>(false);

  useEffect(() => {
    if (signalName && !hasSubscribed.current) {
      subscribeToSignal(signalName, SignalType.Enumeration);
      hasSubscribed.current = true;
    }

    return () => {
      if (hasSubscribed.current) {
        unsubscribeFromSignal(signalName);
        hasSubscribed.current = false;
      }
    };
  }, [signalName]); // Removed function dependencies to prevent infinite loops

  const chartData = useMemo(() => {
    const windowMs = 100; // Same as numerical component
    const filteredData = enumData.filter(
      (d) => d.name === signalName && d.value != null
    );

    if (filteredData.length === 0) return [];

    // Create time windows from all data points (similar to numerical)
    const timeWindows = new Set<number>();
    filteredData.forEach((d) => {
      const t =
        typeof d.time === "number" ? d.time : new Date(d.time).getTime();
      const r = Math.floor(t / windowMs) * windowMs;
      timeWindows.add(r);
    });

    const sortedTimes = Array.from(timeWindows).sort((a, b) => a - b);

    // Track last known state to fill gaps (similar to numerical's forward-fill)
    let lastKnownState: string | undefined;

    // Group data by time for efficient lookup
    const dataByTime: Record<number, string> = {};
    filteredData.forEach((d) => {
      const t =
        typeof d.time === "number" ? d.time : new Date(d.time).getTime();
      const r = Math.floor(t / windowMs) * windowMs;
      dataByTime[r] = String(d.value);
    });

    // Build chart data with forward-filled states
    return sortedTimes
      .map((time) => {
        if (dataByTime[time] !== undefined) {
          // We have data for this time point
          lastKnownState = dataByTime[time];
          return { time, state: lastKnownState };
        } else if (lastKnownState !== undefined) {
          // No data for this time point, use last known state
          return { time, state: lastKnownState };
        }
        // If no last known state exists, don't include this time point
        return null;
      })
      .filter(Boolean) as { time: number; state: string }[];
  }, [enumData, signalName]);

  const pixelPerPoint = 50; // Same as numerical component
  const width =
    Math.max(chartData.length * pixelPerPoint, 100) * (horizontalScale / 100);

  const bars = useMemo(() => {
    return chartData.map((dataPoint, index) => ({
      state: dataPoint.state,
      startTime: dataPoint.time,
      width: pixelPerPoint * (horizontalScale / 100),
      xOffset: index * pixelPerPoint * (horizontalScale / 100),
    }));
  }, [chartData, horizontalScale]);
  const enumVals = getEnumValues(signalName);
  const lastCode = chartData.length
    ? chartData[chartData.length - 1].state
    : undefined;
  const lastLabel =
    lastCode != null
      ? mapEnumValue(signalName, lastCode) ?? lastCode
      : undefined;

  // pick up to 5 labels evenly spaced to declutter the x-axis
  const labelBars = useMemo(() => {
    const maxLabels = 5;
    const step = Math.ceil(bars.length / maxLabels) || 1;
    return bars.filter((_, idx) => idx % step === 0);
  }, [bars]);

  const handleUnsubscribe = useCallback(() => {
    if (hasSubscribed.current) {
      unsubscribeFromSignal(signalName);
      hasSubscribed.current = false;
    }
    onDelete();
  }, [signalName, onDelete]); // Removed unsubscribeFromSignal dependency

  return (
    <div className="mb-6 p-4 inline-block">
      {isPaused && (
        <div className="top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10 block">
          PAUSED
        </div>
      )}
      <div className="sticky left-0 block w-[50vw] animate-none overscroll-contain z-40">
        <div className="flex items-center gap-2 mb-4">
          <h3 className="font-semibold">Enumeration: {signalName}</h3>
          <button
            onClick={handleUnsubscribe}
            className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
            title="Remove this graph and unsubscribe from signal"
          >
            ×
          </button>
        </div>

        <div className="flex flex-wrap gap-4 mb-4">
          <div className="flex flex-col">
            <label className="text-sm">
              Horizontal Scale: {horizontalScale}%
            </label>
            <input
              type="range"
              min={1}
              max={1000}
              value={horizontalScale}
              onChange={(e) => setHorizontalScale(+e.target.value)}
            />
          </div>
        </div>

        <div className="flex flex-wrap gap-4 text-xs mb-4">
          {enumVals.map((label, i) => {
            const color = stateColors[i % stateColors.length];
            const active = label === lastLabel;
            return (
              <div key={label} className="inline-flex items-center gap-1">
                <span
                  className="w-3 h-3 block rounded"
                  style={{ backgroundColor: color }}
                />
                <span
                  className={active ? "font-bold text-black" : "text-gray-500"}
                >
                  {label}
                </span>
              </div>
            );
          })}
        </div>
      </div>

      {chartData.length === 0 ? (
        <div className="w-full h-[24px] flex items-center justify-center text-gray-500">
          No data
        </div>
      ) : (
        <div
          className="relative inline-block"
          style={{
            width,
            transition: "width 100ms ease-out",
            marginLeft: "60px", // Match the left margin that AreaChart uses for Y-axis
          }}
        >
          <div className="h-6 flex flex-row flex-nowrap">
            {bars.map((bar, idx) => {
              const label = mapEnumValue(signalName, bar.state) ?? bar.state;
              const iColor =
                stateColors[
                  Math.max(enumVals.indexOf(label), 0) % stateColors.length
                ];
              return (
                <div
                  key={idx}
                  className="h-6 shrink-0"
                  style={{ width: `${bar.width}px`, backgroundColor: iColor }}
                  title={`${label} @ ${new Date(
                    bar.startTime
                  ).toLocaleTimeString()}`}
                />
              );
            })}
          </div>
          <div className="absolute top-full left-0 h-4">
            {labelBars.map((bar, idx) => (
              <div
                key={idx}
                className="absolute whitespace-nowrap text-xs"
                style={{ left: `${bar.xOffset}px` }}
              >
                {new Date(bar.startTime).toLocaleTimeString()}
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
};

export default EnumerationGraphComponent;
