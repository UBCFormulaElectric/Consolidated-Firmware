"use client";

import { useEffect, useState, useMemo } from "react";
import { useSocket } from "@/lib/hooks/signals/useSocket";

const MAX_HISTORY = 60;

export function ErrorRateIndicator() {
  const [value, setValue] = useState<number | null>(null);
  const [history, setHistory] = useState<number[]>([]);
  const [isHovered, setIsHovered] = useState(false);
  const socket = useSocket();

  useEffect(() => {
    const handleDiagnostic = (payload: any) => {
      if (payload.type === "error_rate") {
        const val = payload.value;
        setValue(val);
        setHistory((prev) => {
          const next = [...prev, val];
          if (next.length > MAX_HISTORY) return next.slice(1);
          return next;
        });
      }
    };

    socket.on("diagnostic", handleDiagnostic);
    return () => {
      socket.off("diagnostic", handleDiagnostic);
    };
  }, [socket]);

  const sparklinePoints = useMemo(() => {
    if (history.length < 2) return "";
    const width = 100;
    const height = 20;
    const maxVal = Math.max(5, ...history);
    return history
      .map((val, i) => {
        const x = (i / (MAX_HISTORY - 1)) * width;
        const y = height - (val / maxVal) * height;
        return `${x},${y}`;
      })
      .join(" ");
  }, [history]);

  return (
    <div
      onMouseEnter={() => setIsHovered(true)}
      onMouseLeave={() => setIsHovered(false)}
      className="relative flex items-center px-3 py-1 rounded-md transition-colors hover:bg-gray-50 cursor-default"
    >
      <div className="flex flex-col items-end mr-2">
        <span className="text-[9px] font-bold text-gray-400 uppercase tracking-tighter leading-none">
          Error Rate (1m)
        </span>
        <div className="flex items-baseline gap-0.5 leading-none">
          <span className="text-sm font-black text-gray-800 tabular-nums">
            {value !== null ? value.toFixed(1) : "--"}
          </span>
          <span className="text-[10px] font-bold text-gray-400">%</span>
        </div>
      </div>

      <div className={`w-1.5 h-6 rounded-full overflow-hidden bg-gray-100`}>
        <div
          className={`w-full transition-all duration-500 ${
            value !== null && value > 5 ? "bg-red-500" : value !== null && value > 1 ? "bg-yellow-500" : "bg-green-500"
          }`}
          style={{ height: `${value !== null ? Math.min(100, value * 10) : 0}%`, marginTop: 'auto' }}
        />
      </div>

      {isHovered && (
        <div className="absolute top-full right-0 mt-1 p-3 bg-white rounded-lg shadow-xl border border-gray-100 z-[60] animate-in fade-in slide-in-from-top-1 duration-200">
          <div className="flex flex-col items-center">
             <svg width="100" height="20" className="overflow-visible">
                <polyline
                  fill="none"
                  stroke={value !== null && value > 5 ? "#ef4444" : value !== null && value > 1 ? "#eab308" : "#22c55e"}
                  strokeWidth="1.5"
                  strokeLinecap="round"
                  strokeLinejoin="round"
                  points={sparklinePoints}
                />
             </svg>
             <span className="text-[8px] text-gray-400 mt-2 uppercase font-bold tracking-widest whitespace-nowrap">1m Rolling</span>
          </div>
        </div>
      )}
    </div>
  );
}
