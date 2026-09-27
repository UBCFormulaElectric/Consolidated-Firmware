"use client";

import DataDashboard from "@/components/DataDashboard";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "true";

function Content() {
    const { initializedFromLocalStorage } = useWidgetManager();
    const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

    return (
        <DataSourceProvider>
            {initializedFromLocalStorage ? (
                <>
                    <AlertTimeline />
                    <DataDashboard emptyMessage="Choose a signal to create your first chart." />
                </>
            ) : (
                <div className="grid h-full place-items-center text-gray-500">Loading Widgets</div>
            )}
        </DataSourceProvider>
    );
}

export default function LiveDataPage() {
    return (
        <div id="live-page" className="h-screen w-screen pt-16 flex flex-col overflow-hidden">
            <DisplayControlProvider>
                <div className="flex-1 min-h-0 w-full relative">
                    <SyncedGraphContainer>
                        <WidgetManager>
                            <Content />
                        </WidgetManager>
                    </SyncedGraphContainer>
                </div>
            </DisplayControlProvider>
        </div>
    );
}
