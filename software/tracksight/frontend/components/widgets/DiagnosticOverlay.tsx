"use client";

import { useEffect, useState, useMemo } from "react";
import { useSocket } from "@/lib/hooks/signals/useSocket";

const MAX_HISTORY = 60;

export function DiagnosticOverlay() {
  const [value, setValue] = useState<number | null>(null);
  const [history, setHistory] = useState<number[]>([]);
  const [isHovered, setIsHovered] = useState(false);
  const socket = useSocket();

  useEffect(() => {
    const handleDiagnostic = (payload: any) => {
      if (payload.type === "packet_loss") {
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
    const width = 120;
    const height = 30;
    const maxVal = Math.max(5, ...history); // Scale to at least 5% for visibility
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
      className={`fixed top-20 right-6 z-[100] flex flex-col items-center justify-center p-3 bg-white/80 backdrop-blur-sm rounded-lg shadow-lg border border-gray-200 transition-all duration-300 ease-in-out pointer-events-auto ${
        isHovered ? "w-40 h-32" : "w-20 h-16"
      }`}
    >
      <span className="text-[10px] font-bold text-gray-400 uppercase tracking-tighter mb-0.5">
        Packet Loss
      </span>
      <div className="flex flex-col items-center">
        <div className="flex items-baseline gap-0.5">
          <span className={`${isHovered ? "text-2xl" : "text-xl"} font-black text-gray-800 tabular-nums transition-all`}>
            {value !== null ? value.toFixed(1) : "--"}
          </span>
          <span className="text-xs font-bold text-gray-400">%</span>
        </div>
        
        {!isHovered && value !== null && (
          <div className="mt-1 w-10 bg-gray-200 rounded-full h-1 overflow-hidden">
            <div
              className={`h-full transition-all duration-500 ${
                value > 5 ? "bg-red-500" : value > 1 ? "bg-yellow-500" : "bg-green-500"
              }`}
              style={{ width: `${Math.min(100, value)}%` }}
            />
          </div>
        )}

        {isHovered && (
          <div className="mt-2 w-full flex flex-col items-center animate-in fade-in zoom-in duration-200">
             <svg width="120" height="30" className="overflow-visible">
                <polyline
                  fill="none"
                  stroke={value !== null && value > 5 ? "#ef4444" : value !== null && value > 1 ? "#eab308" : "#22c55e"}
                  strokeWidth="2"
                  strokeLinecap="round"
                  strokeLinejoin="round"
                  points={sparklinePoints}
                />
             </svg>
             <span className="text-[8px] text-gray-400 mt-2 uppercase font-bold tracking-widest">60s History</span>
          </div>
        )}
      </div>
    </div>
  );
}
