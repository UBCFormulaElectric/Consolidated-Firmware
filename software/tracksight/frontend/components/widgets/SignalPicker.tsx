"use client";

import { Check } from "lucide-react";
import { FC, KeyboardEvent, useCallback, useDeferredValue, useEffect, useMemo, useRef, useState } from "react";

import { useAvailableSignals } from "@/lib/hooks/useAvailableSignals";
import { SignalMetadata } from "@/lib/types/Signal";

const MAX_RENDERED_SIGNALS = 100;

type SignalItemRenderer<T extends SignalMetadata> = FC<{
    data: T;
}>;

type SignalPickerProps<T extends SignalMetadata> = {
    filter: (signal: SignalMetadata) => signal is T;
    getSearchableText: (signal: T) => string;
    ItemRenderer: SignalItemRenderer<T>;
    placeholder: string;

    selectedSignals: T[];
    onSelectedSignalsChange: (signals: T[]) => void;
    onConfirm: (signals: T[]) => void;
};

function normalizeSearchText(value: string): string {
    return value.trim().toLowerCase().replace(/[_-]+/g, " ");
}

function getStatusText(isLoading: boolean, visibleCount: number, matchingCount: number, hasQuery: boolean): string {
    if (isLoading) return "Loading available signals...";
    if (matchingCount > MAX_RENDERED_SIGNALS) return `Showing first ${visibleCount} of ${matchingCount} matching signals`;
    if (!hasQuery) return `Showing ${visibleCount} signals`;

    return `${visibleCount} matching signals`;
}

function SignalPicker<T extends SignalMetadata>(props: SignalPickerProps<T>) {
    const { filter, getSearchableText, ItemRenderer, placeholder, selectedSignals, onSelectedSignalsChange, onConfirm } = props;

    const listRef = useRef<HTMLDivElement | null>(null);
    const [query, setQuery] = useState("");
    const [highlightedIndex, setHighlightedIndex] = useState(0);
    const { data: allSignals = [], isLoading, error } = useAvailableSignals();
    const previousMousePositionRef = useRef<{ x: number; y: number } | null>(null);

    const deferredQuery = useDeferredValue(query);
    const normalizedQuery = normalizeSearchText(deferredQuery);

    const searchableSignals = useMemo(() => {
        return allSignals.filter(filter).map((signal) => ({
            signal,
            searchableText: normalizeSearchText(getSearchableText(signal)),
        }));
    }, [allSignals, filter, getSearchableText]);

    const matchingSignals = useMemo(() => {
        return searchableSignals.filter((entry) => entry.searchableText.includes(normalizedQuery)).map((entry) => entry.signal);
    }, [searchableSignals, normalizedQuery]);

    const visibleSignals = useMemo(() => matchingSignals.slice(0, MAX_RENDERED_SIGNALS), [matchingSignals]);

    const selectedSignalNames = useMemo(() => new Set(selectedSignals.map((signal) => signal.name)), [selectedSignals]);
    const areAllMatchingSelected = matchingSignals.length > 0 && matchingSignals.every((signal) => selectedSignalNames.has(signal.name));

    useEffect(() => {
        setHighlightedIndex(0);
    }, [normalizedQuery]);

    useEffect(() => {
        const highlightedElement = listRef.current?.children[highlightedIndex];

        highlightedElement?.scrollIntoView({ block: "nearest" });
    }, [highlightedIndex]);

    const toggleSignal = useCallback(
        (signal: T, shouldConfirm = false) => {
            const nextSignals = selectedSignalNames.has(signal.name) ? selectedSignals.filter((selected) => selected.name !== signal.name) : [...selectedSignals, signal];

            onSelectedSignalsChange(nextSignals);

            if (!shouldConfirm) return;

            onConfirm(nextSignals);
        },
        [onConfirm, onSelectedSignalsChange, selectedSignalNames, selectedSignals]
    );
    const toggleAllMatching = () => {
        const matchingSignalNames = new Set(matchingSignals.map((signal) => signal.name));
        const signalsOutsideMatch = selectedSignals.filter((signal) => !matchingSignalNames.has(signal.name));

        if (areAllMatchingSelected) {
            onSelectedSignalsChange(signalsOutsideMatch);

            return;
        }

        onSelectedSignalsChange([...signalsOutsideMatch, ...matchingSignals]);
    };

    const moveHighlight = useCallback(
        (offset: number) => {
            if (visibleSignals.length === 0) return;

            setHighlightedIndex((previous) => (previous + offset + visibleSignals.length) % visibleSignals.length);
        },
        [visibleSignals.length]
    );

    useEffect(() => {
        const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>) => {
            if (event.key === "ArrowDown") {
                event.preventDefault();
                moveHighlight(1);
                return;
            }

            if (event.key === "ArrowUp") {
                event.preventDefault();
                moveHighlight(-1);
                return;
            }

            if (event.key === "Enter" && visibleSignals[highlightedIndex]) {
                event.preventDefault();

                toggleSignal(visibleSignals[highlightedIndex], !event.shiftKey);
            }
        };

        document.addEventListener("keydown", handleKeyDown as any);

        return () => {
            document.removeEventListener("keydown", handleKeyDown as any);
        };
    }, [moveHighlight, toggleSignal, visibleSignals, highlightedIndex]);

    return (
        <div className="w-full">
            <label className="mb-1 block text-sm font-medium text-gray-700">Signal Name</label>
            {selectedSignals.length > 0 && (
                <div className="mb-2 flex flex-wrap gap-1.5">
                    {selectedSignals.map((signal) => (
                        <span key={signal.name} className="flex max-w-full items-center gap-1 rounded-full border border-blue-200 bg-blue-100 py-0.5 pl-2.5 pr-1.5 text-xs font-medium text-gray-900">
                            <span className="truncate">{signal.name}</span>
                            <button type="button" onClick={() => toggleSignal(signal)} className="shrink-0 cursor-pointer font-bold text-gray-500 transition-colors hover:text-red-500" title={`Remove ${signal.name}`}>
                                ×
                            </button>
                        </span>
                    ))}
                </div>
            )}
            <input type="text" value={query} onChange={(event) => setQuery(event.target.value)} className="w-full rounded border bg-white px-3 py-2 text-gray-900" placeholder={placeholder} autoFocus autoComplete="off" spellCheck={false} />
            <div className="mt-2 rounded-md border border-gray-200 bg-white">
                <div className="flex items-center justify-between gap-3 border-b border-gray-100 px-3 py-2 text-xs text-gray-500">
                    <span>{getStatusText(isLoading, visibleSignals.length, matchingSignals.length, normalizedQuery.length > 0)}</span>
                    {normalizedQuery.length > 0 && matchingSignals.length > 0 && (
                        <button type="button" onClick={toggleAllMatching} className="cursor-pointer font-medium text-blue-600 hover:text-blue-700">
                            {areAllMatchingSelected ? "Deselect all matching" : `Select all ${matchingSignals.length} matching`}
                        </button>
                    )}
                </div>
                {error ? (
                    <p className="px-3 py-3 text-sm text-red-600">Failed to load available signals: {error.message}</p>
                ) : visibleSignals.length === 0 ? (
                    <p className="px-3 py-3 text-sm text-gray-500">No signals match this search.</p>
                ) : (
                    <div ref={listRef} className="scrollbar-hidden max-h-64 overflow-y-auto py-1">
                        {visibleSignals.map((signal, index) => {
                            const isHighlighted = index === highlightedIndex;
                            const isChecked = selectedSignalNames.has(signal.name);

                            return (
                                <button
                                    key={signal.name}
                                    type="button"
                                    onMouseDown={(event) => event.preventDefault()}
                                    onClick={(event) => toggleSignal(signal, !event.shiftKey)}
                                    onMouseOver={(e) => {
                                        if (previousMousePositionRef.current) {
                                            const { x: prevX, y: prevY } = previousMousePositionRef.current;
                                            const { clientX: currX, clientY: currY } = e;

                                            if (Math.abs(currX - prevX) < 1 && Math.abs(currY - prevY) < 1) return;
                                        }

                                        previousMousePositionRef.current = { x: e.clientX, y: e.clientY };
                                        setHighlightedIndex(index);
                                    }}
                                    className={`flex w-full items-start gap-3 px-3 py-2 text-left transition-colors ${isHighlighted ? "bg-blue-200" : isChecked ? "bg-blue-100" : ""}`}
                                    style={{ contentVisibility: "auto", containIntrinsicSize: "48px" }}
                                >
                                    <span className={`mt-0.5 flex size-4 shrink-0 items-center justify-center rounded border ${isChecked ? "border-blue-600 bg-blue-600 text-white" : "border-gray-300 bg-white"}`}>{isChecked && <Check className="size-3" strokeWidth={3} />}</span>
                                    <div className="min-w-0 flex-1">
                                        <ItemRenderer data={signal} />
                                    </div>
                                </button>
                            );
                        })}
                    </div>
                )}
            </div>
        </div>
    );
}

export type { SignalItemRenderer };

export default SignalPicker;
