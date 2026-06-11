"use client";

import { useCallback, useEffect, useRef, useState } from "react";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import HistoricalSelectionModal from "@/components/historical/HistoricalSelectionModal";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer, { TimeRange } from "@/components/SyncedGraphContainer";
import { WidgetManager, useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import AlertTimeline from "@/components/widgets/AlertTimeline";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { useHistoricalSelection } from "@/lib/contexts/HistoricalSelectionContext";
import { HistoricalSignalStoreProvider } from "@/lib/contexts/signalStores/HistoricalSignalStoreContext";

const HISTORIC_WIDGET_STORAGE_KEY = "tracksight_historic_widgets_config_v1";
const HISTORIC_VIEWPORT_LOCK_STORAGE_KEY = "tracksight_historic_viewport_lock_state_v1";

const expandViewportFetchRange = (range: TimeRange, bounds: TimeRange): TimeRange => {
    const width = Math.max(range.max - range.min, 1);
    const padding = width * 0.5; // + 0.5x viewport range padding on both sides

    return {
        min: Math.max(Math.floor(range.min - padding), bounds.min),
        max: Math.min(Math.ceil(range.max + padding), bounds.max),
    };
};

function HistoricContent(props: { selectedRange: { min: number; max: number }; selectedSource: HistoricalSignalSource }) {
    const { selectedRange, selectedSource } = props;
    const { widgets } = useWidgetManager();
    const [fetchRange, setFetchRange] = useState<TimeRange>(selectedRange);

    useEffect(() => {
        setFetchRange(selectedRange);
    }, [selectedRange, selectedSource]);

    const handleViewportSettled = useCallback(
        (viewportRange: TimeRange) => {
            const nextFetchRange = expandViewportFetchRange(viewportRange, selectedRange);

            setFetchRange((currentFetchRange) => (currentFetchRange.min === nextFetchRange.min && currentFetchRange.max === nextFetchRange.max ? currentFetchRange : nextFetchRange));
        },
        [selectedRange]
    );

    return (
        <SyncedGraphContainer initialTimeRange={selectedRange} onViewportSettled={handleViewportSettled}>
            <HistoricalSignalStoreProvider startUtcMs={fetchRange.min} endUtcMs={fetchRange.max} source={selectedSource} selectedRange={selectedRange}>
                <AlertTimeline />
                {widgets.length === 0 ? <div className="grid h-full place-items-center text-gray-500">Select signals by adding a widget and choosing signals.</div> : <DataDashboard />}
                <WidgetAdder />
            </HistoricalSignalStoreProvider>
        </SyncedGraphContainer>
    );
}

export default function Historical() {
    const { source, selectedRange, selectedSession, isModalOpen, openModal } = useHistoricalSelection();

    // Auto-open the modal once when landing with nothing selected yet.
    const hasAutoOpenedRef = useRef(false);
    useEffect(() => {
        if (!hasAutoOpenedRef.current && !selectedSession && !isModalOpen) {
            hasAutoOpenedRef.current = true;
            openModal(1);
        }
    }, [selectedSession, isModalOpen, openModal]);

    return (
        <DisplayControlProvider defaultViewportLocked={false} viewportLockStorageKey={HISTORIC_VIEWPORT_LOCK_STORAGE_KEY}>
            <div className="mt-20 flex h-[calc(100vh-72px)] flex-col overflow-hidden">
                <div className="relative min-h-0 w-full flex-1">
                    <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>
                        {selectedRange ? (
                            <HistoricContent selectedRange={selectedRange} selectedSource={source} />
                        ) : (
                            <div className="mx-4 grid h-full place-items-center text-gray-500">No historical session selected.</div>
                        )}
                    </WidgetManager>
                </div>
            </div>

            <HistoricalSelectionModal />
        </DisplayControlProvider>
    );
}
