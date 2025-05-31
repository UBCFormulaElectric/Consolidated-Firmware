// enumeration.tsx
"use client";

import { usePausePlay } from "@/components/shared/pause-play-control";
import { SignalType, useSignals } from "@/lib/contexts/SignalContext";
import React, { useCallback, useEffect, useMemo, useState } from "react";

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
  const { isPaused } = usePausePlay();
  const {
    enumData,
    subscribeToSignal,
    unsubscribeFromSignal,
    getEnumValues,
    mapEnumValue,
  } = useSignals();
  const [pausedTime, setPausedTime] = useState<number | null>(null);

  useEffect(() => {
    if (signalName) {
      subscribeToSignal(signalName, SignalType.Enumeration);
    }
  }, [signalName, subscribeToSignal]);

  useEffect(() => {
    if (isPaused && pausedTime === null) {
      setPausedTime(Date.now());
    } else if (!isPaused) {
      setPausedTime(null);
    }
  }, [isPaused, pausedTime]);

  const history = useMemo(() => {
    const raw = enumData
      .filter((dp) => dp.name === signalName && dp.value != null)
      .map((dp) => ({
        state: String(dp.value),
        startTime:
          typeof dp.time === "number" ? dp.time : new Date(dp.time).getTime(),
      }));
    const items: { state: string; startTime: number }[] = [];
    raw.forEach((pt) => {
      const last = items[items.length - 1];
      if (!last || last.state !== pt.state) items.push(pt);
    });
    return items;
  }, [enumData, signalName]);

  const computeBars = useCallback(
    (hist: { state: string; startTime: number }[]) => {
      const windowMs = 5000;
      const width = typeof window !== "undefined" ? window.innerWidth : 1000;
      const pxPerMs = width / windowMs;
      const now = pausedTime !== null && isPaused ? pausedTime : Date.now();

      let offset = 0;
      return hist
        .map((item, i) => {
          const next = hist[i + 1];
          const end = next ? next.startTime : now;
          const w = (end - item.startTime) * pxPerMs;
          const bar =
            w > 0
              ? {
                  state: item.state,
                  startTime: item.startTime,
                  width: w,
                  xOffset: offset,
                }
              : null;
          if (bar) offset += bar.width;
          return bar;
        })
        .filter(Boolean) as {
        state: string;
        startTime: number;
        width: number;
        xOffset: number;
      }[];
    },
    [isPaused, pausedTime]
  );

  const bars = computeBars(history);
  const enumVals = getEnumValues(signalName);
  const lastCode = history.length
    ? history[history.length - 1].state
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
    unsubscribeFromSignal(signalName);
    onDelete();
  }, [signalName, unsubscribeFromSignal, onDelete]);

  return (
    <div className="mb-6 p-4 inline-block">
      {isPaused && (
        <div className="absolute top-2 left-2 bg-red-500 text-white px-2 py-1 rounded text-xs z-10">
          PAUSED
        </div>
      )}
      <div className=" sticky left-0 block w-svw">
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

      <div className="relative inline-block">
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

      {!history.length && (
        <div className="text-gray-500 mt-4">
          Waiting for enumeration data from {signalName}...
        </div>
      )}
    </div>
  );
};

export default EnumerationGraphComponent;
