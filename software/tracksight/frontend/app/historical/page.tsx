"use client";

import { CalendarClock, Check, ChevronDown, HardDrive, RadioIcon } from "lucide-react";
import { useCallback, useEffect, useMemo, useState } from "react";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import CalendarDropdown from "@/components/icons/CalendarDropdown";
import SessionDropdown from "@/components/icons/SessionDropdown";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer, { TimeRange } from "@/components/SyncedGraphContainer";
import { Dialog, DialogContent, DialogDescription, DialogFooter, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { WidgetManager, useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { HistoricalSignalStoreProvider } from "@/lib/contexts/signalStores/HistoricalSignalStoreContext";
import { useHistoricalSessions } from "@/lib/hooks/useHistoricalSessions";
import { cn } from "@/lib/utils";

const HISTORIC_WIDGET_STORAGE_KEY = "tracksight_historic_widgets_config_v1";
const HISTORIC_VIEWPORT_LOCK_STORAGE_KEY = "tracksight_historic_viewport_lock_state_v1";

const _sourceOptions: { value: HistoricalSignalSource; label: string; description: string; Icon: typeof RadioIcon }[] = [
    {
        value: "Radio",
        label: "Radio",
        description: "Radio data",
        Icon: RadioIcon,
    },
    {
        value: "SdCard",
        label: "SD Card",
        description: "Dumped logger data",
        Icon: HardDrive,
    },
];

const toDateKey = (date: Date) => {
    const year = date.getUTCFullYear();
    const month = String(date.getUTCMonth() + 1).padStart(2, "0");
    const day = String(date.getUTCDate()).padStart(2, "0");

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

function SourceDropdown(props: { selectedSource: HistoricalSignalSource; onSourceSelect: (source: HistoricalSignalSource) => void }) {
    const { selectedSource, onSourceSelect } = props;
    const [isOpen, setIsOpen] = useState(false);
    const activeSource = _sourceOptions.find((source) => source.value === selectedSource) ?? _sourceOptions[0];
    const ActiveIcon = activeSource.Icon;

    const handleSourceSelect = (source: HistoricalSignalSource) => {
        onSourceSelect(source);
        setIsOpen(false);
    };

    return (
        <Popover open={isOpen} onOpenChange={setIsOpen}>
            <PopoverTrigger asChild>
                <button type="button" className="flex min-w-48 items-center justify-between gap-3 rounded-2xl border border-gray-200 bg-white px-4 py-3 text-left shadow-[0_12px_34px_rgba(15,23,42,0.12)] transition-colors hover:bg-gray-50">
                    <div className="flex min-w-0 items-center gap-3">
                        <div className="flex size-10 shrink-0 items-center justify-center rounded-xl bg-gray-100 text-gray-900">
                            <ActiveIcon className="size-5" strokeWidth={2.2} />
                        </div>
                        <div className="min-w-0">
                            <div className="text-sm font-semibold uppercase tracking-[0.16em] text-gray-500">Source</div>
                            <div className="truncate text-xl font-semibold leading-none text-gray-950">{activeSource.label}</div>
                        </div>
                    </div>
                    <ChevronDown className={cn("size-6 shrink-0 text-gray-500 transition-transform", isOpen && "rotate-180")} strokeWidth={2.25} />
                </button>
            </PopoverTrigger>

            <PopoverContent align="start" sideOffset={14} className="w-80 rounded-[1.5rem] border border-gray-200 bg-white p-3 shadow-[0_28px_70px_rgba(15,23,42,0.16)]">
                <div className="grid gap-2">
                    {_sourceOptions.map((source) => {
                        const isSelected = source.value === selectedSource;
                        const Icon = source.Icon;

                        return (
                            <button key={source.value} type="button" className={cn("flex items-center justify-between rounded-xl px-4 py-3 text-left transition-colors", isSelected && "bg-blue-600 text-white shadow-[0_8px_18px_rgba(37,99,235,0.28)]", !isSelected && "text-gray-800 hover:bg-gray-100")} onClick={() => handleSourceSelect(source.value)}>
                                <span className="flex items-center gap-3">
                                    <Icon className="size-5" />
                                    <span>
                                        <span className="block text-base font-semibold">{source.label}</span>
                                        <span className={cn("block text-xs font-semibold uppercase tracking-[0.16em]", isSelected ? "text-blue-100" : "text-gray-400")}>{source.description}</span>
                                    </span>
                                </span>
                                {isSelected ? <Check className="size-5" /> : null}
                            </button>
                        );
                    })}
                </div>
            </PopoverContent>
        </Popover>
    );
}

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
                {widgets.length === 0 ? <div className="grid h-full place-items-center text-gray-500">Select signals by adding a widget and choosing signals.</div> : <DataDashboard />}
                <WidgetAdder />
            </HistoricalSignalStoreProvider>
        </SyncedGraphContainer>
    );
}

export default function Historical() {
    const [selectedDate, setSelectedDate] = useState<Date>(new Date());
    const [selectedSessionId, setSelectedSessionId] = useState<string | null>(null);
    const [selectedSource, setSelectedSource] = useState<HistoricalSignalSource>("Radio");
    const [draftDate, setDraftDate] = useState<Date>(selectedDate);
    const [draftSessionId, setDraftSessionId] = useState<string | null>(selectedSessionId);
    const [draftSource, setDraftSource] = useState<HistoricalSignalSource>(selectedSource);
    const [selectionModalOpen, setSelectionModalOpen] = useState(false);
    const selectedDateKey = useMemo(() => toDateKey(selectedDate), [selectedDate]);
    const draftDateKey = useMemo(() => toDateKey(draftDate), [draftDate]);
    const sessionsQuery = useHistoricalSessions(selectedDateKey);
    const draftSessionsQuery = useHistoricalSessions(draftDateKey);
    const sessions = useMemo(() => sessionsQuery.data ?? [], [sessionsQuery.data]);
    const draftSessions = useMemo(() => draftSessionsQuery.data ?? [], [draftSessionsQuery.data]);

    useEffect(() => {
        setDraftSessionId(null);
    }, [draftDateKey]);

    useEffect(() => {
        if (sessions.length === 0) {
            return;
        }

        setSelectedSessionId((currentSessionId) => (sessions.some((session) => session.id === currentSessionId) ? currentSessionId : sessions[0].id));
    }, [sessions]);

    useEffect(() => {
        if (draftSessions.length === 0) {
            return;
        }

        setDraftSessionId((currentSessionId) => (draftSessions.some((session) => session.id === currentSessionId) ? currentSessionId : draftSessions[0].id));
    }, [draftSessions]);

    const selectedSession = useMemo(() => sessions.find((session) => session.id === selectedSessionId) ?? null, [selectedSessionId, sessions]);
    const selectedRange = useMemo(() => (selectedSession ? { min: selectedSession.startUtcMs, max: selectedSession.endUtcMs } : null), [selectedSession]);
    const sourceLabel = _sourceOptions.find((source) => source.value === selectedSource)?.label ?? "Historical";
    const draftSourceLabel = _sourceOptions.find((source) => source.value === draftSource)?.label ?? "Historical";
    const draftSelectedSession = useMemo(() => draftSessions.find((session) => session.id === draftSessionId) ?? null, [draftSessionId, draftSessions]);

    const openSelectionModal = useCallback(
        (nextSource = selectedSource) => {
            setDraftSource(nextSource);
            setDraftDate(selectedDate);
            setDraftSessionId(selectedSessionId);
            setSelectionModalOpen(true);
        },
        [selectedDate, selectedSessionId, selectedSource]
    );

    const handleSourceSelect = (source: HistoricalSignalSource) => {
        openSelectionModal(source);
    };

    const handleLoadData = () => {
        setSelectedSource(draftSource);
        setSelectedDate(draftDate);
        setSelectedSessionId(draftSessionId);
        setSelectionModalOpen(false);
    };

    return (
        <DisplayControlProvider defaultViewportLocked={false} viewportLockStorageKey={HISTORIC_VIEWPORT_LOCK_STORAGE_KEY}>
            <div className="mt-20 h-[calc(100vh-72px)] bg flex flex-col overflow-hidden">
                <div className="mx-4 mb-4 flex flex-wrap items-center gap-4 shrink-0">
                    <SourceDropdown selectedSource={selectedSource} onSourceSelect={handleSourceSelect} />
                    <button type="button" className="flex min-w-72 items-center justify-between gap-4 rounded-3xl border border-gray-200 bg-white px-5 py-4 text-left shadow-[0_10px_30px_rgba(15,23,42,0.10)] transition-colors hover:bg-gray-50" onClick={() => openSelectionModal()}>
                        <div className="flex items-center gap-4">
                            <div className="flex size-11 items-center justify-center rounded-2xl bg-gray-100 text-gray-900">
                                <CalendarClock className="size-6" strokeWidth={2.2} />
                            </div>
                            <div>
                                <div className="text-sm font-semibold uppercase tracking-[0.16em] text-gray-500">Date and Session</div>
                                <div className="text-[1.45rem] font-semibold leading-none text-gray-950">{selectedSession?.label ?? "Select session"}</div>
                            </div>
                        </div>
                        <ChevronDown className="size-6 shrink-0 text-gray-500" strokeWidth={2.25} />
                    </button>
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600 shrink-0">Showing {sourceLabel} data. All times shown in UTC.</div>

                <div className="flex-1 min-h-0 relative w-full">
                    <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>{selectedRange ? <HistoricContent selectedRange={selectedRange} selectedSource={selectedSource} /> : <div className="mx-4 grid h-full place-items-center text-gray-500">{sessionsQuery.isPending ? "Loading sessions..." : "No historical session selected."}</div>}</WidgetManager>
                </div>
            </div>

            <Dialog open={selectionModalOpen} onOpenChange={setSelectionModalOpen}>
                <DialogContent className="max-w-6xl gap-6 rounded-[1.5rem] bg-white">
                    <DialogHeader>
                        <DialogTitle>Select {draftSourceLabel} Data</DialogTitle>
                        <DialogDescription>Choose the source, date, and session to load into the historical dashboard.</DialogDescription>
                    </DialogHeader>

                    <div className="grid gap-4 md:grid-cols-[12rem_minmax(16rem,1fr)] lg:grid-cols-[12rem_minmax(16rem,1fr)_minmax(22rem,1.2fr)]">
                        <SourceDropdown selectedSource={draftSource} onSourceSelect={setDraftSource} />
                        <CalendarDropdown selectedDate={draftDate} onDateSelect={setDraftDate} />
                        <SessionDropdown sessions={draftSessions} selectedSessionId={draftSessionId} isLoading={draftSessionsQuery.isPending} error={draftSessionsQuery.error} onSessionSelect={setDraftSessionId} />
                    </div>

                    <DialogFooter>
                        <button type="button" className="rounded-xl bg-blue-600 px-5 py-3 text-sm font-semibold text-white shadow-[0_8px_18px_rgba(37,99,235,0.28)] transition-colors hover:bg-blue-700 disabled:cursor-not-allowed disabled:opacity-60" disabled={!draftSelectedSession} onClick={handleLoadData}>
                            Load Data
                        </button>
                    </DialogFooter>
                </DialogContent>
            </Dialog>
        </DisplayControlProvider>
    );
}
