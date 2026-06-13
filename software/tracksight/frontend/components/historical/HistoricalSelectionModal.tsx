"use client";

import { Check, ChevronLeft, ChevronRight, Clock3, Loader2 } from "lucide-react";
import { useEffect, useMemo, useState } from "react";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { HistoricalSession } from "@/lib/api/historicalSessions";
import { SOURCE_OPTIONS, SelectionStep, sourceLabelFor, useHistoricalSelection } from "@/lib/contexts/HistoricalSelectionContext";
import { useHistoricalSessionsForRange } from "@/lib/hooks/useHistoricalSessionsForRange";
import { cn } from "@/lib/utils";

const MONTH_NAMES = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
const WEEKDAY_LABELS = ["S", "M", "T", "W", "T", "F", "S"];

const createUtcDate = (year: number, month: number, day: number) => new Date(Date.UTC(year, month, day));
const startOfUtcDay = (utcMs: number) => {
    const date = new Date(utcMs);
    return createUtcDate(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate());
};
const firstOfUtcMonth = (date: Date) => createUtcDate(date.getUTCFullYear(), date.getUTCMonth(), 1);
const getUtcToday = () => startOfUtcDay(Date.now());

const toDateKey = (date: Date) => {
    const year = date.getUTCFullYear();
    const month = String(date.getUTCMonth() + 1).padStart(2, "0");
    const day = String(date.getUTCDate()).padStart(2, "0");
    return `${year}-${month}-${day}`;
};

const tabDateFormatter = new Intl.DateTimeFormat("en-US", { timeZone: "UTC", month: "short", day: "numeric" });

const getCalendarDays = (displayMonth: Date) => {
    const year = displayMonth.getUTCFullYear();
    const month = displayMonth.getUTCMonth();

    const firstDay = createUtcDate(year, month, 1);
    const firstVisibleDay = new Date(firstDay);
    firstVisibleDay.setUTCDate(firstDay.getUTCDate() - firstDay.getUTCDay());

    return Array.from({ length: 42 }, (_, index) => {
        const date = new Date(firstVisibleDay);
        date.setUTCDate(firstVisibleDay.getUTCDate() + index);
        return { date, inCurrentMonth: date.getUTCMonth() === month };
    });
};

type StepTabProps = {
    index: SelectionStep;
    label: string;
    value: string;
    isActive: boolean;
    isComplete: boolean;
    onClick: () => void;
};

function StepTab({ index, label, value, isActive, isComplete, onClick }: StepTabProps) {
    return (
        <button type="button" onClick={onClick} className={cn("flex w-full min-w-0 items-center gap-2.5 rounded border px-3 py-2 text-left transition-colors outline-none hover:cursor-pointer focus:outline-none focus-visible:outline-none", isActive ? "border-blue-500 bg-blue-100" : "border-black hover:border-black/75")}>
            <span className={cn("flex size-5 shrink-0 items-center justify-center text-sm font-bold", isActive ? "text-blue-600" : isComplete ? "text-blue-500" : "text-gray-400")}>{isComplete && !isActive ? <Check className="size-4" strokeWidth={3} /> : index}</span>
            <span className="min-w-0">
                <span className="block text-[0.65rem] font-semibold uppercase tracking-wide text-gray-500">{label}</span>
                <span className={cn("block truncate text-sm font-semibold", isActive ? "text-blue-600" : "text-gray-800")}>{value}</span>
            </span>
        </button>
    );
}

function SourceStep({ value, onSelect }: { value: HistoricalSignalSource; onSelect: (source: HistoricalSignalSource) => void }) {
    return (
        <div className="grid gap-3">
            {SOURCE_OPTIONS.map((option) => {
                const isSelected = option.value === value;
                const Icon = option.Icon;
                return (
                    <button key={option.value} type="button" onClick={() => onSelect(option.value)} className={cn("flex items-center gap-4 rounded border px-4 py-4 text-left transition-colors hover:cursor-pointer", isSelected ? "border-blue-500 bg-blue-100 text-blue-500" : "border-black hover:border-black/75")}>
                        <Icon className={cn("size-7 shrink-0", isSelected ? "text-blue-500" : "opacity-75")} />
                        <span className="flex flex-col leading-5">
                            <span className="text-base font-semibold">{option.label}</span>
                            <span className="text-xs text-gray-500">{option.description}</span>
                        </span>
                        {isSelected ? <Check className="ml-auto size-5 shrink-0 text-blue-500" strokeWidth={2.5} /> : null}
                    </button>
                );
            })}
        </div>
    );
}

type DateStepProps = {
    displayMonth: Date;
    onDisplayMonthChange: (date: Date) => void;
    selectedDate: Date | null;
    onDateSelect: (date: Date) => void;
    daysWithSessions: Set<string>;
    isLoading: boolean;
};

function DateStep({ displayMonth, onDisplayMonthChange, selectedDate, onDateSelect, daysWithSessions, isLoading }: DateStepProps) {
    const calendarDays = useMemo(() => getCalendarDays(displayMonth), [displayMonth]);
    const selectedKey = selectedDate ? toDateKey(selectedDate) : null;

    return (
        <div className="overflow-hidden rounded border border-black">
            <div className="flex items-center justify-between gap-3 border-b border-black bg-gray-100 px-3 py-2">
                <button type="button" className="flex size-8 items-center justify-center rounded border border-black bg-white text-gray-700 transition-colors hover:cursor-pointer hover:border-black/75" onClick={() => onDisplayMonthChange(createUtcDate(displayMonth.getUTCFullYear(), displayMonth.getUTCMonth() - 1, 1))}>
                    <ChevronLeft className="size-5" />
                </button>
                <span className="flex items-center gap-2 text-base font-bold text-gray-900">
                    {MONTH_NAMES[displayMonth.getUTCMonth()]} {displayMonth.getUTCFullYear()}
                    {isLoading ? <Loader2 className="size-4 animate-spin text-blue-500" /> : null}
                </span>
                <button type="button" className="flex size-8 items-center justify-center rounded border border-black bg-white text-gray-700 transition-colors hover:cursor-pointer hover:border-black/75" onClick={() => onDisplayMonthChange(createUtcDate(displayMonth.getUTCFullYear(), displayMonth.getUTCMonth() + 1, 1))}>
                    <ChevronRight className="size-5" />
                </button>
            </div>

            <div className="grid grid-cols-7 gap-y-1 px-3 py-3">
                {WEEKDAY_LABELS.map((label, index) => (
                    <div key={`${label}-${index}`} className="pb-1 text-center text-xs font-semibold uppercase tracking-wide text-gray-400">
                        {label}
                    </div>
                ))}

                {calendarDays.map(({ date, inCurrentMonth }) => {
                    const key = toDateKey(date);
                    const isSelected = key === selectedKey;
                    const hasSessions = daysWithSessions.has(key);

                    return (
                        <button key={date.toISOString()} type="button" disabled={!hasSessions} onClick={() => onDateSelect(date)} className={cn("relative mx-auto flex size-10 flex-col items-center justify-center rounded text-base font-semibold transition-colors", isSelected ? "bg-blue-100 text-blue-500" : hasSessions ? "text-gray-800 hover:cursor-pointer hover:bg-blue-100/50" : inCurrentMonth ? "cursor-not-allowed text-gray-400" : "cursor-not-allowed text-gray-300")}>
                            <span>{date.getUTCDate()}</span>
                            <span className={cn("absolute bottom-1 size-1.5 rounded-full", hasSessions ? "bg-blue-500" : "bg-transparent")} />
                        </button>
                    );
                })}
            </div>
        </div>
    );
}

type SessionStepProps = {
    sessions: HistoricalSession[];
    selectedSessionId: string | null;
    onSessionSelect: (sessionId: string) => void;
    isLoading: boolean;
    error: Error | null;
};

function SessionStep({ sessions, selectedSessionId, onSessionSelect, isLoading, error }: SessionStepProps) {
    if (isLoading) {
        return (
            <div className="flex h-40 items-center justify-center gap-2 rounded border border-black text-sm text-gray-500">
                <Loader2 className="size-4 animate-spin" />
                Loading sessions...
            </div>
        );
    }

    if (error) {
        return <div className="flex h-40 items-center justify-center rounded border border-red-500 bg-red-100 text-sm text-red-600">Failed to load sessions.</div>;
    }

    if (sessions.length === 0) {
        return <div className="flex h-40 items-center justify-center rounded border border-black text-sm text-gray-400">No sessions recorded on this day.</div>;
    }

    return (
        <div className="max-h-72 overflow-y-auto rounded border border-black">
            {sessions.toReversed().map((session) => {
                const isSelected = session.id === selectedSessionId;

                return (
                    <button key={session.id} type="button" onClick={() => onSessionSelect(session.id)} className={cn("flex w-full items-center gap-3 border-b border-black px-4 py-3 text-left transition-colors last:border-b-0 hover:cursor-pointer", isSelected ? "bg-blue-100 text-blue-500" : "text-gray-800 hover:bg-blue-100/50")}>
                        <Clock3 className={cn("size-5 shrink-0", isSelected ? "text-blue-500" : "opacity-75")} />
                        <span className="text-sm font-semibold">{session.label}</span>

                        {isSelected ? <Check className="ml-auto size-5 shrink-0 text-blue-500" strokeWidth={2.5} /> : null}
                    </button>
                );
            })}
        </div>
    );
}

export default function HistoricalSelectionModal() {
    const { isModalOpen, setModalOpen, initialStep, source, selectedSession, applySelection } = useHistoricalSelection();

    const [step, setStep] = useState<SelectionStep>(1);
    const [draftSource, setDraftSource] = useState<HistoricalSignalSource>(source);
    const [draftDate, setDraftDate] = useState<Date | null>(null);
    const [displayMonth, setDisplayMonth] = useState<Date>(() => firstOfUtcMonth(getUtcToday()));
    const [draftSessionId, setDraftSessionId] = useState<string | null>(null);

    useEffect(() => {
        if (!isModalOpen) return;

        const committedDate = selectedSession ? startOfUtcDay(selectedSession.startUtcMs) : null;

        setDraftSource(source);
        setDraftDate(committedDate);
        setDraftSessionId(selectedSession?.id ?? null);

        setDisplayMonth(firstOfUtcMonth(committedDate ?? getUtcToday()));


        setStep(initialStep);
    }, [isModalOpen, initialStep]);

    const visibleRange = useMemo(() => {
        const days = getCalendarDays(displayMonth);
        const startUtcMs = days[0].date.getTime();
        const endUtcMs = days[days.length - 1].date.getTime() + 24 * 60 * 60 * 1000;

        return { startUtcMs, endUtcMs };
    }, [displayMonth]);

    const { query, sessionsByDay, daysWithSessions } = useHistoricalSessionsForRange(visibleRange.startUtcMs, visibleRange.endUtcMs, draftSource, isModalOpen);

    const draftDayKey = draftDate ? toDateKey(draftDate) : null;
    const draftSessions = useMemo(() => (draftDayKey ? (sessionsByDay.get(draftDayKey) ?? []) : []), [sessionsByDay, draftDayKey]);
    const draftSession = useMemo(() => draftSessions.find((session) => session.id === draftSessionId) ?? null, [draftSessions, draftSessionId]);

    const handleSourceSelect = (nextSource: HistoricalSignalSource) => {
        setDraftSource(nextSource);
        setDraftSessionId(null);

        setStep(2);
    };

    const handleDateSelect = (date: Date) => {
        setDraftDate(date);
        setDraftSessionId(null);

        setStep(3);
    };

    const handleLoadData = () => {
        if (!draftSession) return;

        applySelection(draftSource, draftSession);
    };

    return (
        <Dialog open={isModalOpen} onOpenChange={setModalOpen}>
            <DialogContent className="max-w-2xl gap-5 rounded border-black bg-white shadow-none sm:rounded">
                <DialogHeader>
                    <DialogTitle className="text-2xl font-bold tracking-tight">Select Historical Data</DialogTitle>
                    <DialogDescription className="text-sm text-gray-500">Pick a source, date, and session to load into the dashboard. All times shown in UTC.</DialogDescription>
                </DialogHeader>

                <div className="grid grid-cols-3 gap-2">
                    <StepTab index={1} label="Source" value={sourceLabelFor(draftSource)} isActive={step === 1} isComplete onClick={() => setStep(1)} />
                    <StepTab index={2} label="Date" value={draftDate ? tabDateFormatter.format(draftDate) : "—"} isActive={step === 2} isComplete={Boolean(draftDate)} onClick={() => setStep(2)} />
                    <StepTab index={3} label="Session" value={draftSession?.label ?? "—"} isActive={step === 3} isComplete={Boolean(draftSession)} onClick={() => setStep(3)} />
                </div>

                <div>
                    {step === 1 ? <SourceStep value={draftSource} onSelect={handleSourceSelect} /> : null}
                    {step === 2 ? <DateStep displayMonth={displayMonth} onDisplayMonthChange={setDisplayMonth} selectedDate={draftDate} onDateSelect={handleDateSelect} daysWithSessions={daysWithSessions} isLoading={query.isPending} /> : null}
                    {step === 3 ? <SessionStep sessions={draftSessions} selectedSessionId={draftSessionId} onSessionSelect={setDraftSessionId} isLoading={query.isPending} error={query.error} /> : null}
                </div>

                <div className="flex justify-end">
                    <button type="button" className="rounded border border-blue-500 bg-blue-200/50 px-5 py-3 text-sm font-semibold text-gray-900 transition-colors hover:cursor-pointer hover:bg-blue-200/70 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-100/50 disabled:text-gray-600" disabled={!draftSession} onClick={handleLoadData}>
                        Load Data
                    </button>
                </div>
            </DialogContent>
        </Dialog>
    );
}
