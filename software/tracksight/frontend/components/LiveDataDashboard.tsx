"use client";

import React from "react";

import Widget from "@/components/widgets/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";

const LiveDataDashboard: React.FC = () => {
  const { widgets } = useDashboardLayout();

  return (
    <div
      className="flex h-full min-w-[200vw] flex-col gap-16"
      id="data-dashboard-container"
    >
      {widgets.map((widget) => (
        <div className="sticky left-0 w-screen">
          <Widget {...widget} />
        </div>
      ))}
    </div>
  );
};

export default LiveDataDashboard;
