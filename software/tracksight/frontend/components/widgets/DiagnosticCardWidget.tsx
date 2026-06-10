"use client";

import { useEffect, useState } from "react";
import { useSocket } from "@/lib/hooks/signals/useSocket";
import { DiagnosticCardWidgetData } from "@/lib/types/Widget";

export function DiagnosticCardWidget(props: DiagnosticCardWidgetData) {
  const [value, setValue] = useState<number | null>(null);
  const socket = useSocket();

  useEffect(() => {
    const handleDiagnostic = (payload: any) => {
      if (payload.type === props.metric) {
        setValue(payload.value);
      }
    };

    socket.on("diagnostic", handleDiagnostic);
    return () => {
      socket.off("diagnostic", handleDiagnostic);
    };
  }, [socket, props.metric]);

  return (
    <div className="flex flex-col items-center justify-center p-6 bg-white rounded-xl shadow-sm border border-gray-100 h-full min-h-[120px]">
      <span className="text-sm font-semibold text-gray-500 uppercase tracking-wider mb-2">
        {props.options.label}
      </span>
      <div className="flex items-baseline gap-1">
        <span className="text-4xl font-bold text-gray-900">
          {value !== null ? value.toFixed(2) : "--"}
        </span>
        <span className="text-lg font-medium text-gray-400">%</span>
      </div>
      {value !== null && (
        <div className="mt-2 w-full bg-gray-100 rounded-full h-1.5 overflow-hidden">
          <div
            className={`h-full transition-all duration-500 ${
              value > 5 ? "bg-red-500" : value > 1 ? "bg-yellow-500" : "bg-green-500"
            }`}
            style={{ width: `${Math.min(100, value)}%` }}
          />
        </div>
      )}
    </div>
  );
}
