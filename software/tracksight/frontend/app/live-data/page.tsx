"use client";

import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
import DynamicRowManager from "@/components/shared/DynamicRowManager";
import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";
import SyncedGraphContainer from "@/components/shared/SyncedGraphContainer";
import { SignalProvider } from "@/hooks/SignalContext";
import { useEffect, useRef, useState } from "react";

export default function LiveDataPage() {
  const [scrollProgress, setScrollProgress] = useState<number>(0);

  return (
    <SignalProvider>
      <div
        className="overflow-x-scroll pt-24 min-h-screen"
        onScroll={(e) => {
          setScrollProgress(e.currentTarget.scrollLeft);
          console.log(e.currentTarget.scrollLeft);
        }}
      >
        <div style={{ width: "3000px" }}>
          <div className="sticky w-screen left-0">
            <h2 className="text-xl font-bold mb-2">Connection Status</h2>
            <ConnectionStatus />

            {/* TODO rework alerts */}
            {/* <h2 className="text-xl font-bold mb-2">Alerts</h2>
            <AlertBoard /> */}

            <h2 className="text-xl font-bold mb-2">
              Add New Signal Subscriptions
            </h2>
            <DynamicRowManager />

            <SyncedGraphContainer scroll={scrollProgress} />
            {/* big orange boy
            <div className="bg-orange-500 h-24 right-0 absolute" style={{ width: "500px" }} />
            */}
          </div>
        </div>
      </div>
    </SignalProvider>
  );
}
