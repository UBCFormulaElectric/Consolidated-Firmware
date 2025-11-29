"use client";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useEffect, useState, useMemo, memo } from "react";
import CanvasChart, { AlignedData } from "@/components/widgets/CanvasChart";
import { usePausePlay, PausePlayButton } from "@/components/PausePlayControl";
import { SignalType } from "@/lib/SignalConfig";
import { PlusButton } from "@/components/PlusButton";
import { signalColors } from "./signalColors";

export interface MockGraphConfig {
  type: SignalType;
  delay: number; // ms
  initialPoints: number;
  signalName: string;
}

function generateRandomValue(type: SignalType, time: number, index: number = 0) {
  if (type === SignalType.Enumeration) {
    const states = ["IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING"];
    return states[Math.floor(Math.random() * states.length)];
  } else {
    return (
      Math.sin(time / 1000 + index) * 50 +
      Math.random() * 10 +
      50 +
      index * 20
    );
  }
}

const MockGraph = memo(({ onDelete, config }: {
  onDelete: () => void;
  config: MockGraphConfig;
}) => {
  const [configs, setConfigs] = useState<MockGraphConfig[]>([config]);
  const [showAddModal, setShowAddModal] = useState(false);
  const [newSignalName, setNewSignalName] = useState("");
  const [newSignalType, setNewSignalType] = useState<SignalType>(SignalType.Numerical);
  const [newSignalDelay, setNewSignalDelay] = useState(100);
  // Here it is legal to use local data, but for real data use some other data source
  const [data, setData] = useState<{
    timestamps: number[];
    series: Record<string, (number | string | null)[]>;
  }>({ timestamps: [], series: {} });
  const [chartHeight, setChartHeight] = useState(256);

  const { isPaused } = usePausePlay();
  const { scalePxPerSecRef, hoverTimestamp, timeRangeRef } = useSyncedGraph();

  // REGISTERING TIME RANGE???
  // const graphId = useMemo(() => Math.random().toString(36).slice(2), []);
  // useEffect(() => {
  //   if (data.timestamps.length > 0) {
  //     const min = data.timestamps[0];
  //     const max = data.timestamps[data.timestamps.length - 1];
  //     registerTimeRange(graphId, min, max);
  //   }
  //   return () => unregisterTimeRange(graphId);
  // }, [data.timestamps, graphId, registerTimeRange, unregisterTimeRange]);

  // Initialize first config data
  useEffect(() => {
    const now = Date.now();
    const timestamps: number[] = [];
    const values: (number | string | null)[] = [];

    for (let i = 0; i < config.initialPoints; i++) {
      const time =
        now - (config.initialPoints - i) * (100000 / config.initialPoints);
      timestamps.push(time);
      values.push(generateRandomValue(config.type, time, 0));
    }

    setData({
      timestamps,
      series: { [config.signalName]: values },
    });
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [config]);

  useEffect(() => {
    if (isPaused) return;

    const intervals: NodeJS.Timeout[] = [];

    configs.forEach((cfg, idx) => {
      const interval = setInterval(() => {
        const now = Date.now();
        const val = generateRandomValue(cfg.type, now, idx);

        setData((prev) => {
          const newTimestamps = [...prev.timestamps, now];
          const newSeries = { ...prev.series };

          // Ensure all series keys exist
          configs.forEach((c) => {
            if (!newSeries[c.signalName]) {
              newSeries[c.signalName] = new Array(
                prev.timestamps.length
              ).fill(null);
            }
          });

          // Extend all series to match new length
          Object.keys(newSeries).forEach((key) => {
            // If this is the signal updating, push value
            if (key === cfg.signalName) {
              newSeries[key] = [...newSeries[key], val];
            } else {
              // Hold previous value (Zero-Order Hold) to avoid gaps in the line
              const lastVal =
                newSeries[key].length > 0
                  ? newSeries[key][newSeries[key].length - 1]
                  : null;
              newSeries[key] = [...newSeries[key], lastVal];
            }
          });

          return { timestamps: newTimestamps, series: newSeries };
        });
      }, cfg.delay);
      intervals.push(interval);
    });

    return () => intervals.forEach(clearInterval);
  }, [configs, isPaused]);

  const chartData = useMemo<AlignedData>(() => ({
    timestamps: data.timestamps,
    series: configs.map((c) => data.series[c.signalName] || [])
  }), [data, configs]);

  const handleAddSignal = (e: React.FormEvent) => {
    e.preventDefault();
    const name = newSignalName.trim() || `Signal ${configs.length + 1}`;
    if (configs.some((c) => c.signalName === name)) {
      alert("Signal name must be unique in this graph");
      return;
    }

    const newConfig: MockGraphConfig = {
      signalName: name,
      type: newSignalType,
      delay: newSignalDelay,
      initialPoints: 0,
    };

    // Initialize data series for the new signal with nulls so it matches current length
    setData((prev) => {
      const newSeries = { ...prev.series };
      newSeries[name] = new Array(prev.timestamps.length).fill(null);
      return { ...prev, series: newSeries };
    });

    setConfigs((prev) => [...prev, newConfig]);
    setShowAddModal(false);
    setNewSignalName("");
    setNewSignalType(SignalType.Numerical);
  };

  const handleRemoveSignal = (name: string) => {
    if (configs.length <= 1) {
      onDelete();
      return;
    }
    setConfigs((prev) => prev.filter((c) => c.signalName !== name));
    setData((prev) => {
      const nextSeries = { ...prev.series };
      delete nextSeries[name];
      return { ...prev, series: nextSeries };
    });
  };

  // const visiblePointsCount = useMemo(() => {
  //   if (data.timestamps.length === 0) return 0;

  //   const earliestTime = timeRangeRef.current
  //     ? timeRangeRef.current.min
  //     : data.timestamps[0];
  //   const latestTime = timeRangeRef.current
  //     ? timeRangeRef.current.max
  //     : data.timestamps[data.timestamps.length - 1];
  //   const totalTimeRange = latestTime - earliestTime;

  //   if (totalTimeRange <= 0) return data.timestamps.length;

  //   const zoomFactor = 100 / zoomLevel;
  //   const visibleTimeRange = totalTimeRange * zoomFactor;
  //   const normalizedScroll =
  //     typeof scrollProgress === "number" && Number.isFinite(scrollProgress)
  //       ? Math.min(Math.max(scrollProgress, 0), 1)
  //       : 1;

  //   const maxStartOffset = Math.max(totalTimeRange - visibleTimeRange, 0);
  //   const startOffset = maxStartOffset * normalizedScroll;
  //   const visibleStartTime = earliestTime + startOffset;
  //   const visibleEndTime = visibleStartTime + visibleTimeRange;

  //   // Binary search for visible range
  //   const binarySearchForFirstVisibleIndex = (
  //     timestamps: number[],
  //     targetTime: number
  //   ): number => {
  //     let left = 0;
  //     let right = timestamps.length - 1;
  //     let result = 0;
  //     while (left <= right) {
  //       const mid = Math.floor((left + right) / 2);
  //       if (timestamps[mid] >= targetTime) {
  //         result = mid;
  //         right = mid - 1;
  //       } else {
  //         left = mid + 1;
  //       }
  //     }
  //     return result;
  //   };

  //   const startIndex = binarySearchForFirstVisibleIndex(
  //     data.timestamps,
  //     visibleStartTime
  //   );

  //   let endIndex = data.timestamps.length - 1;
  //   let left = 0;
  //   let right = data.timestamps.length - 1;
  //   while (left <= right) {
  //     const mid = Math.floor((left + right) / 2);
  //     if (data.timestamps[mid] <= visibleEndTime) {
  //       endIndex = mid;
  //       left = mid + 1;
  //     } else {
  //       right = mid - 1;
  //     }
  //   }

  //   return Math.max(0, endIndex - startIndex + 1);
  // }, [data.timestamps]);
  // const totalDataPoints = data.timestamps.length;

  return (
    <div className="mb-6 p-4 block w-full relative">
      <div className="sticky left-0 block w-[50vw] animate-none overscroll-contain z-40">
        <div className="flex items-center gap-2 mb-4">
          <h3 className="font-semibold">Mock Graph Container</h3>
          <PausePlayButton />
          <button
            onClick={onDelete}
            className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
            title="Remove mock graph"
          >
            ×
          </button>
        </div>

        {/* <div className="flex flex-wrap gap-4 mb-4">
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
              onClick={() => console.error("Not implemented")}
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
        </div> */}

        <div className="flex flex-wrap items-center gap-3 mb-4">
          {configs.map((cfg, idx) => {
            const color = signalColors[idx % signalColors.length];
            return (
              <div
                key={cfg.signalName}
                className="flex items-center gap-2 px-3 py-1.5 rounded-full border hover:opacity-80 transition-opacity"
                style={{ backgroundColor: color + "20" }}
              >
                <div
                  className="w-3 h-3 rounded-full"
                  style={{ backgroundColor: color }}
                />
                <span className="font-medium">{cfg.signalName}</span>
                <span className="text-xs text-gray-500">({cfg.delay}ms)</span>
                <button
                  onClick={() => handleRemoveSignal(cfg.signalName)}
                  className="ml-1 text-gray-500 hover:text-red-500 font-bold"
                  title="Remove signal"
                >
                  x
                </button>
              </div>
            );
          })}
          <div className="relative">
            <PlusButton
              onClick={() => setShowAddModal(true)}
              variant="rowSide"
            />

            {showAddModal && (
              <div
                className="fixed inset-0 bg-black/50 flex items-center justify-center z-100"
                onClick={() => setShowAddModal(false)}
              >
                <div
                  className="bg-white rounded-lg shadow-xl p-6 w-96 max-w-full m-4"
                  onClick={(e) => e.stopPropagation()}
                >
                  <h3 className="text-lg font-bold mb-4">Add Mock Signal</h3>
                  <form onSubmit={handleAddSignal} className="space-y-4">
                    <div>
                      <label className="block text-sm font-medium text-gray-700 mb-1">
                        Signal Name
                      </label>
                      <input
                        type="text"
                        value={newSignalName}
                        onChange={(e) => setNewSignalName(e.target.value)}
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        placeholder="e.g. Voltage"
                        autoFocus
                      />
                    </div>
                    <div>
                      <label className="block text-sm font-medium text-gray-700 mb-1">
                        Type
                      </label>
                      <select
                        value={newSignalType}
                        onChange={(e) =>
                          setNewSignalType(e.target.value as SignalType)
                        }
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                      >
                        <option value={SignalType.Numerical}>
                          Numerical
                        </option>
                        <option value={SignalType.Enumeration}>
                          Enumeration
                        </option>
                      </select>
                    </div>
                    <div>
                      <label className="block text-sm font-medium text-gray-700 mb-1">
                        Update Delay (ms)
                      </label>
                      <input
                        type="number"
                        value={newSignalDelay}
                        onChange={(e) =>
                          setNewSignalDelay(Number(e.target.value))
                        }
                        min="1"
                        max="5000"
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                      />
                    </div>
                    <div className="flex justify-end gap-2 pt-2">
                      <button
                        type="button"
                        onClick={() => setShowAddModal(false)}
                        className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded"
                      >
                        Cancel
                      </button>
                      <button
                        type="submit"
                        className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded"
                      >
                        Add
                      </button>
                    </div>
                  </form>
                </div>
              </div>
            )}
          </div>
        </div>

        {/* Debug info */}
        {/* <div className="text-xs text-gray-500 mb-4 space-y-1 bg-gray-50 p-2 rounded border">
          <div>Total points: {totalDataPoints}</div>
          <div>Points in view: {visiblePointsCount}</div>
          <div>Scroll Progress: {(scrollProgress * 100).toFixed(1)}%</div>
        </div> */}
      </div>

      <div className="relative overflow-hidden w-full" style={{ height: chartHeight }}>
        <CanvasChart
          data={chartData}
          height={chartHeight}
          series={configs.map((c, idx) => ({
            label: c.signalName,
            color: signalColors[idx % signalColors.length],
          }))}

          // should not be handled period
          // onZoomChange={(z) => setZoomLevel(Math.max(z, 100))}
          // zoomLevel={zoomLevel}

          // TODO these guys should probably be figured out internally
          scrollProgress={0}
          hoverTimestamp={0}
          onHoverTimestampChange={() => { }}
          domainStart={0}
          domainEnd={0}
        />
      </div>
    </div>
  );
});

MockGraph.displayName = "MockGraph";

export default MockGraph;
