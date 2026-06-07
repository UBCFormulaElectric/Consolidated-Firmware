import React, { memo, useEffect, useMemo, useRef, useState } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { fetchHistoricalSignal, HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL } from "@/lib/constants";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import HistoricalSignalStore from "@/lib/signals/HistoricalSignalStore";
import { isAlertSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

type HistoricalSignalStoreProviderProps = {
    children: React.ReactNode;
    startUtcMs: number;
    endUtcMs: number;
    source: HistoricalSignalSource;
    selectedRange: {
        min: number;
        max: number;
    };
    // Reports sync status upward so it can be shown in the fixed page header
    // instead of scrolling away with the dashboard content.
    onStatusChange?: (status: { isLoading: boolean; error: string | null }) => void;
};

export const HistoricalSignalStoreProvider = memo(function HistoricalSignalStoreProvider(props: HistoricalSignalStoreProviderProps) {
    const { children, startUtcMs, endUtcMs, source, selectedRange, onStatusChange } = props;
    const { widgets } = useWidgetManager();
    const { updateWithTimestamp, setTimeRange } = useSyncedGraph();
    const signalStoreRef = useRef<HistoricalSignalStore>(null!);
    const initializedSelectedRangeKeyRef = useRef<string | null>(null);
    const [error, setError] = useState<string | null>(null);
    const [isLoading, setIsLoading] = useState(false);

    if (!signalStoreRef.current) {
        signalStoreRef.current = new HistoricalSignalStore(updateWithTimestamp);
    }

    const selectedSignals = useMemo(() => {
        const signalsByName = new Map<string, SignalMetadata>();
        widgets.forEach((widget) => {
            widget.signals.forEach((signal) => {
                signalsByName.set(signal.name, signal);
            });
        });
        return [...signalsByName.values()];
    }, [widgets]);

    const selectedRangeKey = `${source}:${selectedRange.min}:${selectedRange.max}`;

    useEffect(() => {
        let isCancelled = false;

        const load = async () => {
            setError(null);
            if (selectedSignals.length === 0) {
                setIsLoading(false);
                return;
            }

            setIsLoading(true);
            signalStoreRef.current.clearSignals(selectedSignals);

            const results = await Promise.allSettled(
                selectedSignals.map(async (signal) => ({
                    signal,
                    points: await fetchHistoricalSignal({
                        signalName: signal.name,
                        startUtcMs,
                        endUtcMs,
                        source,
                    }),
                }))
            );

            if (isCancelled) {
                return;
            }

            const failures = results.filter((result) => result.status === "rejected");
            const successes = results.filter((result): result is PromiseFulfilledResult<{ signal: SignalMetadata; points: { timestampMs: number; value: number; name: string }[] }> => result.status === "fulfilled");

            successes.forEach((result) => {
                signalStoreRef.current.hydrateSignal(result.value.signal, result.value.points);
            });

            const shouldFitViewport = initializedSelectedRangeKeyRef.current !== selectedRangeKey;
            if (shouldFitViewport) {
                setTimeRange(selectedRange, true);
                initializedSelectedRangeKeyRef.current = selectedRangeKey;
            }

            if (failures.length > 0) {
                let failString: string = "";
                failures.forEach((failure) => {
                    failString += failure.reason + "\n";
                });

                setError(failString);
            }

            setIsLoading(false);
        };

        void load();

        return () => {
            isCancelled = true;
        };
    }, [endUtcMs, selectedRange, selectedRangeKey, selectedSignals, setTimeRange, source, startUtcMs]);

    // Alert signals aren't widget-selected: the live view streams every alert to all clients,
    // so historical mirrors that by loading all alert signals across the whole session range
    // (not the panning viewport). Decoupling from the viewport keeps the append-only
    // `AlertTimeline` widget from being reset on every pan/zoom.
    useEffect(() => {
        let isCancelled = false;

        const loadAlerts = async () => {
            const store = signalStoreRef.current;
            store.clearAlertData();

            let alertSignals: SignalMetadata[];
            try {
                const allMetadata = await fetchSignalMetadata(API_BASE_URL);
                alertSignals = allMetadata.filter(isAlertSignalMetadata);
            } catch {
                // Metadata fetch failures shouldn't break signal rendering; just skip alerts.
                return;
            }

            if (isCancelled || alertSignals.length === 0) {
                return;
            }

            const results = await Promise.allSettled(
                alertSignals.map(async (signal) => ({
                    name: signal.name,
                    points: await fetchHistoricalSignal({
                        signalName: signal.name,
                        startUtcMs: selectedRange.min,
                        endUtcMs: selectedRange.max,
                        source,
                        agg: "max",
                    }),
                }))
            );

            if (isCancelled) {
                return;
            }

            results.forEach((result) => {
                if (result.status === "fulfilled") {
                    store.hydrateAlertSignal(result.value.name, result.value.points);
                }
            });
        };

        void loadAlerts();

        return () => {
            isCancelled = true;
        };
    }, [source, selectedRange]);

    useEffect(() => {
        onStatusChange?.({ isLoading, error });
    }, [isLoading, error, onStatusChange]);

    return (
        <SignalDataStoreProvider signalStore={signalStoreRef}>
            {error ? <div className="mx-4 mb-3 rounded-md border border-red-800 bg-red-300 px-3 py-2 text-sm text-black">{error}</div> : null}
            {children}
        </SignalDataStoreProvider>
    );
});
