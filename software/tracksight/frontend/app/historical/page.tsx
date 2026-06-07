"use client";

import { CalendarClock, Check, ChevronDown, HardDrive, RadioIcon } from "lucide-react";
import { useCallback, useEffect, useMemo, useState } from "react";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import DataDashboard from "@/components/DataDashboard";
import CalendarDropdown from "@/components/icons/CalendarDropdown";
import DropdownTrigger from "@/components/icons/DropdownTrigger";
import SessionDropdown from "@/components/icons/SessionDropdown";
import { DisplayControlProvider } from "@/components/PausePlayControl";
import SyncedGraphContainer, { TimeRange } from "@/components/SyncedGraphContainer";
import { Dialog, DialogContent, DialogDescription, DialogFooter, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { WidgetManager, useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { HistoricalSignalStoreProvider } from "@/lib/contexts/signalStores/HistoricalSignalStoreContext";
import { useHistoricalSessionSelection } from "@/lib/hooks/useHistoricalSessionSelection";
import { cn } from "@/lib/utils";
import AlertTimeline from "@/components/widgets/AlertTimeline";

const HISTORIC_WIDGET_STORAGE_KEY = "tracksight_historic_widgets_config_v1";
const HISTORIC_VIEWPORT_LOCK_STORAGE_KEY = "tracksight_historic_viewport_lock_state_v1";

const SOURCE_OPTIONS: { value: HistoricalSignalSource; label: string; description: string; Icon: typeof RadioIcon }[] = [
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
    const activeSource = SOURCE_OPTIONS.find((source) => source.value === selectedSource) ?? SOURCE_OPTIONS[0];
    const ActiveIcon = activeSource.Icon;

    const handleSourceSelect = (source: HistoricalSignalSource) => {
        onSourceSelect(source);
        setIsOpen(false);
    };

    return (
        <Popover open={isOpen} onOpenChange={setIsOpen}>
            <PopoverTrigger asChild>
                <DropdownTrigger className="min-w-48" open={isOpen} label="Source" value={activeSource.label} icon={<ActiveIcon className="size-5" strokeWidth={2.2} />} />
            </PopoverTrigger>

            <PopoverContent align="start" sideOffset={14} className="w-80 rounded-[1.5rem] border border-gray-200 bg-white p-3 shadow-[0_28px_70px_rgba(15,23,42,0.16)]">
                <div className="grid gap-2">
                    {SOURCE_OPTIONS.map((source) => {
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
                <AlertTimeline />
                {widgets.length === 0 ? <div className="grid h-full place-items-center text-gray-500">Select signals by adding a widget and choosing signals.</div> : <DataDashboard />}
                <WidgetAdder />
            </HistoricalSignalStoreProvider>
        </SyncedGraphContainer>
    );
}

export default function Historical() {
    const [selectedDate, setSelectedDate] = useState<Date>(new Date());
    const [selectedSource, setSelectedSource] = useState<HistoricalSignalSource>("Radio");
    const [draftDate, setDraftDate] = useState<Date>(selectedDate);
    const [selectionModalOpen, setSelectionModalOpen] = useState(false);
    const selectedDateKey = useMemo(() => toDateKey(selectedDate), [selectedDate]);
    const draftDateKey = useMemo(() => toDateKey(draftDate), [draftDate]);

    // `live` drives the dashboard; `draft` is the staged selection inside the modal.
    const live = useHistoricalSessionSelection(selectedDateKey, selectedSource);
    const draft = useHistoricalSessionSelection(draftDateKey, selectedSource);

    const selectedRange = useMemo(() => (live.selectedSession ? { min: live.selectedSession.startUtcMs, max: live.selectedSession.endUtcMs } : null), [live.selectedSession]);
    const sourceLabel = SOURCE_OPTIONS.find((source) => source.value === selectedSource)?.label ?? "Historical";

    const openSelectionModal = () => {
        setDraftDate(selectedDate);
        draft.setSelectedSessionId(live.selectedSessionId);
        setSelectionModalOpen(true);
    };

    const handleLoadData = () => {
        setSelectedDate(draftDate);
        live.setSelectedSessionId(draft.selectedSessionId);
        setSelectionModalOpen(false);
    };

    return (
        <DisplayControlProvider defaultViewportLocked={false} viewportLockStorageKey={HISTORIC_VIEWPORT_LOCK_STORAGE_KEY}>
            <div className="mt-20 h-[calc(100vh-72px)] flex flex-col overflow-hidden">
                <div className="mx-4 mb-4 flex flex-wrap items-center gap-4 shrink-0">
                    <SourceDropdown selectedSource={selectedSource} onSourceSelect={setSelectedSource} />
                    <button type="button" className="flex min-w-72 items-center justify-between gap-4 rounded-3xl border border-gray-200 bg-white px-5 py-4 text-left shadow-[0_10px_30px_rgba(15,23,42,0.10)] transition-colors hover:bg-gray-50" onClick={() => openSelectionModal()}>
                        <div className="flex items-center gap-4">
                            <div className="flex size-11 items-center justify-center rounded-2xl bg-gray-100 text-gray-900">
                                <CalendarClock className="size-6" strokeWidth={2.2} />
                            </div>
                            <div>
                                <div className="text-sm font-semibold uppercase tracking-[0.16em] text-gray-500">Date and Session</div>
                                <div className="text-[1.45rem] font-semibold leading-none text-gray-950">{live.selectedSession?.label ?? "Select session"}</div>
                            </div>
                        </div>
                        <ChevronDown className="size-6 shrink-0 text-gray-500" strokeWidth={2.25} />
                    </button>
                </div>

                <div className="mx-4 mb-3 text-sm font-medium text-gray-600 shrink-0">Showing {sourceLabel} data. All times shown in UTC.</div>

                <div className="flex-1 min-h-0 relative w-full">
                    <WidgetManager storageKey={HISTORIC_WIDGET_STORAGE_KEY}>{selectedRange ? <HistoricContent selectedRange={selectedRange} selectedSource={selectedSource} /> : <div className="mx-4 grid h-full place-items-center text-gray-500">{live.query.isPending ? "Loading sessions..." : "No historical session selected."}</div>}</WidgetManager>
                </div>
            </div>

            <Dialog open={selectionModalOpen} onOpenChange={setSelectionModalOpen}>
                <DialogContent className="max-w-6xl gap-6 rounded-[1.5rem] bg-white">
                    <DialogHeader>
                        <DialogTitle>Select Historical Data</DialogTitle>
                        <DialogDescription>Choose the date and session to load into the historical dashboard.</DialogDescription>
                    </DialogHeader>

                    <div className="grid gap-4 md:grid-cols-[minmax(16rem,1fr)_minmax(22rem,1.2fr)]">
                        <CalendarDropdown selectedDate={draftDate} onDateSelect={setDraftDate} />
                        <SessionDropdown sessions={draft.sessions} selectedSessionId={draft.selectedSessionId} isLoading={draft.query.isPending} error={draft.query.error} onSessionSelect={draft.setSelectedSessionId} />
                    </div>

                    <DialogFooter>
                        <button type="button" className="rounded-xl bg-blue-600 px-5 py-3 text-sm font-semibold text-white shadow-[0_8px_18px_rgba(37,99,235,0.28)] transition-colors hover:bg-blue-700 disabled:cursor-not-allowed disabled:opacity-60" disabled={!draft.selectedSession} onClick={handleLoadData}>
                            Load Data
                        </button>
                    </DialogFooter>
                </DialogContent>
            </Dialog>
        </DisplayControlProvider>
    );
}
