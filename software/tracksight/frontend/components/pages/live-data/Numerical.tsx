// numerical.tsx
"use client";

import { usePausePlay } from "@/components/shared/PausePlayControl";
import { PlusButton } from "@/components/shared/PlusButton";
import { SignalType, useSignals } from "@/lib/contexts/SignalContext";
import { formatWithMs } from "@/lib/dateformat";
import React, { useCallback, useEffect, useRef, useState } from "react";
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

const NumericalGraphComponent: React.FC<DynamicSignalGraphProps> = React.memo(
  ({ signalName, onDelete }) => {
    const { isPaused, horizontalScale, setHorizontalScale } = usePausePlay();
    const {
      availableSignals,
      activeSignals,
      numericalData,
      enumData,
      subscribeToSignal,
      unsubscribeFromSignal,
      isLoadingSignals,
      isNumericalSignal,
    } = useSignals();

    const [chartHeight, setChartHeight] = useState(256);
    const [searchTerm, setSearchTerm] = useState("");
    const [isSearchOpen, setIsSearchOpen] = useState(false);
    const [dropdownPosition, setDropdownPosition] = useState({
      top: 0,
      left: 0,
    });
    const buttonRef = useRef<HTMLDivElement>(null);
    const chartContainerRef = useRef<HTMLDivElement>(null);
    const [horizontalScrollPosition, setHorizontalScrollPosition] = useState(0);

    // Track signals this component instance subscribed to for proper cleanup
    const componentSubscriptions = useRef<Set<string>>(new Set());
    // Force re-render when subscriptions change
    const [subscriptionVersion, setSubscriptionVersion] = useState(0);

    // Get only the signals that this specific graph instance has subscribed to
    const thisGraphSignals = React.useMemo(() => {
      // When paused, show all signals this component has subscribed to regardless of global activeSignals
      // When not paused, filter by both component subscriptions and global activeSignals
      const result = Array.from(componentSubscriptions.current).filter(
        (sig) =>
          (isPaused || activeSignals.includes(sig)) && isNumericalSignal(sig)
      );
      return result;
    }, [activeSignals, isNumericalSignal, subscriptionVersion, isPaused]);

    const chartData = React.useMemo(() => {
      const filteredData = numericalData.filter(
        (d) =>
          thisGraphSignals.includes(d.name as string) &&
          isNumericalSignal(d.name as string)
      );

      if (filteredData.length === 0) return [];

      // Get all unique timestamps from active signals for the complete timeline
      const allTimestamps = new Set<number>();

      // Get data from all active signals to create shared time grid
      const allActiveData = [...numericalData, ...enumData].filter((d) =>
        activeSignals.includes(d.name as string)
      );

      allActiveData.forEach((d) => {
        const t =
          typeof d.time === "number" ? d.time : new Date(d.time).getTime();
        allTimestamps.add(t);
      });

      const sortedTimes = Array.from(allTimestamps).sort((a, b) => a - b);

      if (sortedTimes.length === 0) return [];

      // Calculate viewport-based windowing parameters
      const viewportWidth =
        typeof window !== "undefined" ? window.innerWidth : 1200;
      const pixelPerPoint = 50;
      const scaledPixelPerPoint = pixelPerPoint * (horizontalScale / 100);
      const pointsPerViewport = Math.ceil(viewportWidth / scaledPixelPerPoint);

      // Buffer: 2x viewport on each side
      const bufferSize = pointsPerViewport * 2;

      // Calculate scroll-based time window
      const totalWidth = sortedTimes.length * scaledPixelPerPoint;
      const maxScrollLeft = Math.max(0, totalWidth - viewportWidth);
      const clampedScrollPosition = Math.min(
        horizontalScrollPosition,
        maxScrollLeft
      );

      // Convert scroll position to data point index
      const scrollProgress =
        maxScrollLeft > 0 ? clampedScrollPosition / maxScrollLeft : 0;
      const maxStartIndex = Math.max(0, sortedTimes.length - pointsPerViewport);
      const viewportStartIndex = Math.floor(scrollProgress * maxStartIndex);

      // Apply buffer around the viewport
      const bufferStartIndex = Math.max(0, viewportStartIndex - bufferSize);
      const bufferEndIndex = Math.min(
        sortedTimes.length,
        viewportStartIndex + pointsPerViewport + bufferSize
      );

      const windowedTimes = sortedTimes.slice(bufferStartIndex, bufferEndIndex);

      // Track last known value for each signal to fill gaps
      const lastKnownValues: Record<string, number> = {};

      // Initialize last known values from data before the window (for forward-filling)
      const preWindowData = sortedTimes.slice(0, bufferStartIndex);
      preWindowData.forEach((time) => {
        thisGraphSignals.forEach((signalName) => {
          if (!isNumericalSignal(signalName)) return;

          // Find the most recent value for this signal before the window
          const signalData = filteredData.find(
            (d) =>
              d.name === signalName &&
              (typeof d.time === "number"
                ? d.time
                : new Date(d.time).getTime()) === time
          );

          if (signalData && signalData.value !== undefined) {
            lastKnownValues[signalName] = Number(signalData.value);
          }
        });
      });

      // Group data by signal and time for efficient lookup
      const dataBySignal: Record<string, Record<number, number>> = {};
      filteredData.forEach((d) => {
        const signalName = d.name as string;
        const t =
          typeof d.time === "number" ? d.time : new Date(d.time).getTime();

        if (!dataBySignal[signalName]) {
          dataBySignal[signalName] = {};
        }
        dataBySignal[signalName][t] =
          d.value !== undefined ? Number(d.value) : 0;
      });

      // Build chart data with forward-filled values for the windowed time range
      return windowedTimes.map((time) => {
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
    }, [
      numericalData,
      enumData,
      activeSignals,
      thisGraphSignals,
      isNumericalSignal,
      horizontalScale,
      horizontalScrollPosition,
    ]);

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

    // Calculate total width based on ALL data points to maintain scroll illusion
    const totalDataPoints = React.useMemo(() => {
      const allTimestamps = new Set<number>();
      const allActiveData = [...numericalData, ...enumData].filter((d) =>
        activeSignals.includes(d.name as string)
      );
      allActiveData.forEach((d) => {
        const t =
          typeof d.time === "number" ? d.time : new Date(d.time).getTime();
        allTimestamps.add(t);
      });
      return allTimestamps.size;
    }, [numericalData, enumData, activeSignals]);

    // Total width represents the entire timeline for scrolling
    const totalWidth =
      Math.max(totalDataPoints * pixelPerPoint, 100) * (horizontalScale / 100);

    // Chart width matches the rendered data
    const chartWidth =
      Math.max(chartData.length * pixelPerPoint, 100) * (horizontalScale / 100);

    // Calculate chart position offset to maintain scroll illusion
    const chartOffset = React.useMemo(() => {
      if (totalDataPoints === 0) return 0;

      const viewportWidth =
        typeof window !== "undefined" ? window.innerWidth : 1200;
      const scaledPixelPerPoint = pixelPerPoint * (horizontalScale / 100);
      const pointsPerViewport = Math.ceil(viewportWidth / scaledPixelPerPoint);
      const bufferSize = pointsPerViewport * 2;

      const totalTimelineWidth = totalDataPoints * scaledPixelPerPoint;
      const maxScrollLeft = Math.max(0, totalTimelineWidth - viewportWidth);
      const clampedScrollPosition = Math.min(
        horizontalScrollPosition,
        maxScrollLeft
      );

      const scrollProgress =
        maxScrollLeft > 0 ? clampedScrollPosition / maxScrollLeft : 0;
      const maxStartIndex = Math.max(0, totalDataPoints - pointsPerViewport);
      const viewportStartIndex = Math.floor(scrollProgress * maxStartIndex);
      const bufferStartIndex = Math.max(0, viewportStartIndex - bufferSize);

      return bufferStartIndex * scaledPixelPerPoint;
    }, [
      totalDataPoints,
      horizontalScale,
      horizontalScrollPosition,
      pixelPerPoint,
    ]);

    const handleSelect = useCallback(
      (name: string) => {
        subscribeToSignal(name, SignalType.Numerical);
        componentSubscriptions.current.add(name);
        setSubscriptionVersion((v) => v + 1);
        setSearchTerm("");
        setIsSearchOpen(false);
      },
      [subscribeToSignal]
    );

    const handleUnsub = useCallback(
      (name: string) => {
        unsubscribeFromSignal(name);
        componentSubscriptions.current.delete(name);
        setSubscriptionVersion((v) => v + 1);
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
        setSubscriptionVersion((v) => v + 1);
      }

      return () => {
        // Only cleanup signals that this specific component instance subscribed to
        componentSubscriptions.current.forEach((signal) => {
          unsubscribeFromSignal(signal);
        });
        componentSubscriptions.current.clear();
      };
    }, [signalName]); // Only depend on signalName, not the functions

    // Track scroll position for viewport windowing
    useEffect(() => {
      const handleScroll = (event: Event) => {
        const target = event.target as HTMLElement;
        setHorizontalScrollPosition(target.scrollLeft);
      };

      // Try multiple selectors to find the correct scroll container
      const scrollContainers = [
        document.querySelector(".overflow-x-scroll"),
        chartContainerRef.current?.closest(".overflow-x-scroll"),
        document.querySelector('[class*="overflow-x-scroll"]'),
      ].filter(Boolean);

      let activeContainer: Element | null = null;

      // Find the container that actually has horizontal scroll capability
      for (const container of scrollContainers) {
        if (container && container.scrollWidth > container.clientWidth) {
          activeContainer = container;
          break;
        }
      }

      if (activeContainer) {
        activeContainer.addEventListener("scroll", handleScroll, {
          passive: true,
        });
        // Set initial scroll position
        setHorizontalScrollPosition(activeContainer.scrollLeft);
        return () =>
          activeContainer?.removeEventListener("scroll", handleScroll);
      }
    }, [chartData.length]); // Re-run when chart data changes

    // Debug info for windowing calculations
    const debugInfo = React.useMemo(() => {
      if (totalDataPoints === 0) return null;

      const viewportWidth =
        typeof window !== "undefined" ? window.innerWidth : 1200;
      const scaledPixelPerPoint = 50 * (horizontalScale / 100);
      const pointsPerViewport = Math.ceil(viewportWidth / scaledPixelPerPoint);
      const totalWidth = totalDataPoints * scaledPixelPerPoint;
      const maxScrollLeft = Math.max(0, totalWidth - viewportWidth);
      const scrollProgress =
        maxScrollLeft > 0
          ? ((horizontalScrollPosition / maxScrollLeft) * 100).toFixed(1)
          : "0";

      return {
        pointsPerViewport,
        scrollProgress: `${scrollProgress}%`,
        totalWidth: Math.round(totalWidth),
        viewportWidth: Math.round(viewportWidth),
      };
    }, [totalDataPoints, horizontalScale, horizontalScrollPosition]);

    return (
      <div className="mb-6 p-4 block w-full relative">
        {isPaused && (
          <div className="top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10 block">
            PAUSED
          </div>
        )}
        <div className=" sticky left-0 block w-[50vw] animate-none overscroll-contain z-40">
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
                        left: rect.left + window.scrollX,
                      });
                    }
                    setIsSearchOpen((o) => !o);
                  }}
                  variant="rowSide"
                />
                {isSearchOpen && (
                  <div
                    className="fixed bg-white border rounded shadow w-64 p-2 z-[9999]"
                    style={{
                      top: dropdownPosition.top,
                      left: dropdownPosition.left,
                    }}
                  >
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
                        <div className="px-2 py-1 text-gray-500">
                          Loading...
                        </div>
                      ) : filteredSignals.length === 0 ? (
                        <div className="px-2 py-1 text-gray-500">
                          No signals
                        </div>
                      ) : (
                        filteredSignals.map((s) => (
                          <div
                            key={s.name}
                            onClick={() => handleSelect(s.name)}
                            className="px-2 py-1 hover:bg-gray-100 cursor-pointer"
                          >
                            <div className="font-medium">{s.name}</div>
                            <div className="text-xs text-gray-500">
                              {s.unit}
                            </div>
                          </div>
                        ))
                      )}
                    </div>
                  </div>
                )}
              </div>
            )}
          </div>

          {/* Debug info for viewport windowing */}
          <div className="text-xs text-gray-500 mb-4 space-y-1 bg-gray-50 p-2 rounded border">
            <div>
              Total points: {totalDataPoints} | Rendered: {chartData.length}
            </div>
            <div>
              Horizontal Scroll: {horizontalScrollPosition}px | Chart Offset:{" "}
              {Math.round(chartOffset)}px
            </div>
            {debugInfo && (
              <div>
                Viewport: {debugInfo.pointsPerViewport} pts | Scroll:{" "}
                {debugInfo.scrollProgress} | Width: {debugInfo.viewportWidth}px
                / {debugInfo.totalWidth}px
              </div>
            )}
          </div>
        </div>

        {chartData.length === 0 ? (
          <div className="w-full h-[256px] flex items-center justify-center text-gray-500">
            No data
          </div>
        ) : (
          <>
            <div
              ref={chartContainerRef}
              style={{
                width: totalWidth,
                height: chartHeight,
                transition: "width 100ms ease-out",
                position: "relative",
              }}
            >
              <div
                style={{
                  position: "absolute",
                  left: chartOffset,
                  top: 0,
                }}
              >
                <AreaChart
                  width={chartWidth}
                  height={chartHeight}
                  data={chartData}
                >
                  <XAxis
                    dataKey="time"
                    tickFormatter={(t) => formatWithMs(new Date(t))}
                    interval={1}
                  />
                  <YAxis domain={[0, "auto"]} />
                  <Tooltip
                    isAnimationActive={false}
                    labelFormatter={(time) => formatWithMs(new Date(time))}
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
                  ))}{" "}
                </AreaChart>
              </div>
            </div>
          </>
        )}
      </div>
    );
  }
);

// Add display name for better debugging
NumericalGraphComponent.displayName = "NumericalGraphComponent";

export default NumericalGraphComponent;
