"use client";

import { usePausePlay } from "@/components/shared/PausePlayControl";
import { PlusButton } from "@/components/shared/PlusButton";
import { useSyncedGraphScroll } from "@/components/shared/SyncedGraphContainer";
import { SignalType } from "@/hooks/SignalConfig";
import { useSignals, useDataVersion } from "@/hooks/SignalContext";
import { WidgetDataNumerical } from "@/lib/types/Widget";
import React, {
  useCallback,
  useEffect,
  useMemo,
  useRef,
  useState,
} from "react";
import CanvasChart from "@/components/shared/CanvasChart";

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
type AlignedData = [number[], ...Array<(number | string | null)[]>];

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

interface NumericalWidgetProps {
  widgetData: WidgetDataNumerical;
  appendNumSignal: (widgetId: string, newSignal: string) => void;
  removeNumSignal: (widgetId: string, signalToRemove: string) => void;
  onDelete?: () => void; 
}

const NumericalWidget: React.FC<NumericalWidgetProps> = React.memo(
  ({ widgetData, appendNumSignal, removeNumSignal, onDelete }) => {
    const { isPaused } = usePausePlay();
    
    const signalsCtx = useSignals() as any;
    const dataVersion = useDataVersion();
    const {
      activeSignals,
      subscribeToSignal,
      unsubscribeFromSignal,
      getAllData,
    } = signalsCtx;

    // TODO: hook up availableSignals metadata (currently only fetched in DropdownSearch on demand)
    // In a real implementation this should come from a metadata hook
    const availableSignals: any[] = useMemo(() => [], []);
    
    const allData: any[] = useMemo(
      () => getAllData(),
      [getAllData, dataVersion]
    );

    const [chartHeight, setChartHeight] = useState(256);
    const [searchTerm, setSearchTerm] = useState("");
    const [isSearchOpen, setIsSearchOpen] = useState(false);
    const [dropdownPosition, setDropdownPosition] = useState({
      top: 0,
      left: 0,
    });
    const buttonRef = useRef<HTMLDivElement>(null);

    const {
      progress: scrollProgress,
      setProgress: setScrollProgress,
      zoomLevel,
      setZoomLevel,
      hoverTimestamp,
      setHoverTimestamp,
      registerTimeRange,
      unregisterTimeRange,
      globalTimeRange,
    } = useSyncedGraphScroll();

    const graphId = widgetData.id;

    const isLoadingSignals = false;

    const uplotBuildCount = useRef(0);
    const lastBuildMetaRef = useRef<{ points: number; signals: number }>({
      points: 0,
      signals: 0,
    });

    useEffect(() => {
      widgetData.signals.forEach(sig => {
        subscribeToSignal(sig, SignalType.Numerical);
      });
      
      return () => {
        widgetData.signals.forEach(sig => {
           unsubscribeFromSignal(sig);
        });
      };
    }, [widgetData.signals, subscribeToSignal, unsubscribeFromSignal]);

    const thisGraphSignals = useMemo(() => {
      return widgetData.signals.filter(
        (sig) => isPaused || activeSignals.includes(sig)
      );
    }, [activeSignals, widgetData.signals, isPaused]);

    const uplotData = useMemo(() => {
      const buildStart =
        typeof performance !== "undefined" ? performance.now() : 0;

      if (thisGraphSignals.length === 0) {
        if (process.env.NODE_ENV !== "production") {
          uplotBuildCount.current += 1;
          lastBuildMetaRef.current = { points: 0, signals: 0 };
        }
        return [[], ...thisGraphSignals.map(() => [])] as AlignedData;
      }

      const timestamps: number[] = [];
      const seriesBySignal = new Map<string, (number | string | null)[]>();
      thisGraphSignals.forEach((sig) => {
        seriesBySignal.set(sig, []);
      });

      const indexByTimestamp = new Map<number, number>();

      for (const entry of allData) {
        const sig = entry.name as string;
        if (!seriesBySignal.has(sig)) continue;

        const timestamp =
          typeof entry.time === "number"
            ? entry.time
            : new Date(entry.time).getTime();

        let rawValue: number | string | null = entry.value;

        if (rawValue !== null && rawValue !== undefined) {
          if (typeof rawValue !== "number" && typeof rawValue !== "string") {
            rawValue = Number(rawValue);
          }
          // If it's a number but NaN, skip it?
          if (typeof rawValue === "number" && !Number.isFinite(rawValue))
            continue;
        }

        if (!Number.isFinite(timestamp)) continue;

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

      const aligned = [
        timestamps,
        ...thisGraphSignals.map((sig) => seriesBySignal.get(sig)!),
      ] as AlignedData;

      if (process.env.NODE_ENV !== "production") {
        uplotBuildCount.current += 1;
        const buildDuration =
          typeof performance !== "undefined"
            ? performance.now() - buildStart
            : 0;
        lastBuildMetaRef.current = {
          points: timestamps.length,
          signals: thisGraphSignals.length,
        };
        if (uplotBuildCount.current % 25 === 1) {
          console.debug(
            `[perf] numerical uplotData rebuilt x${uplotBuildCount.current} (${timestamps.length
            } pts, ${thisGraphSignals.length
            } signals) in ${buildDuration.toFixed(2)}ms`
          );
        }
      }

      return aligned;
    }, [allData, thisGraphSignals]);

    const numericalSignals = thisGraphSignals;

    const availableOptions = React.useMemo(
      () => availableSignals.filter((s) => !widgetData.signals.includes(s.name)),
      [availableSignals, widgetData.signals]
    );

    const filteredSignals = React.useMemo(() => {
      if (!searchTerm) return availableOptions;
      return availableOptions.filter((s) =>
        s.name.toLowerCase().includes(searchTerm.toLowerCase())
      );
    }, [availableOptions, searchTerm]);

    const totalDataPoints = (uplotData[0] as number[]).length;

    useEffect(() => {
      const timestamps = uplotData[0] as number[];
      if (timestamps && timestamps.length > 0) {
        const min = timestamps[0];
        const max = timestamps[timestamps.length - 1];
        registerTimeRange(graphId, min, max);
      } else {
        unregisterTimeRange(graphId);
      }
      return () => unregisterTimeRange(graphId);
    }, [uplotData, graphId, registerTimeRange, unregisterTimeRange]);

    const chartWidth =
      typeof window !== "undefined" ? window.innerWidth - 100 : 1200;

    const handleSelect = useCallback(
      (name: string) => {
        appendNumSignal(widgetData.id, name);
        setSearchTerm("");
        setIsSearchOpen(false);
      },
      [appendNumSignal, widgetData.id]
    );

    const handleUnsub = useCallback(
      (name: string) => {
        removeNumSignal(widgetData.id, name);
      },
      [removeNumSignal, widgetData.id]
    );

    const handleSnapToLatest = useCallback(() => {
      setScrollProgress(1);
    }, [setScrollProgress]);

    const lastBuildMeta = lastBuildMetaRef.current;
    const buildCountSnapshot = uplotBuildCount.current;

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
            {onDelete && (
                <button
                onClick={onDelete}
                className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
                title="Remove this graph"
                >
                ×
                </button>
            )}
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
            <div className="flex flex-col gap-1">
              <label className="text-sm">Zoom: {zoomLevel}%</label>
              <button
                onClick={() => setZoomLevel(100)}
                className="w-32 px-3 py-1 bg-blue-500 text-white opacity-80 rounded-lg transition duration-150 ease-in-out hover:cursor-pointer hover:scale-105 hover:bg-blue-600 hover:opacity-100"
              >
                Reset Zoom
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
                      placeholder="Search or type signal name..."
                      value={searchTerm}
                      onChange={(e) => setSearchTerm(e.target.value)}
                      onKeyDown={(e) => {
                          if (e.key === 'Enter' && searchTerm) {
                              handleSelect(searchTerm);
                          }
                      }}
                      autoFocus
                      className="w-full mb-2 px-2 py-1 border rounded"
                    />
                    <div className="max-h-32 overflow-auto">
                      {isLoadingSignals ? (
                        <div className="px-2 py-1 text-gray-500">
                          Loading...
                        </div>
                      ) : filteredSignals.length === 0 ? (
                        <div 
                            className="px-2 py-1 hover:bg-gray-100 cursor-pointer text-blue-600"
                            onClick={() => searchTerm && handleSelect(searchTerm)}
                        >
                          Add "{searchTerm}"
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
          </div>

          {/* Debug info */}
          <div className="text-xs text-gray-500 mb-4 space-y-1 bg-gray-50 p-2 rounded border">
            <div>Total points rendered: {totalDataPoints}</div>
            <div>Zoom: {zoomLevel}%</div>
            <div>Scroll Progress: {(scrollProgress * 100).toFixed(1)}%</div>
            <div>Chart Width: {chartWidth}px</div>
            {process.env.NODE_ENV !== "production" && (
              <>
                <div>uplot rebuilds (dev only): {buildCountSnapshot} times</div>
                <div>
                  Last rebuild size: {lastBuildMeta.points} pts /{" "}
                  {lastBuildMeta.signals} signals
                </div>
              </>
            )}
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
              data={
                uplotData as [number[], ...Array<(number | string | null)[]>]
              }
              width={chartWidth}
              height={chartHeight}
              series={numericalSignals.map((sig, i) => ({
                label: sig,
                color: signalColors[i % signalColors.length],
              }))}
              zoomLevel={zoomLevel}
              onZoomChange={(z) => setZoomLevel(Math.max(z, 100))}
              scrollProgress={scrollProgress}
              hoverTimestamp={hoverTimestamp}
              onHoverTimestampChange={setHoverTimestamp}
              domainStart={globalTimeRange?.min}
              domainEnd={globalTimeRange?.max}
            />
          </div>
        )}
      </div>
    );
  }
);

NumericalWidget.displayName = "NumericalWidget";

export default NumericalWidget;

