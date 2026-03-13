"use client";
// react
import { useEffect, useRef, useState, RefObject, SubmitEvent } from "react";
// types
import { NumericalSignalConfig, WidgetDataNumerical } from "@/components/widgets/WidgetTypes";
import { isNumericalSignalMetadata, NumericalSignalMetadata, SignalMetadata, SignalType } from "@/lib/types/Signal";
import { ChartData, ChartDataNumerical, NumericalSeries } from "./CanvasChartTypes";
import { SeriesData } from "@/lib/seriesData";
import chroma from "chroma-js";
// hooks
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useDataVersion, useSignals } from "@/lib/contexts/SignalContext";
import { API_BASE_URL } from "@/lib/constants";
import { useWidgetManager } from "./WidgetManagerContext";
// components
import { Dialog, DialogDescription, DialogHeader, DialogTitle, DialogContent, DialogTrigger } from "@/components/ui/dialog";
import { PlusButton } from "@/components/icons/PlusButton";

function createEmptyNumericalChartData(): ChartDataNumerical {
    return { type: SignalType.NUMERICAL, all_series: [] };
}

function createEmptyNumericalSeries(label: string): NumericalSeries {
    return { label, timestamps: [], data: new SeriesData() };
}

function parsePointTimestamp(time: number | string): number {
    return typeof time === "number" ? time : new Date(time).getTime();
}

function parsePointValue(value: number | string | undefined): number {
    if (typeof value === "number") {
        return value;
    }

    if (typeof value === "string") {
        return Number(value);
    }

    return Number.NaN;
}

async function fetchNumericalSignalMetadata(signalName: string): Promise<NumericalSignalMetadata> {
    const response = await fetch(`${API_BASE_URL}/signal?name=${encodeURIComponent(signalName)}`);

    if (!response.ok) {
        throw new Error(`Failed to fetch signal metadata: ${response.statusText}`);
    }

    const payload = (await response.json()) as SignalMetadata[];
    const signal = payload.find((candidate) => candidate.name === signalName);

    if (!signal) {
        throw new Error("Signal not found");
    }

    if (!isNumericalSignalMetadata(signal)) {
        throw new Error("Signal is not numerical");
    }

    return signal;
}

export function NumericalWidgetModalForm({ closeModal }: { closeModal: () => void; }) {
    const { appendWidget: onAddWidget } = useWidgetManager();
    const [numericalSignalName, setNumericalSignalName] = useState("");
    const [error, setError] = useState<string | null>(null);
    const [isLoading, setIsLoading] = useState(false);

    const handleNumericalSubmit = async (e: SubmitEvent<HTMLFormElement>) => {
        e.preventDefault();
        const trimmedSignalName = numericalSignalName.trim();

        setIsLoading(true);
        setError(null);

        try {
            const metadata = await fetchNumericalSignalMetadata(trimmedSignalName);

            onAddWidget({
                type: SignalType.NUMERICAL,
                signals: [{
                    signal_name: trimmedSignalName,
                    delay: metadata.cycle_time_ms,
                    min: metadata.min_val,
                    max: metadata.max_val,
                    color: chroma.random()
                }],
                id: trimmedSignalName,
                mock: false,
            });
            closeModal();
        } catch (e) {
            setError(e instanceof Error ? e.message : "Failed to create widget");
        } finally {
            setIsLoading(false);
        }
    };

    return (
        <>
            <DialogHeader>
                <DialogTitle>New Numerical Widget</DialogTitle>
                <DialogDescription> options for configuring the numerical widget </DialogDescription>
            </DialogHeader>
            <form onSubmit={handleNumericalSubmit} className="space-y-4">
                <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                        Signal Name
                    </label>
                    <input
                        type="text"
                        value={numericalSignalName}
                        onChange={(e) => {
                            setNumericalSignalName(e.target.value);
                            if (error) {
                                setError(null);
                            }
                        }}
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        placeholder="e.g. BMS_TractiveSystemVoltage"
                        autoFocus
                        required
                    />
                </div>
                {error && (
                    <p className="text-sm text-red-600">{error}</p>
                )}
                <div className="flex justify-end gap-2 pt-2">
                    <button type="button" onClick={closeModal} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
                        Cancel
                    </button>
                    <button
                        type="submit"
                        disabled={isLoading || numericalSignalName.trim().length === 0}
                        className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded cursor-pointer disabled:cursor-not-allowed disabled:opacity-60"
                    >
                        {isLoading ? "Creating..." : "Create Widget"}
                    </button>
                </div>
            </form>
        </>
    );
}



export function useLiveNumericalData(widgetData: WidgetDataNumerical): RefObject<ChartData> {
    const dataRef = useRef<ChartDataNumerical>(createEmptyNumericalChartData());
    const previousSignalNamesRef = useRef<string[]>([]);
    const seenCountRef = useRef<Map<string, number>>(new Map());
    const { getSignalData, subscribeToSignal, unsubscribeFromSignal } = useSignals();
    const dataVersion = useDataVersion();
    const { updateWithTimestamp } = useSyncedGraph();
    const signalNames = widgetData.signals.map((signal) => signal.signal_name);
    const signalNamesKey = `${widgetData.id}:${signalNames.join("\0")}`;

    useEffect(() => {
        for (const signalName of previousSignalNamesRef.current) {
            try {
                unsubscribeFromSignal(signalName);
            } catch (error) {
                console.warn(`Failed to unsubscribe from signal ${signalName}:`, error);
            }
        }

        dataRef.current.all_series = [];
        previousSignalNamesRef.current = [];
        seenCountRef.current = new Map();
    }, [widgetData.id, unsubscribeFromSignal]);

    useEffect(() => {
        const previousSignalNames = previousSignalNamesRef.current;
        const previousSignalSet = new Set(previousSignalNames);
        const nextSignalSet = new Set(signalNames);

        for (const signalName of signalNames) {
            if (!previousSignalSet.has(signalName)) {
                subscribeToSignal(signalName);
            }
            if (!seenCountRef.current.has(signalName)) {
                seenCountRef.current.set(signalName, 0);
            }
        }

        for (const signalName of previousSignalNames) {
            if (!nextSignalSet.has(signalName)) {
                unsubscribeFromSignal(signalName);
                seenCountRef.current.delete(signalName);
            }
        }

        const existingSeriesByLabel = new Map(dataRef.current.all_series.map((series) => [series.label, series]));
        dataRef.current.all_series = signalNames.map((signalName) => existingSeriesByLabel.get(signalName) ?? createEmptyNumericalSeries(signalName));
        previousSignalNamesRef.current = [...signalNames];
    }, [signalNamesKey, subscribeToSignal, unsubscribeFromSignal]);

    useEffect(() => {
        for (const signalName of signalNames) {
            const signalData = getSignalData(signalName);
            let seenCount = seenCountRef.current.get(signalName) ?? 0;
            let series = dataRef.current.all_series.find((candidate) => candidate.label === signalName);

            if (!series) {
                series = createEmptyNumericalSeries(signalName);
                dataRef.current.all_series.push(series);
            }

            if (signalData.length < seenCount) {
                series.timestamps = [];
                series.data = new SeriesData();
                seenCount = 0;
            }

            for (const point of signalData.slice(seenCount)) {
                const timestamp = parsePointTimestamp(point.time);
                const value = parsePointValue(point.value);

                if (!Number.isFinite(timestamp) || !Number.isFinite(value)) {
                    continue;
                }

                series.timestamps.push(timestamp);
                series.data.push(value);
                updateWithTimestamp(timestamp);
            }

            seenCountRef.current.set(signalName, signalData.length);
        }
    }, [dataVersion, getSignalData, signalNamesKey, updateWithTimestamp]);

    useEffect(() => {
        return () => {
            for (const signalName of previousSignalNamesRef.current) {
                unsubscribeFromSignal(signalName);
            }
        };
    }, [unsubscribeFromSignal]);

    return dataRef as RefObject<ChartData>;
}

function NumericalSignalModalForm ({ closeModal, configs, widgetData }: {
    closeModal: () => void;
    configs: NumericalSignalConfig[];
    widgetData: WidgetDataNumerical;
}) {
    const [signalName, setSignalName] = useState("");
    const [error, setError] = useState<string | null>(null);
    const [isLoading, setIsLoading] = useState(false);
    const { appendSignal } = useWidgetManager();

    const handleClose = () => {
        setSignalName("");
        setError(null);
        closeModal();
    };

    const handleAddSignal = async (e: SubmitEvent<HTMLFormElement>) => {
        e.preventDefault();
        const trimmedSignalName = signalName.trim();

        if (!trimmedSignalName) {
            setError("Signal name is required");
            return;
        }

        if (configs.some((c) => c.signal_name === trimmedSignalName)) {
            setError("Already listening to this signal");
            return;
        }

        setIsLoading(true);
        setError(null);

        try {
            const metadata = await fetchNumericalSignalMetadata(trimmedSignalName);

            appendSignal(widgetData.id, {
                delay: metadata.cycle_time_ms ?? 100,
                min: metadata.min_val,
                max: metadata.max_val,
                signal_name: trimmedSignalName,
                color: chroma.random()
            } satisfies NumericalSignalConfig);
            handleClose();
        } catch (metadataError) {
            setError(metadataError instanceof Error ? metadataError.message : "Failed to add signal");
        } finally {
            setIsLoading(false);
        }
    };

    return (
        <form onSubmit={handleAddSignal} className="space-y-4">
            <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                    Signal Name
                </label>
                <input
                    type="text"
                    value={signalName}
                    onChange={(e) => {
                        setSignalName(e.target.value);
                        if (error) {
                            setError(null);
                        }
                    }}
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                    placeholder="e.g. TractiveSystemVoltage"
                    autoFocus
                />
            </div>
            <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                    Type
                </label>
                <div className="w-full border rounded px-3 py-2 text-gray-900 bg-gray-50">
                    {widgetData.type === SignalType.NUMERICAL ? "Numerical" : "Enumeration"}
                </div>
            </div>
            {error && (
                <p className="text-sm text-red-600">{error}</p>
            )}
            <div className="flex justify-end gap-2 pt-2">
                <button type="button" onClick={handleClose} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
                    Cancel
                </button>
                <button
                    type="submit"
                    disabled={isLoading || signalName.trim().length === 0}
                    className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded cursor-pointer disabled:cursor-not-allowed disabled:opacity-60"
                >
                    {isLoading ? "Adding..." : "Add"}
                </button>
            </div>
        </form>);
};

export function NumericalWidgetAddSignalModal({ configs, widgetData }: {
    configs: NumericalSignalConfig[];
    widgetData: WidgetDataNumerical;
}) {
    const [modalOpen, setModalOpen] = useState(false);
    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger>
                <PlusButton />
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Add Numerical Signal</DialogTitle>
                    <DialogDescription>
                        Add a live numerical signal to this graph.
                    </DialogDescription>
                </DialogHeader>
                <NumericalSignalModalForm closeModal={() => setModalOpen(false)} configs={configs} widgetData={widgetData} />
            </DialogContent>
        </Dialog>
    );
}
