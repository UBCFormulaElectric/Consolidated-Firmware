"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";

function Content() {
  const { widgets } = useWidgetManager();

  return (
    <>
      <PausePlayButton />
      {widgets.map(widgetData => // TODO(evan): Add the universal timeline
        <Widget widgetData={widgetData} key={widgetData.id} />
      )}
      <WidgetAdder />
    </>
  );
}

export default function LiveDataPage() {
  return (
    <div id="live-page" className="pt-14 h-screen">
      <DisplayControlProvider>
        <SignalProvider>
          <SyncedGraphContainer>
            <WidgetManager>
              <Content />
            </WidgetManager>
          </SyncedGraphContainer>
        </SignalProvider >
      </DisplayControlProvider >
    </div>
  );
}
