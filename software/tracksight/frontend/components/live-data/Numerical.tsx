// numerical.tsx
"use client";

import { usePausePlay } from "@/components/shared/pause-play-control";
import { PlusButton } from "@/components/shared/PlusButton";
import { SignalType, useSignals } from "@/lib/contexts/SignalContext";
import React, { useCallback, useEffect, useState } from "react";
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

  useEffect(() => {
    if (signalName && !activeSignals.includes(signalName)) {
      subscribeToSignal(signalName, SignalType.Numerical);
    }
  }, [signalName, activeSignals, subscribeToSignal]);

  const chartData = React.useMemo(() => {
    const windowMs = 100;
    const map: Record<number, any> = {};

    numericalData
      .filter(
        (d) =>
          activeSignals.includes(d.name as string) &&
          isNumericalSignal(d.name as string)
      )
      .forEach((d) => {
        const t =
          typeof d.time === "number" ? d.time : new Date(d.time).getTime();
        const r = Math.floor(t / windowMs) * windowMs;
        if (!map[r]) map[r] = { time: r };
        map[r][d.name as string] = d.value;
      });

    return Object.values(map).sort((a, b) => a.time - b.time);
  }, [numericalData, activeSignals, isNumericalSignal]);

  const numericalSignals = React.useMemo(
    () => activeSignals.filter((sig) => isNumericalSignal(sig)),
    [activeSignals, isNumericalSignal]
  );

  const availableOptions = React.useMemo(
    () =>
      availableSignals.filter(
        (s) => !activeSignals.includes(s.name) && isNumericalSignal(s.name)
      ),
    [availableSignals, activeSignals, isNumericalSignal]
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
      setSearchTerm("");
      setIsSearchOpen(false);
    },
    [subscribeToSignal]
  );

  const handleUnsub = useCallback(
    (name: string) => unsubscribeFromSignal(name),
    [unsubscribeFromSignal]
  );

  const handleDelete = useCallback(() => {
    numericalSignals.forEach((n) => unsubscribeFromSignal(n));
    onDelete();
  }, [numericalSignals, unsubscribeFromSignal, onDelete]);

  return (
    <div className="mb-6 p-4 inline-block w-min-[100vm]">
      {isPaused && (
        <div className="top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10 block">
          PAUSED
        </div>
      )}
      <div className=" sticky left-0 block w-[50vw] animate-none overscroll-contain">
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
            <div className="relative">
              <PlusButton
                onClick={() => setIsSearchOpen((o) => !o)}
                variant="rowSide"
              />
              {isSearchOpen && (
                <div className="absolute top-full left-0 mt-1 bg-white border rounded shadow w-64 p-2 z-10">
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
