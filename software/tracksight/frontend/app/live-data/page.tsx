"use client";

// import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
// import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";

import { useRef } from "react";
import { DisplayControlProvider } from "@/components/shared/PausePlayControl";
import SyncedGraphContainer from "@/components/shared/SyncedGraphContainer";
import { SignalProvider } from "@/hooks/SignalContext";
import DynamicRowManager from "@/components/shared/DynamicRowManager";
import { WidgetAdder } from "@/components/shared/WidgetAdder";
import { Widget } from "./Widget";
import useWidgetManager from "./useWidgetManager";

export default function LiveDataPage() {
  const containerRef = useRef<HTMLDivElement>(null);
  const { 
    widgets, 
    setEnumSignal, 
    appendNumSignal, 
    removeNumSignal,
    removeWidget,
    updateMockConfig,
    appendWidget
  } = useWidgetManager();
  
  return (
    <div>
      <DisplayControlProvider> {/* not so sure actually (OLD) */}
        <SignalProvider> {/* provides signal data (OLD) */}
          <div className="pt-24 min-h-screen">
            <div className="sticky w-screen left-0" ref={containerRef}>
              {/* TODO rework alerts */}
              {/* <h2 className="text-xl font-bold mb-2">Alerts</h2> */}
              {/* <AlertBoard /> */}
              {/* <h2 className="text-xl font-bold mb-2">Connection Status</h2> */}
              {/* <ConnectionStatus /> */}

              <h2 className="text-xl font-bold mb-2">
                Add New Signal Subscriptions
              </h2>
              <SyncedGraphContainer>
                {widgets.map(widgetData => <Widget
                  widgetData={widgetData}
                  setEnumSignal={setEnumSignal}
                  appendNumSignal={appendNumSignal}
                  removeNumSignal={removeNumSignal}
                  removeWidget={removeWidget}
                  updateMockConfig={updateMockConfig}
                  key={widgetData.id} />
                )}
                <WidgetAdder onAddWidget={appendWidget} />
                <DynamicRowManager />
              </SyncedGraphContainer>
            </div>
          </div>
        </SignalProvider>
      </DisplayControlProvider>
    </div>
  );
}
