"use client";

import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import { useScrollableRef } from "@/lib/contexts/ScrollContext";
import { Widget } from "./widgets/Widget";

function DataDashboard() {
  const { widgets } = useDashboardLayout();

  const {
    scrollableRef,
    childRef
  } = useScrollableRef();

  return (
    <div
      className="w-full h-fit overflow-x-scroll scrollbar-hidden overscroll-x-none"
      ref={scrollableRef}
    >
      <div
        className="flex h-full min-w-[200vw] flex-col gap-16"
        ref={childRef}
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

export default DataDashboard;
