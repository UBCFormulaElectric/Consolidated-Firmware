"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";

export default function LiveDataPage() {
  const { widgets } = useWidgetManager();
  return (
    <div id="live-page" className="pt-14 h-screen">
      <DisplayControlProvider>
        <SignalProvider>
          <SyncedGraphContainer>
            <WidgetManager>
              <PausePlayButton />
              {/** TODO(evan): Add the universal timeline here */}
              {widgets.map(widgetData =>
                <Widget widgetData={widgetData} key={widgetData.id} />
              )}
              <WidgetAdder />
            </WidgetManager>
          </SyncedGraphContainer>
        </SignalProvider >
      </DisplayControlProvider >
    </div>
  );
}
