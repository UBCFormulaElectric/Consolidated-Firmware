"use client";

import { useWidgetManager } from "./widgets/WidgetManagerContext";
import { Widget } from "./widgets/Widget";
import { useRef } from "react";

function DataDashboard() {
  const { widgets } = useWidgetManager();


  const hoveredSignal = useRef<string | null>(null);

  return (
    <div
      className="w-full h-fit overflow-x-scroll scrollbar-hidden overscroll-x-none"
    >
      <div
        className="flex h-full min-w-[200vw] flex-col gap-16"
      >
        {widgets.map((widget, index) => (
          <div key={widget.id} className="sticky left-0 w-screen" style={{ zIndex: widgets.length - index }}>
            <Widget {...widget} hoveredSignal={hoveredSignal} />
          </div>
        ))}
      </div>
    </div>
  );
}

export default DataDashboard;
