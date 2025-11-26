"use client";

import { useRef } from "react";

import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import { LiveSignalDataStoreProvider } from "@/lib/contexts/LiveSignalDataStore";

function LiveDataDashboard() {
  const { widgets } = useDashboardLayout();

  const containerRef = useRef<HTMLDivElement>(null);

  return (
    <LiveSignalDataStoreProvider>
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
    </LiveSignalDataStoreProvider>
  );
}

export default LiveDataDashboard;
