"use client";

import DataDashboard from "@/components/DataDashboard";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer from "@/components/SyncedGraphContainer";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import { useWidgetManager, WidgetManager } from "@/components/widgets/WidgetManagerContext";
import { LiveSignalStoreProvider } from "@/lib/contexts/signalStores/LiveSignalStoreContext";
import { MockSignalStoreProvider } from "@/lib/contexts/signalStores/MockSignalStoreContext";
import { getMockSignalCatalog } from "@/lib/mock/catalog";
import { createMockCharts } from "@/lib/mock/charts";
import { readMockParam } from "@/lib/mock/config";
import { useEffect } from "react";
import { DndProvider } from "react-dnd";
import { HTML5Backend } from "react-dnd-html5-backend";

const USE_MOCK_DATA = process.env.NEXT_PUBLIC_USE_MOCK_DATA === "true";

function Content() {
    const { initializedFromLocalStorage, replaceWidgets } = useWidgetManager();
    const DataSourceProvider = USE_MOCK_DATA ? MockSignalStoreProvider : LiveSignalStoreProvider;

    useEffect(() => {
        // ?mockCharts=N swaps in N random charts for stress testing, then drops the param so a reload keeps them
        if (!USE_MOCK_DATA || !initializedFromLocalStorage) return;

        const count = readMockParam("mockCharts");
        if (!count) return;

        replaceWidgets(createMockCharts(count, getMockSignalCatalog()));
        const url = new URL(window.location.href);
        url.searchParams.delete("mockCharts");
        window.history.replaceState(null, "", url);
    }, [initializedFromLocalStorage, replaceWidgets]);

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
            <DndProvider backend={HTML5Backend}>
                <DisplayControlProvider>
                    <div className="flex-1 min-h-0 w-full relative">
                        <SyncedGraphContainer>
                            <WidgetManager>
                                <Content />
                            </WidgetManager>
                        </SyncedGraphContainer>
                    </div>
                </DisplayControlProvider>
            </DndProvider>
        </div>
    );
}
