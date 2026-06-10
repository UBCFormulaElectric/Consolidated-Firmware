"use client";

import { useEffect, useState } from "react";
import { useSocket } from "@/lib/hooks/signals/useSocket";

export function DiagnosticOverlay() {
  const [value, setValue] = useState<number | null>(null);
  const socket = useSocket();

  useEffect(() => {
    const handleDiagnostic = (payload: any) => {
      if (payload.type === "packet_loss") {
        setValue(payload.value);
      }
    };

    socket.on("diagnostic", handleDiagnostic);
    return () => {
      socket.off("diagnostic", handleDiagnostic);
    };
  }, [socket]);

  return (
    <div className="fixed bottom-6 right-6 z-[100] flex flex-col items-center justify-center p-3 bg-white/80 backdrop-blur-sm rounded-lg shadow-lg border border-gray-200 pointer-events-auto">
      <span className="text-[10px] font-bold text-gray-400 uppercase tracking-tighter mb-0.5">
        Packet Loss
      </span>
      <div className="flex flex-col items-center">
        <div className="flex items-baseline gap-0.5">
          <span className="text-xl font-black text-gray-800 tabular-nums">
            {value !== null ? value.toFixed(1) : "--"}
          </span>
          <span className="text-xs font-bold text-gray-400">%</span>
        </div>
        {value !== null && (
          <div className="mt-1 w-10 bg-gray-200 rounded-full h-1 overflow-hidden">
            <div
              className={`h-full transition-all duration-500 ${
                value > 5 ? "bg-red-500" : value > 1 ? "bg-yellow-500" : "bg-green-500"
              }`}
              style={{ width: `${Math.min(100, value)}%` }}
            />
          </div>
        )}
      </div>
    </div>
  );
}
