"use client";

// import AlertBoard from "@/components/shared/AlertBoard/AlertBoard";
// import ConnectionStatus from "@/components/pages/live-data/ConnectionStatus";
import { DisplayControlProvider } from "@/components/shared/PausePlayControl";
import SyncedGraphContainer from "@/components/shared/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/components/shared/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import useWidgetManager from "@/components/widgets/useWidgetManager";

export default function LiveDataPage() {
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
    <DisplayControlProvider> {/* not so sure actually (OLD) */}
      <SignalProvider> {/* provides signal data (OLD) */}
        <SyncedGraphContainer>
          {/** TODO(evan): Add pause / play button here */}
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
            updateMockConfig={updateMockConfig}
            key={widgetData.id} />
          )}
          <WidgetAdder onAddWidget={appendWidget} />
          {/* <DynamicRowManager /> */}
        </SyncedGraphContainer>
      </SignalProvider >
    </DisplayControlProvider >
  );
}
