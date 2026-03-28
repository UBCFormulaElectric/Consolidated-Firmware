"use client";

import { KeyboardEvent, memo, useDeferredValue, useEffect, useMemo, useRef, useState } from "react";

import { useAvailableNumericalSignals } from "@/lib/hooks/useAvailableNumericalSignals";
import { NumericalSignalMetadata } from "@/lib/types/Signal";

const MAX_RENDERED_SIGNALS = 100;

function normalizeSearchText(value: string): string {
    return value.trim().toLowerCase().replace(/[_-]+/g, " ");
}

function getSignalSubtitle(signal: NumericalSignalMetadata): string {
    const parts = [signal.msg_name, signal.tx_node];

    if (signal.unit) {
        parts.push(signal.unit);
    }

    parts.push(`${signal.cycle_time_ms}ms`);
    return parts.join(" • ");
}

export const NumericalSignalPicker = memo(function NumericalSignalPicker({ query, selectedSignalName, onQueryChange, onSelectSignal }: { query: string; selectedSignalName: string | null; onQueryChange: (value: string) => void; onSelectSignal: (signal: NumericalSignalMetadata) => void }) {
    const containerRef = useRef<HTMLDivElement | null>(null);
    const [isListOpen, setIsListOpen] = useState(false);
    const [highlightedIndex, setHighlightedIndex] = useState(0);
    const { data: availableSignals = [], isLoading, error } = useAvailableNumericalSignals();
    const deferredQuery = useDeferredValue(query); // i've noticed this helps a little bit with lag when typing
    const normalizedQuery = normalizeSearchText(deferredQuery);

    const searchableSignals = useMemo(() => {
        return availableSignals.map((signal) => ({
            signal,
            searchableText: normalizeSearchText([signal.name, signal.msg_name, signal.tx_node, signal.unit].filter(Boolean).join(" ")), // look for signals based on any useful field
        }));
    }, [availableSignals]);

    const matchingSignals = useMemo(() => {
        if (!normalizedQuery) {
            return availableSignals;
        }

        return searchableSignals.filter((entry) => entry.searchableText.includes(normalizedQuery)).map((entry) => entry.signal);
    }, [availableSignals, normalizedQuery, searchableSignals]);

    const visibleSignals = useMemo(() => matchingSignals.slice(0, MAX_RENDERED_SIGNALS), [matchingSignals]);

    useEffect(() => {
        setHighlightedIndex(0);
    }, [normalizedQuery]);

    useEffect(() => {
        const handlePointerDown = (event: MouseEvent) => {
            if (!containerRef.current?.contains(event.target as Node)) {
                setIsListOpen(false);
            }
        };

        document.addEventListener("mousedown", handlePointerDown);
        return () => document.removeEventListener("mousedown", handlePointerDown);
    }, []); // close the list if user clicks outside of the component

    const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>) => {
        if (event.key === "Enter" && isListOpen && visibleSignals[highlightedIndex]) {
            event.preventDefault();
            onSelectSignal(visibleSignals[highlightedIndex]);
            setIsListOpen(false);
            return;
        }

        if (event.key === "Escape") {
            event.preventDefault();
            setIsListOpen(false);
        }
    };

    return (
        <div ref={containerRef}>
            <label className="block text-sm font-medium text-gray-700 mb-1">Signal Name</label>
            <input
                type="text"
                value={query}
                onChange={(event) => {
                    onQueryChange(event.target.value);
                    setIsListOpen(true);
                }}
                onFocus={() => setIsListOpen(true)}
                onKeyDown={handleKeyDown}
                className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                placeholder="Search by signal, message, unit, or node"
                autoFocus
                autoComplete="off"
                spellCheck={false}
            />
            <div className="mt-2 rounded-md border border-gray-200 bg-white">
                <div className="border-b border-gray-100 px-3 py-2 text-xs text-gray-500">{isLoading ? "Loading available signals..." : matchingSignals.length > MAX_RENDERED_SIGNALS ? `Showing first ${visibleSignals.length} of ${matchingSignals.length} matching signals` : query.trim().length === 0 ? `Showing ${visibleSignals.length} signals` : `${visibleSignals.length} matching signals`}</div>
                {error ? (
                    <p className="px-3 py-3 text-sm text-red-600">Failed to load available signals.</p>
                ) : isListOpen ? (
                    <div className="max-h-64 overflow-y-auto py-1">
                        {visibleSignals.length === 0 ? (
                            <p className="px-3 py-3 text-sm text-gray-500">No signals match this search.</p>
                        ) : (
                            <>
                                {visibleSignals.map((signal, index) => {
                                    const isHighlighted = index === highlightedIndex;
                                    const isSelected = signal.name === selectedSignalName;

                                    return (
                                        <button
                                            key={signal.name}
                                            type="button"
                                            onMouseDown={(event) => event.preventDefault()}
                                            onClick={() => {
                                                onSelectSignal(signal);
                                                setIsListOpen(false);
                                            }}
                                            className={`w-full px-3 py-2 text-left transition-colors ${isHighlighted ? "bg-blue-200" : "hover:bg-blue-100"} ${isSelected ? "bg-blue-100" : ""}`}
                                            style={{ contentVisibility: "auto", containIntrinsicSize: "48px" }}
                                        >
                                            <div className="flex items-center justify-between gap-3">
                                                <span className="font-medium text-gray-900">{signal.name}</span>
                                                {signal.unit && <span className="shrink-0 text-xs uppercase tracking-wide text-gray-500">{signal.unit}</span>}
                                            </div>
                                            <p className="mt-1 text-xs text-gray-500">{getSignalSubtitle(signal)}</p>
                                        </button>
                                    );
                                })}
                            </>
                        )}
                    </div>
                ) : (
                    <button type="button" onClick={() => setIsListOpen(true)} className="w-full px-3 py-3 text-left text-sm text-gray-500 hover:bg-gray-50">
                        Show matching signals
                    </button>
                )}
            </div>
        </div>
    );
});
