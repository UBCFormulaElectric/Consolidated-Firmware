"use client";

import React, { useEffect, useState, useMemo, useRef } from "react";
import CanvasChart from "@/components/widgets/CanvasChart";
import { usePausePlay, PausePlayButton } from "@/components/PausePlayControl";
import { PlusButton } from "@/components/PlusButton";
import { useSyncedGraphScroll } from "@/components/SyncedGraphContainer";
import { MockGraphConfig, WidgetDataMock } from "@/lib/types/Widget";
import { dataTagErrorSymbol } from "@tanstack/react-query";

enum MockSignalType {
  Numerical = "numerical",
  Enumeration = "enumeration",
}

interface MockWidgetProps {
  widgetData: WidgetDataMock;
  updateMockConfig: (widgetId: string, updater: (prevConfigs: MockGraphConfig[]) => MockGraphConfig[]) => void;
  onDelete: () => void;
}

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

const MockWidget: React.FC<MockWidgetProps> = React.memo(
  ({ widgetData, updateMockConfig, onDelete }) => {
    const { isPaused } = usePausePlay();
    const configs = widgetData.configs;

    const [showAddModal, setShowAddModal] = useState(false);
    const [newSignalName, setNewSignalName] = useState("");
    const [newSignalType, setNewSignalType] = useState<MockSignalType>(
      MockSignalType.Numerical
    );
    const [newSignalDelay, setNewSignalDelay] = useState(100);

    /*const [data, setData] = useState<{
      timestamps: number[];
      series: Record<string, (number | string | null)[]>;
    }>({ timestamps: [], series: {} });*/

    const dataRef = useRef<{
      timestamps: number[];
      series: Record<string, (number | string | null)[]>;
    }>({ timestamps: [], series: {} });

    // dirty flag to indicate that the data is not up to date
    const isDirtyRef = useRef(false);
    const isLiveRef = useRef(true);

    const [tick, setTick] = useState(0);

    const [chartHeight, setChartHeight] = useState(256);
    const {
      progress: scrollProgress,
      zoomLevel,
      setZoomLevel,
      setProgress: setScrollProgress,
      hoverTimestamp,
      setHoverTimestamp,
      registerTimeRange,
      unregisterTimeRange,
      globalTimeRange,
    } = useSyncedGraphScroll();

    useEffect(() => {
      isLiveRef.current = scrollProgress >= 0.99;
    }, [scrollProgress]);

    const graphId = widgetData.id;

    useEffect(() => {
      const ts = dataRef.current.timestamps;
      if (ts.length > 0) {
        const min = ts[0];
        const max = ts[ts.length - 1];
        registerTimeRange(graphId, min, max);
      }
      return () => unregisterTimeRange(graphId);
      //eslint-disable-next-line react-hooks/exhaustive-deps
    }, [tick, graphId, registerTimeRange, unregisterTimeRange]); //udpate on tick

    const generateRandomValue = (
      type: string,
      time: number,
      index: number = 0
    ) => {
      if (type === MockSignalType.Enumeration) {
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
    };

    // TODO: could add initialPoints loop here (but this is very optional)
    // it'd just be what we use in MockGraph.tsx

    useEffect(() => {
      if (isPaused) return;

      const intervals: NodeJS.Timeout[] = [];

      configs.forEach((cfg, idx) => {
        const interval = setInterval(() => {
          const now = Date.now();
          const val = generateRandomValue(cfg.type, now, idx);

          /*setData((prev) => {
            const newTimestamps = [...prev.timestamps, now];
            const newSeries = { ...prev.series };

            configs.forEach((c) => {
              if (!newSeries[c.signalName]) {
                newSeries[c.signalName] = new Array(
                  prev.timestamps.length
                ).fill(null);
              }
            });

            Object.keys(newSeries).forEach((key) => {
              if (key === cfg.signalName) {
                newSeries[key] = [...newSeries[key], val];
              } else {
                // zero order hold
                const lastVal =
                  newSeries[key].length > 0
                    ? newSeries[key][newSeries[key].length - 1]
                    : null;
                newSeries[key] = [...newSeries[key], lastVal];
              }
            });

            return { timestamps: newTimestamps, series: newSeries };
          });*/
          const store = dataRef.current;

          configs.forEach((c) => {
            if (!store.series[c.signalName]) {
              store.series[c.signalName] = new Array(store.timestamps.length).fill(null);
            }
          });

          isDirtyRef.current = true;
          store.timestamps.push(now);

          Object.keys(store.series).forEach((key) => {
            if (key === cfg.signalName) {
              store.series[key].push(val);
            } else {
              const arr = store.series[key];
              const lastVal = arr.length > 0 ? arr[arr.length - 1] : null;
              arr.push(lastVal);
            }
          });
        }, cfg.delay);
        intervals.push(interval);
      });

      return () => intervals.forEach(clearInterval);
    }, [configs, isPaused]);

    useEffect(() => {
      let frameId: number;
      const loop = () => {
        frameId = requestAnimationFrame(loop);
        if (isDirtyRef.current && isLiveRef.current) {
          setTick(t => t + 1);
          isDirtyRef.current = false;
        }
      };
      frameId = requestAnimationFrame(loop);
      return () => cancelAnimationFrame(frameId);
    }, []);

    const chartData = useMemo<
      [number[], ...Array<(number | string | null)[]>]
    >(() => {
      //const seriesArrays = configs.map((c) => data.series[c.signalName] || []);
      //if (data.timestamps.length === 0) {
      // read from ref ontick
      const { timestamps, series } = dataRef.current;
      const seriesArrays = configs.map((c) => series[c.signalName] || []);

      if (timestamps.length === 0) {
        return [[], ...configs.map(() => [])];
      }
      //return [data.timestamps, ...seriesArrays];
      //}, [data, configs]);
      return [timestamps, ...seriesArrays];
    }, [tick, configs]); // dependency now tick instead of data

    const handleSnapToLatest = () => setScrollProgress(1);

    const handleAddSignal = (e: React.FormEvent) => {
      e.preventDefault();
      const name = newSignalName.trim() || `Signal ${configs.length + 1}`;
      if (configs.some((c) => c.signalName === name)) {
        alert("Signal name must be unique in this graph");
        return;
      }

      const newConfig: MockGraphConfig = {
        signalName: name,
        type: newSignalType, // "numerical" | "enumeration"
        delay: newSignalDelay,
        initialPoints: 0,
      };

      // initialize data series for the new signal with nulls
      /*setData((prev) => {
        const newSeries = { ...prev.series };
        newSeries[name] = new Array(prev.timestamps.length).fill(null);
        return { ...prev, series: newSeries };
      });*/

      const store = dataRef.current;
      store.series[name] = new Array(store.timestamps.length).fill(null);

      updateMockConfig(widgetData.id, (prev) => [...prev, newConfig]);
      setShowAddModal(false);
      setNewSignalName("");
      setNewSignalType(MockSignalType.Numerical);
    };

    const handleRemoveSignal = (name: string) => {
      if (configs.length <= 1) {
        onDelete();
        return;
      }
      updateMockConfig(widgetData.id, (prev) => prev.filter((c) => c.signalName !== name));
      /*setData((prev) => {
        const nextSeries = { ...prev.series };
        delete nextSeries[name];
        return { ...prev, series: nextSeries };
      });*/
      if (dataRef.current.series[name]) {
        delete dataRef.current.series[name];
      }
    };

    const visiblePointsCount = React.useMemo(() => {
      // ... simplified visible points logic or just use timestamps.length if lazy ...
      // reusing logic from MockGraph.tsx would be verbose but good for accuracy
      //return data.timestamps.length; 
      //}, [data.timestamps]);

      //const totalDataPoints = data.timestamps.length;
      return dataRef.current.timestamps.length;
    }, [tick]);

    const totalDataPoints = dataRef.current.timestamps.length;

    return (
      <div className="mb-6 p-4 block w-full relative">
        <div className="sticky left-0 block w-[50vw] animate-none overscroll-contain z-40">
          <div className="flex items-center gap-2 mb-4">
            <h3 className="font-semibold">Mock Graph Container (Widget)</h3>
            <PausePlayButton />
            <button
              onClick={onDelete}
              className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
              title="Remove mock graph"
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
                    ×
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
                  className="fixed inset-0 bg-black/50 flex items-center justify-center z-[100]"
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
                            setNewSignalType(e.target.value as MockSignalType)
                          }
                          className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        >
                          <option value={MockSignalType.Numerical}>
                            Numerical
                          </option>
                          <option value={MockSignalType.Enumeration}>
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
          <div className="text-xs text-gray-500 mb-4 space-y-1 bg-gray-50 p-2 rounded border">
            <div>Total points: {totalDataPoints}</div>
            <div>Points in view: {visiblePointsCount}</div>
            <div>Scroll Progress: {(scrollProgress * 100).toFixed(1)}%</div>
          </div>
        </div>

        <div
          style={{
            width:
              typeof window !== "undefined" ? window.innerWidth - 100 : 1200,
            height: chartHeight,
            position: "relative",
            overflow: "hidden",
          }}
        >
          <CanvasChart
            data={chartData}
            width={
              typeof window !== "undefined" ? window.innerWidth - 100 : 1200
            }
            height={chartHeight}
            series={configs.map((c, idx) => ({
              label: c.signalName,
              color: signalColors[idx % signalColors.length],
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
      </div>
    );
  }
);

MockWidget.displayName = "MockWidget";

export default MockWidget;

