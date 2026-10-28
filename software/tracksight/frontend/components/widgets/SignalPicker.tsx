"use client";

import { Check } from "lucide-react";
import { KeyboardEvent, ReactNode, useDeferredValue, useEffect, useMemo, useRef, useState } from "react";

import { alertNodeColor } from "@/lib/alerts";
import { useAvailableSignals } from "@/lib/hooks/useAvailableSignals";
import { findMatchRanges, prepareSignalSearch, searchSignals } from "@/lib/signalSearch";
import { BooleanSignalMetadata, EnumSignalMetadata, isBooleanSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { cn } from "@/lib/utils";

const MAX_RENDERED_SIGNALS = 100;
// same cap as #2157's select all, so one search can't flood a chart
const SELECT_ALL_LIMIT = 25;

export type ChartableSignalMetadata = NumericalSignalMetadata | EnumSignalMetadata | BooleanSignalMetadata;

// keep these at module level: the picker memoises its search index on `accept`
export const isChartableSignal = (signal: SignalMetadata): signal is ChartableSignalMetadata => isNumericalSignalMetadata(signal) || isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal);
export const isStateSignal = (signal: SignalMetadata): signal is EnumSignalMetadata | BooleanSignalMetadata => isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal);

function describeSignal(signal: SignalMetadata): string {
    if (isNumericalSignalMetadata(signal)) return signal.unit || "Graph";
    if (isEnumSignalMetadata(signal)) return signal.enum_signal.enum_name;
    if (isBooleanSignalMetadata(signal)) return "On / off";
    return signal.type;
}

// bolds the typed words and lets long names wrap after each underscore instead of being cut off
function HighlightedName(props: { name: string; query: string }) {
    const { name, query } = props;
    const parts: ReactNode[] = [];
    let cursor = 0;
    const pushText = (text: string, bold: boolean) => {
        text.split(/(?<=_)/).forEach((piece, index) => {
            if (index > 0) parts.push(<wbr key={`w${parts.length}`} />);
            parts.push(
                bold ? (
                    <mark key={parts.length} className="rounded-sm bg-yellow-200 text-gray-900">
                        {piece}
                    </mark>
                ) : (
                    piece
                )
            );
        });
    };
    for (const [start, end] of findMatchRanges(name, query)) {
        if (start > cursor) pushText(name.slice(cursor, start), false);
        pushText(name.slice(start, end), true);
        cursor = end;
    }
    if (cursor < name.length) pushText(name.slice(cursor), false);
    return <>{parts}</>;
}

export function SignalPicker<T extends SignalMetadata>(props: {
    /** which signals can be picked */
    accept: (signal: SignalMetadata) => signal is T;
    /** called once with every checked signal, in the order they were checked */
    onAdd: (signals: T[]) => void;
    /** already on the chart: listed, but can't be checked */
    addedSignalNames?: string[];
}) {
    const { accept, onAdd, addedSignalNames = [] } = props;
    const [query, setQuery] = useState("");
    const [highlightedIndex, setHighlightedIndex] = useState(0);
    // kept across searches, so signals found by different searches can be added together
    const [selected, setSelected] = useState<T[]>([]);
    const listRef = useRef<HTMLDivElement>(null);
    const { data: allSignals, isPending, isError, refetch } = useAvailableSignals();
    const deferredQuery = useDeferredValue(query);

    const searchable = useMemo(() => prepareSignalSearch((allSignals ?? []).filter(accept)), [allSignals, accept]);
    const matches = useMemo(() => searchSignals(searchable, deferredQuery), [searchable, deferredQuery]);
    const visible = matches.slice(0, MAX_RENDERED_SIGNALS);
    const selectedNames = useMemo(() => new Set(selected.map((signal) => signal.name)), [selected]);

    useEffect(() => setHighlightedIndex(0), [deferredQuery]);

    useEffect(() => {
        listRef.current?.querySelector(`[data-index="${highlightedIndex}"]`)?.scrollIntoView({ block: "nearest" });
    }, [highlightedIndex]);

    const toggle = (signal: T | undefined) => {
        if (!signal || addedSignalNames.includes(signal.name)) return;
        setSelected((prev) => (prev.some((existing) => existing.name === signal.name) ? prev.filter((existing) => existing.name !== signal.name) : [...prev, signal]));
    };
    const unselectedMatches = matches.filter((signal) => !addedSignalNames.includes(signal.name) && !selectedNames.has(signal.name));
    const canSelectAll = deferredQuery.trim() !== "" && unselectedMatches.length > 1 && unselectedMatches.length <= SELECT_ALL_LIMIT;
    const confirm = () => {
        if (selected.length > 0) onAdd(selected);
    };

    const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>) => {
        if (event.key === "Enter" && (event.metaKey || event.ctrlKey)) {
            event.preventDefault();
            confirm();
            return;
        }
        if (visible.length === 0) return;
        if (event.key === "ArrowDown" || event.key === "ArrowUp") {
            event.preventDefault();
            const step = event.key === "ArrowDown" ? 1 : -1;
            setHighlightedIndex((index) => (index + step + visible.length) % visible.length);
        } else if (event.key === "Enter") {
            event.preventDefault();
            toggle(visible[highlightedIndex]);
        }
    };

    return (
        <div className="flex min-h-0 flex-col gap-3">
            <input type="search" aria-label="Search signals" aria-controls="signal-picker-results" aria-activedescendant={visible[highlightedIndex] ? `signal-option-${highlightedIndex}` : undefined} role="combobox" aria-expanded value={query} onChange={(event) => setQuery(event.target.value)} onKeyDown={handleKeyDown} autoFocus autoComplete="off" spellCheck={false} placeholder="Search by signal, message, node, or unit" className="h-14 w-full rounded-lg border-2 border-gray-400 bg-white px-4 text-lg text-gray-900 placeholder:text-gray-500 focus:border-blue-600 focus:outline-none" />
            <p className="text-sm text-gray-700" aria-live="polite">
                {isPending ? "Loading signals…" : isError ? "" : matches.length > MAX_RENDERED_SIGNALS ? `Best ${MAX_RENDERED_SIGNALS} of ${matches.length} matches. Keep typing to narrow.` : `${matches.length} ${matches.length === 1 ? "signal" : "signals"}`}
                <span className="float-right hidden text-gray-600 sm:inline">↑ ↓ to move · Enter to check · Ctrl + Enter to add</span>
            </p>
            {canSelectAll && (
                <button type="button" onClick={() => setSelected((prev) => [...prev, ...unselectedMatches])} className="self-start rounded border border-blue-600 px-3 py-1.5 text-sm font-medium text-blue-700 hover:bg-blue-50 focus-visible:outline-2 focus-visible:outline-blue-600">
                    Select all {unselectedMatches.length} matches
                </button>
            )}
            <div ref={listRef} id="signal-picker-results" role="listbox" aria-label="Matching signals" aria-multiselectable className="max-h-[min(55vh,32rem)] min-h-40 overflow-y-auto overscroll-contain rounded-lg border-2 border-gray-300 bg-white">
                {isPending ? null : isError ? (
                    <p className="flex items-center gap-3 p-4 text-base text-red-700">
                        Could not load signals.
                        <button type="button" onClick={() => refetch()} className="font-semibold underline">
                            Retry
                        </button>
                    </p>
                ) : visible.length === 0 ? (
                    <p className="p-4 text-base text-gray-700">No signals match “{query.trim()}”.</p>
                ) : (
                    visible.map((signal, index) => {
                        const isHighlighted = index === highlightedIndex;
                        const isAdded = addedSignalNames.includes(signal.name);
                        const isSelected = selectedNames.has(signal.name);
                        return (
                            <div key={signal.name} id={`signal-option-${index}`} data-index={index} role="option" aria-selected={isSelected} aria-disabled={isAdded} onMouseMove={() => setHighlightedIndex(index)} onMouseDown={(event) => event.preventDefault()} onClick={() => toggle(signal)} className={cn("flex items-center gap-3 border-b border-gray-200 px-4 py-3 text-gray-900 last:border-0", isAdded ? "cursor-not-allowed opacity-50" : "cursor-pointer", isSelected ? "bg-blue-50" : isHighlighted && !isAdded && "bg-gray-100", isHighlighted && "outline-2 -outline-offset-2 outline-blue-600")}>
                                <span className={cn("grid size-6 shrink-0 place-items-center rounded border-2", isSelected ? "border-blue-600 bg-blue-600 text-white" : isAdded ? "border-gray-400 bg-gray-200 text-gray-600" : "border-gray-400 bg-white")} aria-hidden>
                                    {(isSelected || isAdded) && <Check className="size-4" strokeWidth={3} />}
                                </span>
                                <span className="w-14 shrink-0 rounded px-1.5 py-1 text-center text-xs font-bold tracking-wide text-white" style={{ backgroundColor: alertNodeColor(signal.tx_node) }}>
                                    {signal.tx_node}
                                </span>
                                <span className="min-w-0 flex-1">
                                    <span className="block text-base leading-snug font-semibold break-words">
                                        <HighlightedName name={signal.name} query={deferredQuery} />
                                    </span>
                                    <span className="block truncate text-sm text-gray-700">
                                        {signal.msg_name}
                                        {signal.cycle_time_ms !== null && ` · ${signal.cycle_time_ms} ms`}
                                    </span>
                                </span>
                                <span className="max-w-40 shrink-0 truncate text-sm font-medium text-gray-700">{isAdded ? "Added" : describeSignal(signal)}</span>
                            </div>
                        );
                    })
                )}
            </div>
            <div className="flex items-center gap-3">
                <span className="text-sm text-gray-700" aria-live="polite">
                    {selected.length === 0 ? "Check the signals to add." : `${selected.length} selected`}
                </span>
                {selected.length > 0 && (
                    <button type="button" onClick={() => setSelected([])} className="text-sm font-medium text-gray-700 underline hover:text-gray-900">
                        Clear
                    </button>
                )}
                <button type="button" onClick={confirm} disabled={selected.length === 0} className="ml-auto rounded bg-blue-600 px-4 py-2 text-base font-semibold text-white hover:bg-blue-700 focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600 disabled:cursor-not-allowed disabled:opacity-50">
                    {selected.length === 0 ? "Add signals" : `Add ${selected.length} ${selected.length === 1 ? "signal" : "signals"}`}
                </button>
            </div>
        </div>
    );
}
