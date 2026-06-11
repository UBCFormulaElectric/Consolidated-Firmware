import React, { memo, useEffect, useMemo, useRef, useState } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { fetchHistoricalSignal, HistoricalSignalResult, HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import HistoricalSignalStore from "@/lib/signals/HistoricalSignalStore";
import { SignalMetadata, SignalType } from "@/lib/types/Signal";

type HistoricalSignalStoreProviderProps = {
    children: React.ReactNode;
    startUtcMs: number;
    endUtcMs: number;
    source: HistoricalSignalSource;
    selectedRange: {
        min: number;
        max: number;
    };
};

export const HistoricalSignalStoreProvider = memo(function HistoricalSignalStoreProvider(props: HistoricalSignalStoreProviderProps) {
    const { children, startUtcMs, endUtcMs, source, selectedRange } = props;
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
            setIsLoading(true);

            const results = await Promise.allSettled(
                selectedSignals.map(async (signal) => ({
                    signal,
                    result: await fetchHistoricalSignal({
                        signalName: signal.name,
                        startUtcMs,
                        endUtcMs,
                        source,
                        signalType: signal.type,
                    }),
                }))
            );

            {
                const alertResult = await fetchHistoricalSignal({
                    signalName: "alert",
                    signalType: SignalType.ALERT,
                    startUtcMs,
                    endUtcMs,
                    source,
                });

                signalStoreRef.current.mergeAlerts(alertResult.resolutionMs, startUtcMs, endUtcMs, alertResult.points);
            }

            if (isCancelled) {
                return;
            }

            const failures = results.filter((result) => result.status === "rejected");
            const successes = results.filter((result): result is PromiseFulfilledResult<{ signal: SignalMetadata; result: HistoricalSignalResult }> => result.status === "fulfilled");

            successes.forEach((result) => {
                const { signal, result: signalResult } = result.value;
                signalStoreRef.current.mergeSignal(signal, signalResult.resolutionMs, startUtcMs, endUtcMs, signalResult.points);
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

    return (
        <SignalDataStoreProvider signalStore={signalStoreRef}>
            {error ? <div className="mx-4 mb-3 rounded-md border border-red-800 bg-red-300 px-3 py-2 text-sm text-black">{error}</div> : null}
            {isLoading ? (
                <div className="mx-4 mb-3 flex items-center gap-3 px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg animate-pulse">
                    <div className="w-2 h-2 bg-green-500 rounded-full animate-bounce" />
                    <span className="text-sm font-medium text-gray-600">Syncing {source === "SdCard" ? "SD card" : "historical"} data...</span>
                </div>
            ) : !error ? (
                <div className="mx-4 mb-3 flex items-center gap-3 px-3 py-2 bg-green-100 border border-gray-200 rounded-lg ">
                    <span className="text-sm font-medium text-gray-600">{source === "SdCard" ? "SD Card" : "Historical"} Data Synced</span>
                </div>
            ) : null}
            {children}
        </SignalDataStoreProvider>
    );
});
