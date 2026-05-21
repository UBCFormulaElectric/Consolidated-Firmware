"use client";

import { useCallback, useEffect, useMemo, useState } from "react";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import CalendarDropdown from "@/components/icons/CalendarDropdown";
import SessionDropdown from "@/components/icons/SessionDropdown";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer, { TimeRange } from "@/components/SyncedGraphContainer";
import { WidgetManager, useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { HistoricalSignalStoreProvider } from "@/lib/contexts/signalStores/HistoricalSignalStoreContext";
import { useHistoricalSessions } from "@/lib/hooks/useHistoricalSessions";

const HISTORIC_WIDGET_STORAGE_KEY = "tracksight_historic_widgets_config_v1";
const HISTORIC_VIEWPORT_LOCK_STORAGE_KEY = "tracksight_historic_viewport_lock_state_v1";

const toDateKey = (date: Date) => {
    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, "0");
    const day = String(date.getDate()).padStart(2, "0");

    return `${year}-${month}-${day}`;
};

const expandViewportFetchRange = (range: TimeRange, bounds: TimeRange): TimeRange => {
    const width = Math.max(range.max - range.min, 1);
    const padding = width * 0.5; // + 0.5x viewport range padding on both sides

    return {
        min: Math.max(Math.floor(range.min - padding), bounds.min),
        max: Math.min(Math.ceil(range.max + padding), bounds.max),
    };
};

function HistoricContent(props: { selectedRange: { min: number; max: number } }) {
    const { selectedRange } = props;
    const { widgets } = useWidgetManager();
    const [fetchRange, setFetchRange] = useState<TimeRange>(selectedRange);

    useEffect(() => {
        setFetchRange(selectedRange);
    }, [selectedRange]);

    const handleViewportSettled = useCallback(
        (viewportRange: TimeRange) => {
            const nextFetchRange = expandViewportFetchRange(viewportRange, selectedRange);

            setFetchRange((currentFetchRange) => (currentFetchRange.min === nextFetchRange.min && currentFetchRange.max === nextFetchRange.max ? currentFetchRange : nextFetchRange));
        },
        [selectedRange]
    );

    return (
        <SyncedGraphContainer initialTimeRange={selectedRange} onViewportSettled={handleViewportSettled}>
            <HistoricalSignalStoreProvider startUtcMs={fetchRange.min} endUtcMs={fetchRange.max} selectedRange={selectedRange}>
                {widgets.length === 0 ? <div className="grid h-full place-items-center text-gray-500">Select signals by adding a widget and choosing signals.</div> : <DataDashboard />}
                <WidgetAdder />
            </HistoricalSignalStoreProvider>
        </SyncedGraphContainer>
    );
}

export default function Historical() {
    const [selectedDate, setSelectedDate] = useState<Date>(new Date());
    const [selectedSessionId, setSelectedSessionId] = useState<string | null>(null);
    const selectedDateKey = useMemo(() => toDateKey(selectedDate), [selectedDate]);
    const sessionsQuery = useHistoricalSessions(selectedDateKey);
    const sessions = useMemo(() => sessionsQuery.data ?? [], [sessionsQuery.data]);

    useEffect(() => {
        setSelectedSessionId(null);
    }, [selectedDateKey]);

    useEffect(() => {
        if (sessions.length === 0) {
            return;
        }

        setSelectedSessionId((currentSessionId) => (sessions.some((session) => session.id === currentSessionId) ? currentSessionId : sessions[0].id));
    }, [sessions]);

    const selectedSession = useMemo(() => sessions.find((session) => session.id === selectedSessionId) ?? null, [selectedSessionId, sessions]);
    const selectedRange = useMemo(() => (selectedSession ? { min: selectedSession.startUtcMs, max: selectedSession.endUtcMs } : null), [selectedSession]);

    return (
        <DisplayControlProvider defaultViewportLocked={false} viewportLockStorageKey={HISTORIC_VIEWPORT_LOCK_STORAGE_KEY}>
            <div className="mt-20 h-[calc(100vh-72px)] bg">
                <div className="mx-4 mb-4 flex flex-wrap items-center gap-4">
                    <CalendarDropdown selectedDate={selectedDate} onDateSelect={setSelectedDate} />
                    <SessionDropdown sessions={sessions} selectedSessionId={selectedSessionId} isLoading={sessionsQuery.isPending} error={sessionsQuery.error} onSessionSelect={setSelectedSessionId} />
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600">Time axis shown in UTC!! ts aint pst.</div>

                <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>{selectedRange ? <HistoricContent selectedRange={selectedRange} /> : <div className="mx-4 grid h-full place-items-center text-gray-500">{sessionsQuery.isPending ? "Loading sessions..." : "No historical session selected."}</div>}</WidgetManager>
            </div>
        </DisplayControlProvider>
    );
}
