// numerical.tsx
"use client";

import { usePausePlay } from "@/components/shared/PausePlayControl";
import { PlusButton } from "@/components/shared/PlusButton";
import { SignalType } from "@/hooks/SignalConfig";
import { useSignals, useDataVersion } from "@/hooks/SignalContext";
import { formatWithMs } from "@/lib/dateformat";
import React, { useCallback, useEffect, useMemo, useRef, useState } from "react";
import UPlotChart from "@/components/shared/UPlotChart";
import createDataPointFormatter from '@/lib/charts/createDataPointFormatter';

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

const options = {
  // Make uPlot lighter by disabling interactive features
  legend: { show: false },
  cursor: {
    show: false,
    x: false,
    y: false,
    drag: { x: false, y: false, setScale: false },
    // Disable proximity calculations for focus/hover
    focus: { prox: -1 },
    hover: { prox: null },
  },
  // Optional: on HiDPI (retina) this reduces pixel work. Enable if you want extra speed.
  // pxRatio: 1,
}

const NumericalGraphComponent: React.FC<DynamicSignalGraphProps> = React.memo(
  ({ signalName, onDelete }) => {
    const { isPaused, horizontalScale, setHorizontalScale } = usePausePlay();
    // Single context access (removed enum mixing & duplicate calls)
    const signalsCtx = useSignals() as any;
    const dataVersion = useDataVersion();
    const { activeSignals, subscribeToSignal, unsubscribeFromSignal, getNumericalData } = signalsCtx;

    // TODO: hook up availableSignals metadata (currently only fetched in DropdownSearch on demand)
    const availableSignals: any[] = useMemo(() => [], []);
    // Only numerical data now
    const numericalData: any[] = useMemo(() => getNumericalData(), [getNumericalData, dataVersion]);
    const isLoadingSignals = false;

    const [chartHeight, setChartHeight] = useState(256);
    const [searchTerm, setSearchTerm] = useState("");
    const [isSearchOpen, setIsSearchOpen] = useState(false);
    const [dropdownPosition, setDropdownPosition] = useState({
      top: 0,
      left: 0,
    });
    const buttonRef = useRef<HTMLDivElement>(null);
    const chartContainerRef = useRef<HTMLDivElement>(null);
    // Removed horizontal scroll virtualization state

    // Track signals this component instance subscribed to for proper cleanup
    const componentSubscriptions = useRef<Set<string>>(new Set());
    // Force re-render when subscriptions change
    const [subscriptionVersion, setSubscriptionVersion] = useState(0);

    // Get only the signals that this specific graph instance has subscribed to
    const thisGraphSignals = React.useMemo(() => {
      return Array.from(componentSubscriptions.current).filter(
        (sig) => isPaused || activeSignals.includes(sig)
      );
    }, [activeSignals, subscriptionVersion, isPaused]);

    const dataPointsFormatter = useMemo(() => {
      return createDataPointFormatter(componentSubscriptions.current);
    }, [componentSubscriptions.current]);

    const uPlotData = useMemo(() => {
      return dataPointsFormatter(numericalData);
    }, [dataPointsFormatter, dataVersion]);

    const numericalSignals = thisGraphSignals;

    const availableOptions = React.useMemo(
      () => availableSignals.filter((s) => !thisGraphSignals.includes(s.name)),
      [availableSignals, thisGraphSignals]
    );

    const filteredSignals = React.useMemo(() => {
      if (!searchTerm) return availableOptions;
      return availableOptions.filter((s) =>
        s.name.toLowerCase().includes(searchTerm.toLowerCase())
      );
    }, [availableOptions, searchTerm]);

    const pixelPerPoint = 50;

    // Simplified: total points = rendered chart points
    const totalDataPoints = (uPlotData[0] as number[]).length;

    // Width simply scales with number of points shown
    const totalWidth = 1028;
    const chartWidth = totalWidth;

    // No offset needed without virtualization
    const chartOffset = 0;

    const handleSelect = useCallback(
      (name: string) => {
        if (process.env.NODE_ENV !== "production") {
          console.log(`[ui] Numerical add ${name} -> subscribe & attach to this graph`);
        }
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
        if (process.env.NODE_ENV !== "production") {
          console.log(`[ui] Numerical remove ${name} -> unsubscribe (chip click)`);
        }
        unsubscribeFromSignal(name);
        componentSubscriptions.current.delete(name);
        setSubscriptionVersion((v) => v + 1);
      },
      [unsubscribeFromSignal]
    );

    const handleDelete = useCallback(() => {
      if (process.env.NODE_ENV !== "production") {
        console.log(`[ui] Numerical delete graph -> unsubscribe all owned signals`, Array.from(componentSubscriptions.current));
      }
      // Unsubscribe all signals that this component had subscribed to
      Array.from(componentSubscriptions.current).forEach((n) => {
        unsubscribeFromSignal(n);
        componentSubscriptions.current.delete(n);
      });
      onDelete();
    }, [unsubscribeFromSignal, onDelete]);

    // Cleanup effect to prevent memory leaks and race conditions on unmount
    useEffect(() => {
      // Subscribe to the initial signal if provided
      if (signalName && !componentSubscriptions.current.has(signalName)) {
        if (process.env.NODE_ENV !== "production") {
          console.log(`[ui] Numerical mount with initial ${signalName} -> subscribe`);
        }
        subscribeToSignal(signalName, SignalType.Numerical);
        componentSubscriptions.current.add(signalName);
        setSubscriptionVersion((v) => v + 1);
      }

      return () => {
        // Only cleanup signals that this specific component instance subscribed to
        componentSubscriptions.current.forEach((signal) => {
          if (process.env.NODE_ENV !== "production") {
            console.log(`[ui] Numerical unmount -> unsubscribe ${signal}`);
          }
          unsubscribeFromSignal(signal);
        });
        componentSubscriptions.current.clear();
      };
    }, [signalName]); // Only depend on signalName, not the functions

    const numericalSeries = React.useMemo(() =>
      numericalSignals.map((sig, i) => ({ label: sig, color: signalColors[i % signalColors.length] })),
      [numericalSignals]
    )

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
            <div>Total points: {totalDataPoints}</div>
            <div>Chart Offset: {Math.round(chartOffset)}px</div>
          </div>
        </div>

        {totalDataPoints === 0 ? (
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
                <UPlotChart
                  data={uPlotData as [number[], ...(Array<(number | null)[]>)]}
                  version={dataVersion}
                  width={chartWidth}
                  height={chartHeight}
                  series={numericalSeries}
                  options={options}
                />
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
