"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import { DisplayControlProvider, PausePlayButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import DataDashboard from "@/components/DataDashboard";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "true";

function Content() {
  const { initializedFromLocalStorage } = useWidgetManager();
  const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

  return (
    <DataSourceProvider>
      <PausePlayButton />
      {initializedFromLocalStorage ? (
        <>
          <DataDashboard />
          <WidgetAdder />
        </>
      ) : (
        <div className="grid h-full place-items-center text-gray-500">
          Loading Widgets
        </div>
      )}
    </DataSourceProvider>
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
      </DisplayControlProvider>
    </div>
  );
}
