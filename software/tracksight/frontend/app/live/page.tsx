"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { SignalProvider } from "@/lib/contexts/SignalContext";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { Widget } from "@/components/widgets/Widget";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { SignalType } from "@/lib/types/Signal";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = true;

function Content() {
  const { widgets, initializedFromLocalStorage } = useWidgetManager();

  const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

  return (
    <>
      <DataSourceProvider>
        <PausePlayButton />
        {
          initializedFromLocalStorage ?
            <>
              {widgets.map(widgetData => {
                if (widgetData.type === SignalType.NUMERICAL) {
                  return (
                    <Widget key={widgetData.id} {...widgetData} type="numericalGraph" />
                  );
                } else if (widgetData.type === SignalType.ENUM) {
                  return (
                    <Widget key={widgetData.id} {...widgetData} type="enumTimeline" />
                  );
                }
              })}
              <WidgetAdder />
            </>
            :
            <div className='grid place-items-center h-full text-gray-500'>
              Loading Widgets
            </div>
        }
      </DataSourceProvider>
    </>
  );
}

export default function LiveDataPage() {

  return (
    <div id="live-page" className="pt-14 h-screen">
      <DisplayControlProvider>
        <SyncedGraphContainer>
          <WidgetManager>
            <Content />
          </WidgetManager>
        </SyncedGraphContainer>
      </DisplayControlProvider >
    </div >
  );
}
