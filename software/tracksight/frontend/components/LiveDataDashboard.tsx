"use client";

import { useRef } from "react";

import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";

// TODO(evan): Rename to just DataDashboard since it can be used for both live and historical data
function LiveDataDashboard() {
  const { widgets } = useDashboardLayout();

  const containerRef = useRef<HTMLDivElement>(null);

  return (
    <div className="w-full h-fit overflow-x-scroll scrollbar-hidden overscroll-x-none">
      <div
        className="flex h-full min-w-[200vw] flex-col gap-16"
        ref={containerRef}
      >
        {widgets.map((widget, index) => (
          <div className="sticky left-0 w-screen" style={{ zIndex: widgets.length - index }}>
            <Widget {...widget} />
          </div>
        ))}
      </div>
    </div>
  );
}

export default LiveDataDashboard;
