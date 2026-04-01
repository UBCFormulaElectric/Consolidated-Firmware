import React, { memo, useEffect, useMemo, useRef, useState } from "react";

import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { fetchHistoricalSignal } from "@/lib/api/historicalSignals";
import { SignalDataStoreProvider } from "@/lib/contexts/signalStores/SignalStoreContext";
import HistoricalSignalStore from "@/lib/signals/HistoricalSignalStore";
import { SignalMetadata } from "@/lib/types/Signal";

type HistoricalSignalStoreProviderProps = {
    children: React.ReactNode;
    startUtcMs: number;
    endUtcMs: number;
    resolution: string;
};

export const HistoricalSignalStoreProvider = memo(function HistoricalSignalStoreProvider(props: HistoricalSignalStoreProviderProps) {
    const { children, startUtcMs, endUtcMs, resolution } = props;
    const { widgets } = useWidgetManager();
    const { updateWithTimestamp } = useSyncedGraph();
    const signalStoreRef = useRef<HistoricalSignalStore>(null!);
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
                        resolution,
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
    }, [endUtcMs, resolution, selectedSignals, startUtcMs]);

    return (
        <SignalDataStoreProvider signalStore={signalStoreRef}>
            {error ? <div className="mx-4 mb-3 rounded-md border border-red-800 bg-red-300 px-3 py-2 text-sm text-black">{error}</div> : null}
            {isLoading ? (
                <div className="mx-4 mb-3 flex items-center gap-3 px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg animate-pulse">
                    <div className="w-2 h-2 bg-green-500 rounded-full animate-bounce" />
                    <span className="text-sm font-medium text-gray-600">Syncing historical data...</span>
                </div>
            ) :
                (!error ?
                    <div className="mx-4 mb-3 flex items-center gap-3 px-3 py-2 bg-green-100 border border-gray-200 rounded-lg ">
                        <span className="text-sm font-medium text-gray-600">Data Synced</span>
                    </div>
                    : null)
            }
            {children}
        </SignalDataStoreProvider>
    );
});
