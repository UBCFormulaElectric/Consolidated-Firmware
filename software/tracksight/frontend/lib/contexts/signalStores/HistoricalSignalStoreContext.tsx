import { Loader2 } from "lucide-react";
import React, { memo, useEffect, useMemo, useRef, useState } from "react";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { fetchHistoricalSignal, HistoricalSignalPoint, HistoricalSignalResult, HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { useHistoricalSelection } from "@/lib/contexts/HistoricalSelectionContext";
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

const filterPointsInRange = (points: HistoricalSignalPoint[], min: number, max: number): HistoricalSignalPoint[] => {
    return points.filter((point) => point.timestampMs >= min && point.timestampMs <= max);
};

export const HistoricalSignalStoreProvider = memo(function HistoricalSignalStoreProvider(props: HistoricalSignalStoreProviderProps) {
    const { children, startUtcMs, endUtcMs, source, selectedRange } = props;
    const { widgets } = useWidgetManager();
    const { updateWithTimestamp, setTimeRange } = useSyncedGraph();
    const { setSyncing } = useHistoricalSelection();
    const signalStoreRef = useRef<HistoricalSignalStore>(null!);
    const initializedSelectedRangeKeyRef = useRef<string | null>(null);
    const [error, setError] = useState<string | null>(null);
    const [isLoading, setIsLoading] = useState(false);
    const isLoadingRef = useRef(false);
    const [progress, setProgress] = useState({ done: 0, total: 0, points: 0, alertsDone: false });

    if (!signalStoreRef.current) {
        signalStoreRef.current = new HistoricalSignalStore(updateWithTimestamp);
    }

    const previousSelectedSignalsRef = useRef<SignalMetadata[]>([]);
    const selectedSignals = useMemo(() => {
        const signalsByName = new Map<string, SignalMetadata>();
        widgets.forEach((widget) => {
            if ("signals" in widget) {
                widget.signals.forEach((signal) => {
                    signalsByName.set(signal.name, signal);
                });
            }
        });

        const previousSelectedSignals = previousSelectedSignalsRef.current;
        const isSameSignalSet = previousSelectedSignals.length === signalsByName.size && previousSelectedSignals.every((signal) => signalsByName.has(signal.name));
        if (isSameSignalSet) {
            return previousSelectedSignals;
        }

        const nextSelectedSignals = [...signalsByName.values()];
        previousSelectedSignalsRef.current = nextSelectedSignals;
        return nextSelectedSignals;
    }, [widgets]);

    const selectedRangeKey = `${source}:${selectedRange.min}:${selectedRange.max}`;

    useEffect(() => {
        let isCancelled = false;

        const track = async (promise: Promise<HistoricalSignalResult>, isAlert = false) => {
            let points = 0;
            try {
                const result = await promise;
                points = result.points.length;
                return result;
            } finally {
                if (!isCancelled) setProgress((prev) => (isAlert ? { ...prev, alertsDone: true, points: prev.points + points } : { ...prev, done: prev.done + 1, points: prev.points + points }));
            }
        };

        const load = async () => {
            setError(null);
            setIsLoading(true);
            setProgress({ done: 0, total: selectedSignals.length, points: 0, alertsDone: false });

            // Alerts are by far the slowest request, so start them alongside the signals rather than after.
            const alertPromise = Promise.allSettled([
                track(
                    fetchHistoricalSignal({
                        signalName: "alert",
                        signalType: SignalType.ALERT,
                        startUtcMs,
                        endUtcMs,
                        source,
                    }),
                    true
                ),
            ]);

            const results = await Promise.allSettled(
                selectedSignals.map(async (signal) => ({
                    signal,
                    result: await track(
                        fetchHistoricalSignal({
                            signalName: signal.name,
                            startUtcMs,
                            endUtcMs,
                            source,
                            signalType: signal.type,
                        })
                    ),
                }))
            );

            if (isCancelled) {
                return;
            }

            const failures = results.filter((result) => result.status === "rejected");
            const successes = results.filter((result): result is PromiseFulfilledResult<{ signal: SignalMetadata; result: HistoricalSignalResult }> => result.status === "fulfilled");

            successes.forEach((result) => {
                const { signal, result: signalResult } = result.value;

                const filteredPoints = filterPointsInRange(signalResult.points, selectedRange.min, selectedRange.max);

                signalStoreRef.current.mergeSignal(signal, signalResult.resolutionMs, startUtcMs, endUtcMs, filteredPoints);
            });

            const shouldFitViewport = initializedSelectedRangeKeyRef.current !== selectedRangeKey;
            if (shouldFitViewport) {
                setTimeRange(selectedRange, true);
                initializedSelectedRangeKeyRef.current = selectedRangeKey;
            }

            // Draw signals now; alerts can take much longer.
            const [alertResult] = await alertPromise;
            if (isCancelled) {
                return;
            }

            if (alertResult.status === "fulfilled") {
                const { resolutionMs, points } = alertResult.value;
                signalStoreRef.current.mergeAlerts(resolutionMs, startUtcMs, endUtcMs, filterPointsInRange(points, selectedRange.min, selectedRange.max));
            } else {
                failures.push(alertResult);
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

    useEffect(() => {
        isLoadingRef.current = isLoading;
        setSyncing(isLoading);
    }, [isLoading, setSyncing]);

    useEffect(() => () => setSyncing(false), [setSyncing]);

    return (
        <SignalDataStoreProvider signalStore={signalStoreRef} isLoadingRef={isLoadingRef}>
            {isLoading ? (
                <div className="mx-4 mb-3 overflow-hidden rounded border border-black/20 text-base">
                    <div className="flex items-center gap-3 px-4 py-3">
                        <Loader2 className="size-6 animate-spin text-blue-500" />
                        <div>
                            <div>
                                Loading session data · {progress.done} / {progress.total} signals · {progress.points.toLocaleString()} points
                            </div>
                            <div className="text-sm opacity-70">{progress.alertsDone ? "Alerts loaded" : "Loading alerts… (usually the slowest part)"}</div>
                        </div>
                    </div>
                    <div className="h-1 bg-blue-500 transition-[width] duration-200" style={{ width: `${((progress.done + Number(progress.alertsDone)) / (progress.total + 1)) * 100}%` }} />
                </div>
            ) : null}
            {error ? <div className="mx-4 mb-3 rounded border border-red-500 bg-red-100 px-3 py-2 text-sm whitespace-pre-line text-red-600">{error}</div> : null}
            {children}
        </SignalDataStoreProvider>
    );
});
