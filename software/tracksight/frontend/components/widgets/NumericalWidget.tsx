"use client";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import CanvasChart, { AlignedData } from "@/components/widgets/CanvasChart";
import { useSignals } from "@/lib/contexts/SignalContext";
import { makeRingArrayView, NumericRingBuffer } from "@/lib/data/NumericRingBuffer";
import { WidgetDataNumerical } from "@/lib/types/Widget";
import React, { useEffect, useMemo, useRef, useState } from "react";

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

const SIGNAL_NAME = "BMS_TractiveSystemVoltage";

// Size this to your live viewing window. Example: 2000 Hz * 60s = 120k points.
const RING_CAPACITY = 120_000;
const TIME_RANGE_UPDATE_EVERY_N_POINTS = 25;

export default function NumericalWidget({ onDelete }: NumericalWidgetProps) {
  const { subscribeToSignal, unsubscribeFromSignal, addSignalListener } = useSignals();
  const { setTimeRange, timeRangeRef } = useSyncedGraph();

  const ringRef = useRef<NumericRingBuffer>(new NumericRingBuffer(RING_CAPACITY));
  const pointsSinceRangeUpdateRef = useRef(0);

  const [chartHeight, setChartHeight] = useState(256);
  const [scalingMode, setScalingMode] = useState<"absolute" | "relative">("absolute");

  useEffect(() => {
    subscribeToSignal(SIGNAL_NAME);

    const removeListener = addSignalListener(SIGNAL_NAME, (point) => {
      if (point.value == null) return;

      const timestamp =
        typeof point.time === "number"
          ? point.time
          : typeof point.time === "string"
            ? new Date(point.time).getTime()
            : Date.now();

      const rawValue = point.value;
      const value = typeof rawValue === "number" ? rawValue : Number.parseFloat(String(rawValue));
      if (!Number.isFinite(value)) return;

      ringRef.current.push(timestamp, value);

      pointsSinceRangeUpdateRef.current += 1;
      if (pointsSinceRangeUpdateRef.current >= TIME_RANGE_UPDATE_EVERY_N_POINTS) {
        pointsSinceRangeUpdateRef.current = 0;
        const ring = ringRef.current;
        if (ring.length > 0) {
          const min = ring.timestampAt(0);
          const max = ring.timestampAt(ring.length - 1);
          if (Number.isFinite(min) && Number.isFinite(max)) {
            setTimeRange({
              min: Math.min(min, timeRangeRef.current?.min ?? min),
              max: Math.max(max, timeRangeRef.current?.max ?? max),
            });
          }
        }
      }
    });

    return () => {
      removeListener();
      unsubscribeFromSignal(SIGNAL_NAME);
    };
  }, [subscribeToSignal, unsubscribeFromSignal, addSignalListener, setTimeRange, timeRangeRef]);

  const chartData = useMemo<AlignedData>(() => {
    const ring = ringRef.current;
    return {
      timestamps: makeRingArrayView(ring, "timestamps"),
      series: [makeRingArrayView(ring, "values")],
    };
  }, []);

  const seriesMeta = useMemo(
    () => [
      {
        label: SIGNAL_NAME,
        color: signalColors[0],
      },
    ],
    []
  );

  return (
    <div className="mb-6 p-4 block w-full relative">
      <div className="flex items-center gap-2 mb-4">
        <h3 className="font-semibold">{SIGNAL_NAME}</h3>
        {onDelete && (
          <button
            onClick={onDelete}
            className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors"
            title="Remove graph"
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
        <div className="flex flex-col">
          <label className="text-sm">Scaling Mode</label>
          <div className="flex bg-gray-100 p-1 rounded-md">
            <button
              className={`px-3 py-1 text-sm rounded ${
                scalingMode === "absolute"
                  ? "bg-white shadow text-black"
                  : "text-gray-500 hover:text-gray-900"
              }`}
              onClick={() => setScalingMode("absolute")}
            >
              Absolute
            </button>
            <button
              className={`px-3 py-1 text-sm rounded ${
                scalingMode === "relative"
                  ? "bg-white shadow text-black"
                  : "text-gray-500 hover:text-gray-900"
              }`}
              onClick={() => setScalingMode("relative")}
            >
              Relative
            </button>
          </div>
        </div>
      </div>

      <div style={{ height: chartHeight }} className="overflow-hidden relative">
        <CanvasChart
          data={chartData}
          height={chartHeight}
          scalingMode={scalingMode}
          series={seriesMeta}
        />
      </div>
    </div>
  );
}
