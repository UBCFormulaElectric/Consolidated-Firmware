"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";

function Content() {
  const { widgets, initializedFromLocalStorage } = useWidgetManager();

  return (
    <>
      <PausePlayButton />
      {
        initializedFromLocalStorage ?
          <>
            {widgets.map(widgetData => // TODO(evan): Add the universal timeline
              <Widget widgetData={widgetData} key={widgetData.id} />
            )}
            <WidgetAdder />
          </>
          :
          <div className='grid place-items-center h-full text-gray-500'>
            Loading Widgets
          </div>
      }
    </>
  );
}

export default function LiveDataPage() {
  return (
    <div id="live-page" className="pt-14 h-screen">
      <DisplayControlProvider>
        {/* <SignalProvider> */}
        <SyncedGraphContainer>
          <WidgetManager>
            <Content />
          </WidgetManager>
        </SyncedGraphContainer>
        {/* </SignalProvider > */}
      </DisplayControlProvider >
    </div>
  );
}
