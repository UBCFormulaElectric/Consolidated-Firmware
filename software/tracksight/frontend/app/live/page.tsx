"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import { DisplayControlProvider, ViewportLockButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "true";

function Content() {
    const { initializedFromLocalStorage, widgets } = useWidgetManager();
    const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

    return (
        <DataSourceProvider>
            {initializedFromLocalStorage ? (
                <>
                    <AlertTimeline />
                    <DataDashboard />
                    <div className="flex flex-col py-8 items-center gap-4">
                        {widgets.length === 0 && <p className="text-sm text-gray-500">Choose a signal to create your first chart.</p>}
                        <WidgetAdder />
                    </div>
                </>
            ) : (
                <div className="grid h-full place-items-center text-gray-500">Loading Widgets</div>
            )}
        </DataSourceProvider>
    );
}

export default function LiveDataPage() {
    return (
        <div id="live-page" className="h-screen w-screen pt-14 flex flex-col overflow-hidden">
            <DisplayControlProvider>
                <div className="flex items-center justify-between border-b border-gray-200 px-4 py-2">
                    <span className="text-xs text-gray-500">Ctrl + scroll to zoom</span>
                    <ViewportLockButton />
                </div>
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
