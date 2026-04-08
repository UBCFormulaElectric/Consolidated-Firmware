"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import {
  DisplayControlProvider,
  ViewportLockButton,
} from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import {
  useWidgetManager,
  WidgetManager,
} from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "false";
function Content() {
  const { initializedFromLocalStorage } = useWidgetManager();
  const DataSourceProvider = USE_MOCK_DATA
    ? MockSignalStoreProvider
    : LiveSignalStoreProvider;

  return (
    <DataSourceProvider>
      <AlertTimeline />
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
    <div id="live-page" className="pt-14 h-screen w-screen">
      <DisplayControlProvider>
        <div className="sticky z-10 mr-auto ml-2">
          <ViewportLockButton />
        </div>
        <SyncedGraphContainer>
          <WidgetManager>
            <Content />
          </WidgetManager>
        </SyncedGraphContainer>
      </DisplayControlProvider>
    </div>
  );
}
