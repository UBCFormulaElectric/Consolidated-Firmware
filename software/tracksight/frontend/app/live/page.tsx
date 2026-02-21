"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
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
    <div id="live-page" className="pt-14 h-screen">
      <DisplayControlProvider>
        <SignalProvider>
          <SyncedGraphContainer>
            <PausePlayButton />
            {/** TODO(evan): Add the universal timeline here */}
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
          </SyncedGraphContainer>
        </SignalProvider >
      </DisplayControlProvider >
    </div>
  );
}
