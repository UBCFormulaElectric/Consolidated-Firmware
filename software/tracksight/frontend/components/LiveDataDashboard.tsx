"use client";

import type { FC } from "react";
import { useRef } from "react";

import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";

const LiveDataDashboard: FC = () => {
  const { widgets } = useDashboardLayout();

  const containerRef = useRef<HTMLDivElement>(null);

  return (
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
  );
};

export default LiveDataDashboard;
