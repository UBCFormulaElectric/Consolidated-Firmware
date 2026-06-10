"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import { DisplayControlProvider, ViewportLockButton } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";
import { DiagnosticOverlay } from "@/components/widgets/DiagnosticOverlay";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "true";

function Content() {
    const { initializedFromLocalStorage } = useWidgetManager();
    const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

    return (
        <DataSourceProvider>
            {initializedFromLocalStorage ? (
                <>
                    <AlertTimeline />
                    <DataDashboard />
                    <div className="flex flex-col py-8 items-center gap-4">
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
        <div id="live-page" className="pt-14 h-[calc(100vh-3.5rem)] w-screen flex flex-col overflow-hidden">
            <DiagnosticOverlay />
            <DisplayControlProvider>
                <div className="fixed top-14 left-0 z-60 flex w-screen -translate-y-1/2 justify-center pointer-events-none">
                    <div className="pointer-events-auto relative rounded-full bg-white p-2 before:pointer-events-none before:absolute before:inset-0 before:rounded-full before:border before:border-gray-200 before:[clip-path:inset(47%_-2px_-2px_-2px)] before:content-['']">
                        <ViewportLockButton />
                    </div>
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
