"use client";

// import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
// import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";
import { DisplayControlProvider, DownsampleControls, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/components/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import useWidgetManager from "@/components/widgets/useWidgetManager";

export default function LiveDataPage() {
  const {
    widgets,
    setEnumSignal,
    appendNumSignal,
    removeNumSignal,
    removeWidget,
    updateWidget,
    appendWidget
  } = useWidgetManager();

  return (
    <DisplayControlProvider> {/* not so sure actually (OLD) */}
      <SignalProvider> {/* provides signal data (OLD) */}
        <SyncedGraphContainer>
          <div className="sticky top-0 left-0 z-50 w-screen bg-white/80 backdrop-blur border-b">
            <div className="px-4 py-2 flex items-center gap-3">
              <PausePlayButton />
              <DownsampleControls />
            </div>
          </div>
          {/** TODO(evan): Add the universal timeline here */}
          {/* TODO rework alerts */}
          {/* <h2 className="text-xl font-bold mb-2">Alerts</h2> */}
          {/* <AlertBoard /> */}
          {/* <h2 className="text-xl font-bold mb-2">Connection Status</h2> */}
          {/* <ConnectionStatus /> */}
          {widgets.map(widgetData => <Widget
            widgetData={widgetData}
            setEnumSignal={setEnumSignal}
            appendNumSignal={appendNumSignal}
            removeNumSignal={removeNumSignal}
            removeWidget={removeWidget}
            updateWidget={updateWidget}
            key={widgetData.id} />
          )}
          <WidgetAdder onAddWidget={appendWidget} />
          {/* <DynamicRowManager /> */}
        </SyncedGraphContainer>
      </SignalProvider >
    </DisplayControlProvider >
  );
}
