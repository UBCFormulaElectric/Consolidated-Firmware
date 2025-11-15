// numerical.tsx
"use client";

import { usePausePlay } from "@/components/shared/PausePlayControl";
import { PlusButton } from "@/components/shared/PlusButton";
import { useSyncedGraphScroll } from "@/components/shared/SyncedGraphContainer";
import { SignalType } from "@/hooks/SignalConfig";
import { useSignals, useDataVersion } from "@/hooks/SignalContext";
import React, {
  useCallback,
  useEffect,
  useMemo,
  useRef,
  useState,
} from "react";
import CanvasChart from "@/components/shared/CanvasChart";

interface DynamicSignalGraphProps {
  signalName: string;
  onDelete: () => void;
}

type AlignedData = [number[], ...Array<(number | null)[]>];

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
    const { isPaused } = usePausePlay();
    // Single context access (removed enum mixing & duplicate calls)
    const signalsCtx = useSignals() as any;
    const dataVersion = useDataVersion();
    const {
      activeSignals,
      subscribeToSignal,
      unsubscribeFromSignal,
      getNumericalData,
    } = signalsCtx;

    // TODO: hook up availableSignals metadata (currently only fetched in DropdownSearch on demand)
    const availableSignals: any[] = useMemo(() => [], []);
    const numericalData: any[] = useMemo(
      () => getNumericalData(),
      [getNumericalData, dataVersion]
    );

    const [chartHeight, setChartHeight] = useState(256);
    const [searchTerm, setSearchTerm] = useState("");
    const [isSearchOpen, setIsSearchOpen] = useState(false);
    const [dropdownPosition, setDropdownPosition] = useState({
      top: 0,
      left: 0,
    });
    const buttonRef = useRef<HTMLDivElement>(null);

    const [zoomLevel, setZoomLevel] = useState(100);
    const [timeTickCount, setTimeTickCount] = useState(6);
    const isLoadingSignals = false;

    const { progress: scrollProgress, setProgress: setScrollProgress } =
      useSyncedGraphScroll();

    const componentSubscriptions = useRef<Set<string>>(new Set());
    const [subscriptionVersion, setSubscriptionVersion] = useState(0);

    const thisGraphSignals = useMemo(() => {
      return Array.from(componentSubscriptions.current).filter(
        (sig) => isPaused || activeSignals.includes(sig)
      );
    }, [activeSignals, subscriptionVersion, isPaused]);

    const uplotData = useMemo(() => {
      if (thisGraphSignals.length === 0) {
        return [[], ...thisGraphSignals.map(() => [])] as AlignedData;
      }

      const timestamps: number[] = [];
      const seriesBySignal = new Map<string, (number | null)[]>();
      thisGraphSignals.forEach((sig) => {
        seriesBySignal.set(sig, []);
      });

      const indexByTimestamp = new Map<number, number>();

      for (const entry of numericalData) {
        const sig = entry.name as string;
        if (!seriesBySignal.has(sig)) continue;

        const timestamp =
          typeof entry.time === "number"
            ? entry.time
            : new Date(entry.time).getTime();
        const rawValue =
          typeof entry.value === "number" ? entry.value : Number(entry.value);

        if (!Number.isFinite(timestamp) || !Number.isFinite(rawValue)) continue;

        let columnIndex = indexByTimestamp.get(timestamp);
        if (columnIndex === undefined) {
          columnIndex = timestamps.length;
          indexByTimestamp.set(timestamp, columnIndex);
          timestamps.push(timestamp);
          seriesBySignal.forEach((arr) => arr.push(null));
        }

        const seriesArr = seriesBySignal.get(sig)!;
        seriesArr[columnIndex] = rawValue;
      }

      if (timestamps.length > 1) {
        let isMonotonic = true;
        for (let i = 1; i < timestamps.length; i++) {
          if (timestamps[i] < timestamps[i - 1]) {
            isMonotonic = false;
            break;
          }
        }

        if (!isMonotonic) {
          const indices = timestamps.map((_, idx) => idx);
          indices.sort((a, b) => timestamps[a] - timestamps[b]);

          const sortedTimestamps = indices.map((idx) => timestamps[idx]);
          const sortedSeries = thisGraphSignals.map((sig) => {
            const arr = seriesBySignal.get(sig)!;
            return indices.map((idx) => arr[idx]);
          });
          return [sortedTimestamps, ...sortedSeries] as AlignedData;
        }
      }

      return [
        timestamps,
        ...thisGraphSignals.map((sig) => seriesBySignal.get(sig)!),
      ] as AlignedData;
    }, [numericalData, thisGraphSignals]);

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

    const totalDataPoints = (uplotData[0] as number[]).length;

    // Fixed width - use container width or full viewport width
    const chartWidth =
      typeof window !== "undefined" ? window.innerWidth - 100 : 1200;

    const handleSelect = useCallback(
      (name: string) => {
        if (process.env.NODE_ENV !== "production") {
          console.log(
            `[ui] Numerical add ${name} -> subscribe & attach to this graph`
          );
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
          console.log(
            `[ui] Numerical remove ${name} -> unsubscribe (chip click)`
          );
        }
        unsubscribeFromSignal(name);
        componentSubscriptions.current.delete(name);
        setSubscriptionVersion((v) => v + 1);
      },
      [unsubscribeFromSignal]
    );

    const handleDelete = useCallback(() => {
      if (process.env.NODE_ENV !== "production") {
        console.log(
          `[ui] Numerical delete graph -> unsubscribe all owned signals`,
          Array.from(componentSubscriptions.current)
        );
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
          console.log(
            `[ui] Numerical mount with initial ${signalName} -> subscribe`
          );
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

    // Mouse event handlers for panning
    const handleSnapToLatest = useCallback(() => {
      setScrollProgress(1);
    }, [setScrollProgress]);

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
              <label className="text-sm">Zoom: {zoomLevel}%</label>
              <input
                type="range"
                min={10}
                max={10000}
                step={10}
                value={zoomLevel}
                onChange={(e) => setZoomLevel(+e.target.value)}
              />
            </div>
            <div className="flex flex-col">
              <label className="text-sm">Time Ticks: {timeTickCount}</label>
              <input
                type="range"
                min={2}
                max={12}
                step={1}
                value={timeTickCount}
                onChange={(e) => setTimeTickCount(+e.target.value)}
              />
            </div>
            <div className="flex flex-col gap-1">
              <label className="text-sm">
                Scroll Position: {(scrollProgress * 100).toFixed(1)}%
              </label>
              <button
                onClick={handleSnapToLatest}
                className="px-3 py-1 bg-blue-500 text-white opacity-80 rounded-lg transition duration-150 ease-in-out hover:cursor-pointer hover:scale-105 hover:bg-blue-600 hover:opacity-100"
              >
                Jump to Latest
              </button>
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

          {/* Debug info */}
          <div className="text-xs text-gray-500 mb-4 space-y-1 bg-gray-50 p-2 rounded border">
            <div>Total points rendered: {totalDataPoints}</div>
            <div>Zoom: {zoomLevel}%</div>
            <div>Scroll Progress: {(scrollProgress * 100).toFixed(1)}%</div>
            <div>Chart Width: {chartWidth}px</div>
          </div>
        </div>

        {totalDataPoints === 0 ? (
          <div className="w-full h-[256px] flex items-center justify-center text-gray-500">
            No data
          </div>
        ) : (
          <div
            style={{
              width: chartWidth,
              height: chartHeight,
              position: "relative",
              overflow: "hidden",
            }}
          >
            <CanvasChart
              data={uplotData as [number[], ...Array<(number | null)[]>]}
              width={chartWidth}
              height={chartHeight}
              series={numericalSignals.map((sig, i) => ({
                label: sig,
                color: signalColors[i % signalColors.length],
              }))}
              zoomLevel={zoomLevel}
              timeTickCount={timeTickCount}
              scrollProgress={scrollProgress}
            />
          </div>
        )}
      </div>
    );
  }
);

NumericalGraphComponent.displayName = "NumericalGraphComponent";

export default NumericalGraphComponent;
