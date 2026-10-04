"use client";

import { KeyboardEvent, ReactNode, useDeferredValue, useEffect, useMemo, useRef, useState } from "react";

import { alertNodeColor } from "@/lib/alerts";
import { useAvailableSignals } from "@/lib/hooks/useAvailableSignals";
import { findMatchRanges, prepareSignalSearch, searchSignals } from "@/lib/signalSearch";
import { BooleanSignalMetadata, EnumSignalMetadata, isBooleanSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { cn } from "@/lib/utils";

const MAX_RENDERED_SIGNALS = 100;
// same cap as #2157's select all, so one search can't flood a chart
const ADD_ALL_LIMIT = 25;

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
    /** keepOpen is true when shift was held, so several signals can be picked in one go */
    onPick: (signal: T, keepOpen: boolean) => void;
    /** shift-picking an added signal takes it off again; without this, added signals can't be picked */
    onRemove?: (signal: T) => void;
    /** adds every match that isn't added yet; offered when there are at most ADD_ALL_LIMIT */
    onPickAll?: (signals: T[]) => void;
    /** already on the chart: listed, but not pickable */
    addedSignalNames?: string[];
}) {
    const { accept, onPick, onRemove, onPickAll, addedSignalNames = [] } = props;
    const [query, setQuery] = useState("");
    const [highlightedIndex, setHighlightedIndex] = useState(0);
    const listRef = useRef<HTMLDivElement>(null);
    const { data: allSignals, isPending, isError, refetch } = useAvailableSignals();
    const deferredQuery = useDeferredValue(query);

    const searchable = useMemo(() => prepareSignalSearch((allSignals ?? []).filter(accept)), [allSignals, accept]);
    const matches = useMemo(() => searchSignals(searchable, deferredQuery), [searchable, deferredQuery]);
    const visible = matches.slice(0, MAX_RENDERED_SIGNALS);

    useEffect(() => setHighlightedIndex(0), [deferredQuery]);

    useEffect(() => {
        listRef.current?.querySelector(`[data-index="${highlightedIndex}"]`)?.scrollIntoView({ block: "nearest" });
    }, [highlightedIndex]);

    const pick = (signal: T | undefined, keepOpen: boolean) => {
        if (!signal) return;
        if (!addedSignalNames.includes(signal.name)) onPick(signal, keepOpen);
        else if (keepOpen) onRemove?.(signal);
    };
    const notYetAdded = matches.filter((signal) => !addedSignalNames.includes(signal.name));
    const canAddAll = onPickAll !== undefined && deferredQuery.trim() !== "" && notYetAdded.length > 1 && notYetAdded.length <= ADD_ALL_LIMIT;

    const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>) => {
        if (visible.length === 0) return;
        if (event.key === "ArrowDown" || event.key === "ArrowUp") {
            event.preventDefault();
            const step = event.key === "ArrowDown" ? 1 : -1;
            setHighlightedIndex((index) => (index + step + visible.length) % visible.length);
        } else if (event.key === "Enter") {
            event.preventDefault();
            pick(visible[highlightedIndex], event.shiftKey);
        }
    };

    return (
        <div className="flex min-h-0 flex-col gap-3">
            <input type="search" aria-label="Search signals" aria-controls="signal-picker-results" aria-activedescendant={visible[highlightedIndex] ? `signal-option-${highlightedIndex}` : undefined} role="combobox" aria-expanded value={query} onChange={(event) => setQuery(event.target.value)} onKeyDown={handleKeyDown} autoFocus autoComplete="off" spellCheck={false} placeholder="Search by signal, message, node, or unit" className="h-14 w-full rounded-lg border-2 border-gray-400 bg-white px-4 text-lg text-gray-900 placeholder:text-gray-500 focus:border-blue-600 focus:outline-none" />
            <p className="text-sm text-gray-700" aria-live="polite">
                {isPending ? "Loading signals…" : isError ? "" : matches.length > MAX_RENDERED_SIGNALS ? `Best ${MAX_RENDERED_SIGNALS} of ${matches.length} matches. Keep typing to narrow.` : `${matches.length} ${matches.length === 1 ? "signal" : "signals"}`}
                <span className="float-right hidden text-gray-600 sm:inline">↑ ↓ to move · Enter to add · hold Shift to pick several</span>
            </p>
            {canAddAll && (
                <button type="button" onClick={() => onPickAll(notYetAdded)} className="self-start rounded border border-blue-600 px-3 py-1.5 text-sm font-medium text-blue-700 hover:bg-blue-50 focus-visible:outline-2 focus-visible:outline-blue-600">
                    Add all {notYetAdded.length} matches
                </button>
            )}
            <div ref={listRef} id="signal-picker-results" role="listbox" aria-label="Matching signals" className="max-h-[min(60vh,36rem)] min-h-40 overflow-y-auto overscroll-contain rounded-lg border-2 border-gray-300 bg-white">
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
                        return (
                            <div key={signal.name} id={`signal-option-${index}`} data-index={index} role="option" aria-selected={isHighlighted} aria-disabled={isAdded && !onRemove} onMouseMove={() => setHighlightedIndex(index)} onMouseDown={(event) => event.preventDefault()} onClick={(event) => pick(signal, event.shiftKey)} className={cn("flex items-center gap-3 border-b border-gray-200 px-4 py-3 last:border-0", isAdded && !onRemove ? "cursor-not-allowed opacity-50" : isAdded ? "cursor-pointer opacity-50" : "cursor-pointer", isHighlighted && !isAdded ? "bg-blue-700 text-white" : isHighlighted ? "bg-gray-100" : "text-gray-900")}>
                                <span className="w-14 shrink-0 rounded px-1.5 py-1 text-center text-xs font-bold tracking-wide text-white" style={{ backgroundColor: alertNodeColor(signal.tx_node) }}>
                                    {signal.tx_node}
                                </span>
                                <span className="min-w-0 flex-1">
                                    <span className="block text-base leading-snug font-semibold break-words">
                                        <HighlightedName name={signal.name} query={deferredQuery} />
                                    </span>
                                    <span className={cn("block truncate text-sm", isHighlighted && !isAdded ? "text-blue-100" : "text-gray-700")}>
                                        {signal.msg_name}
                                        {signal.cycle_time_ms !== null && ` · ${signal.cycle_time_ms} ms`}
                                    </span>
                                </span>
                                <span className={cn("max-w-40 shrink-0 truncate text-sm font-medium", isHighlighted && !isAdded ? "text-white" : "text-gray-700")}>{isAdded ? "Added" : describeSignal(signal)}</span>
                            </div>
                        );
                    })
                )}
            </div>
        </div>
    );
}
