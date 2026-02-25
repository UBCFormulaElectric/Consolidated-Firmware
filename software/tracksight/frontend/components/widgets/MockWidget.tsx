"use client";

import { useEffect, useState, useRef, useCallback, RefObject, SubmitEvent } from "react";
import { NumericalSignalConfig, WidgetConfigs, WidgetData } from "@/lib/types/Widget";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalType } from "@/lib/types/Signal";
import { DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { ChartData } from "./chart_types";

function generateRandomNumericalValue(time: number, index: number = 0, min: number, max: number) {
    if (min !== undefined && max !== undefined) {
        const range = max - min;
        // Generate a wave within the range
        const normalized = (Math.sin(time / 1000 + index) + 1) / 2; // 0 to 1

        // Add some noise
        const noise = (Math.random() - 0.5) * 0.1; // -0.05 to 0.05
        let n = normalized + noise;
        n = Math.max(0, Math.min(1, n));
        return min + n * range;
    }

    return (
        //Math.sin(time / 1000 + index) * 50 + Math.random() * 10 + 50 + index * 20 * (1 + time / 100000)
        Math.sin(time / 1000 + index) * 50 + Math.random() * 10 + 50 + index * 20
        //val
        //Math.random() * (time % 10000) * (index * 1000)
        // 50 + Math.random() * 10 + 50 + index * 20 * time
    );
}

const MOCK_STATES = ["IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING"];
function generateRandomEnumValue() {
    return MOCK_STATES[Math.floor(Math.random() * MOCK_STATES.length)];
}

export function MockWidgetAddSignalModal({ closeModal, configs, updateWidget, dataRef, widgetData }: {
    closeModal: () => void,
    updateWidget: (updater: (prevWidget: WidgetData) => WidgetData) => void,

    // read only
    configs: WidgetConfigs,
    dataRef: RefObject<unknown>,
    widgetData: WidgetData,
}) {
    const [newSignalName, setNewSignalName] = useState("");
    const [newSignalType, setNewSignalType] = useState<SignalType>(SignalType.NUMERICAL);
    const [newSignalDelay, setNewSignalDelay] = useState(100);
    const [newSignalMin, setNewSignalMin] = useState<number>(-10);
    const [newSignalMax, setNewSignalMax] = useState<number>(10);

    const handleAddSignal = useCallback((e: SubmitEvent<HTMLFormElement>) => {
        e.preventDefault();
        const name = newSignalName.trim() || `Signal ${configs.length + 1}`;
        if (configs.some((c) => c.signal_name === name)) {
            alert("Signal name must be unique in this graph");
            return;
        }
        if (newSignalMin >= newSignalMax) {
            alert("Min value must be less than max value");
            return;
        }

        // TODO
        // const newConfig: MockGraphConfig = {
        //     signal_name: name,
        //     type: newSignalType,
        //     delay: newSignalDelay,
        //     initialPoints: 0,
        //     min: newSignalType === SignalType.NUMERICAL ? newSignalMin : undefined,
        //     max: newSignalType === SignalType.NUMERICAL ? newSignalMax : undefined,
        // };
        // const store = dataRef.current;
        // store.series[name] = new Array(store.timestamps.length).fill(null);
        updateWidget((prev) => {
            // if (prev.type === SignalType.MOCK) {
            //     return { ...prev, configs: [...prev.configs, newConfig] };
            // }
            return prev;
        });
        closeModal();
    }, [configs, newSignalName, newSignalType, newSignalDelay, updateWidget, widgetData.id, newSignalMin, newSignalMax]);

    return (
        <>
            <DialogHeader>
                <DialogTitle className="text-lg font-bold mb-4">Add Mock Signal</DialogTitle>
                <DialogDescription>
                    Configure a new signal to be generated in this mock graph.
                </DialogDescription>
            </DialogHeader>
            <form onSubmit={handleAddSignal} className="space-y-4">
                <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                        Signal Name
                    </label>
                    <input
                        type="text"
                        value={newSignalName}
                        onChange={(e) => setNewSignalName(e.target.value)}
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        placeholder="e.g. Voltage"
                        autoFocus
                    />
                </div>
                <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                        Type
                    </label>
                    <select
                        value={newSignalType}
                        onChange={(e) =>
                            setNewSignalType(e.target.value as SignalType)
                        }
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                    >
                        <option value={SignalType.NUMERICAL}>
                            Numerical
                        </option>
                        <option value={SignalType.ENUM}>
                            Enumeration
                        </option>
                    </select>
                </div>
                {newSignalType === SignalType.NUMERICAL && (
                    <div className="flex gap-4">
                        <div className="flex-1">
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                                Min Value
                            </label>
                            <input
                                type="number"
                                value={newSignalMin}
                                onChange={(e) => setNewSignalMin(Number(e.target.value))}
                                className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                            />
                        </div>
                        <div className="flex-1">
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                                Max Value
                            </label>
                            <input
                                type="number"
                                value={newSignalMax}
                                onChange={(e) => setNewSignalMax(Number(e.target.value))}
                                className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                            />
                        </div>
                    </div>
                )}
                <div>
                    <label className="block text-sm font-medium text-gray-700 mb-1">
                        Update Delay (ms)
                    </label>
                    <input type="number" value={newSignalDelay}
                        onChange={(e) => setNewSignalDelay(Number(e.target.value))}
                        className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                    />
                </div>
                <div className="flex justify-end gap-2 pt-2">
                    <button type="button" onClick={closeModal} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
                        Cancel
                    </button>
                    <button type="submit" className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded cursor-pointer">
                        Add
                    </button>
                </div>
            </form>
        </>
    );
}

export function useMockData(
    isPaused: boolean,
    widgetData: WidgetData,
): RefObject<ChartData> {
    const { setTimeRange, timeRangeRef } = useSyncedGraph(); // because this widget is also doing the data generation
    // const dataRef = useRef<Record<string, SeriesData<number | string>>>({});
    const dataRef = useRef({
        type: widgetData.type,
        all_series: []
    } satisfies ChartData);

    useEffect(() => {
        if (isPaused) return;
        const intervals: ReturnType<typeof setInterval>[] = widgetData.configs.map((cfg, idx) => {
            const interval = setInterval(() => {
                const store = dataRef.current;
                if (!store[cfg.signal_name]) {
                    store[cfg.signal_name] = {
                        timestamps: [],
                        values: []
                    };
                }
                const at = store[cfg.signal_name]

                const now = Date.now();
                at.timestamps.push(now);
                if (at.timestamps.length > 0) { // update timeRanges (i feel like this needs to be handled elsewhere...)
                    // TODO this cannot be the best interface for this
                    setTimeRange({
                        min: Math.min(at.timestamps[0], timeRangeRef.current?.min ?? at.timestamps[0]),
                        max: Math.max(at.timestamps[at.timestamps.length - 1], timeRangeRef.current?.max ?? at.timestamps[at.timestamps.length - 1])
                    });
                }
                if (widgetData.type == SignalType.NUMERICAL) {
                    at.values.push(generateRandomNumericalValue(now, idx, (cfg as NumericalSignalConfig).min, (cfg as NumericalSignalConfig).max));
                } else {
                    at.values.push(generateRandomEnumValue());
                }
            }, cfg.delay);
            return interval;
        });
        return () => intervals.forEach(clearInterval);
    }, [widgetData, isPaused]);

    return dataRef;
}