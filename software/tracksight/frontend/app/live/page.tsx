"use client";

import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import { WidgetAdder } from "@/app/live/WidgetAdder";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";
import DataDashboard from "@/components/DataDashboard";

const USE_MOCK_DATA = true;

function Content() {
  const { initializedFromLocalStorage } = useWidgetManager();

  const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

  return (
    <>
      <DataSourceProvider>
        <PausePlayButton />
        {/* <AlertTimeline /> */}
        {
          initializedFromLocalStorage ?
            <>
              <DataDashboard />
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
    </div>
  );
}
