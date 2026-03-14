"use client";
// react
import { useEffect, useState, useRef, useCallback, RefObject, SubmitEvent } from "react";
// types
import { EnumSignalConfig, NumericalSignalConfig, WidgetConfigs, WidgetData } from "@/components/widgets/WidgetTypes";
import { SignalType } from "@/lib/types/Signal";
import { ChartData, ChartDataEnum, ChartDataNumerical, EnumSeries, NumericalSeries } from "./CanvasChartTypes";
import { SeriesData } from "@/lib/seriesData";
import chroma from "chroma-js";
// hooks
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useWidgetManager } from "./WidgetManagerContext";
// components
import { Dialog, DialogDescription, DialogHeader, DialogTitle, DialogContent, DialogTrigger } from "@/components/ui/dialog";
import { PlusButton } from "@/components/icons/PlusButton";

export const MOCK_STATES = [ // needed to hardcode for widgetadder
    "IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING", "SKIBIDI", 
    "TOILET", "MORE", "SIGNALS", "TO", "TEST", "RANDOM", 
    "ENUM", "COLOR", "GEN"
];

export function generateRandomColorPalette(count: number = MOCK_STATES.length) {
    const normalizedCount = Math.max(1, Math.floor(count));
    const startingHue = Math.random() * 360;
    const hueStep = 360 / normalizedCount;

    return Array.from({ length: normalizedCount }, (_, idx) => {
        const hue = (startingHue + idx * hueStep) % 360;
        return chroma.hsl(hue, 0.72, 0.56);
    });
}

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
        Math.sin(time / 1000 + index) * 50 + Math.random() * 10 + 50 + index * 20
    );
}

function generateRandomEnumValue() {
    const v = Math.floor(Math.random() * MOCK_STATES.length);
    return {
        name: MOCK_STATES[v],
        idx: v,
    };
}

function MockSignalModalForm({ closeModal, configs, dataRef, widgetData }: {
    closeModal: () => void,
    configs: WidgetConfigs,
    dataRef: RefObject<ChartData>,
    widgetData: WidgetData,
}) {
    const [newSignalName, setNewSignalName] = useState("");
    const [newSignalDelay, setNewSignalDelay] = useState(100);
    const [newSignalMin, setNewSignalMin] = useState<number>(-10);
    const [newSignalMax, setNewSignalMax] = useState<number>(10);

    const { appendSignal } = useWidgetManager();

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

        if (widgetData.type === SignalType.NUMERICAL) {
            (dataRef.current as ChartDataNumerical).all_series.push({ label: name, timestamps: [], data: new SeriesData(), });
            appendSignal(widgetData.id, {
                delay: newSignalDelay,
                min: newSignalMin, max: newSignalMax,
                signal_name: name,
                color: chroma.random()
            } satisfies NumericalSignalConfig);
        }
        else {
            (dataRef.current as ChartDataEnum).all_series.push({ label: name, timestamps: [], data: [], enumValuesToNames: {} });
            appendSignal(widgetData.id, {
                signal_name: name, delay: newSignalDelay, initialPoints: 0, options: {
                    colorPalette: generateRandomColorPalette(MOCK_STATES.length)
                }, color: chroma.random()
            } satisfies EnumSignalConfig);
        }
        closeModal();
    }, [configs, appendSignal, dataRef, widgetData, newSignalName, newSignalDelay, newSignalMin, newSignalMax]);

    return (
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
                <div className="w-full border rounded px-3 py-2 text-gray-900 bg-gray-50">
                    {widgetData.type === SignalType.NUMERICAL ? "Numerical" : "Enumeration"}
                </div>
            </div>
            {widgetData.type === SignalType.NUMERICAL && (
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
    )
}

export function MockWidgetAddSignalModal({ configs, dataRef, widgetData }: {
    configs: WidgetConfigs;
    widgetData: WidgetData;
    dataRef: RefObject<ChartData>;
}) {
    const [modalOpen, setModalOpen] = useState(false);
    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger>
                <PlusButton />
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Add Mock Signal</DialogTitle>
                    <DialogDescription>
                        Configure a new signal to be generated in this mock graph.
                    </DialogDescription>
                </DialogHeader>
                <MockSignalModalForm closeModal={() => setModalOpen(false)} configs={configs} dataRef={dataRef} widgetData={widgetData} />
            </DialogContent>
        </Dialog>
    );
}

export function useMockData(isPaused: boolean, widgetData: WidgetData,): RefObject<ChartData> {
    const dataRef = useRef<ChartData>({ type: widgetData.type, all_series: [] });
    const { updateWithTimestamp } = useSyncedGraph();

    useEffect(() => {
        if (isPaused) return;
        const intervals: ReturnType<typeof setInterval>[] = widgetData.signals.map((sig_cfg, idx) => setInterval(() => {
            const rn = Date.now();
            if (widgetData.type === SignalType.NUMERICAL) {
                // cast is legal due to inference on widgetData.type
                let series = (dataRef.current as ChartDataNumerical).all_series.find((s) => s.label === sig_cfg.signal_name);
                if (!series) {
                    series = {
                        label: sig_cfg.signal_name, timestamps: [], data: new SeriesData()
                    } satisfies NumericalSeries;
                    (dataRef.current as ChartDataNumerical).all_series.push(series);
                }
                series.timestamps.push(rn);
                series.data.push(generateRandomNumericalValue(rn, idx, (sig_cfg as NumericalSignalConfig).min, (sig_cfg as NumericalSignalConfig).max));
            } else {
                // cast is legal due to inference on widgetData.type
                let series = (dataRef.current as ChartDataEnum).all_series.find((s) => s.label === sig_cfg.signal_name);
                if (!series) {
                    series = {
                        label: sig_cfg.signal_name,
                        timestamps: [],
                        data: [],
                        enumValuesToNames: {}
                    } satisfies EnumSeries;
                    (dataRef.current as ChartDataEnum).all_series.push(series);
                }
                series.timestamps.push(rn);
                const vals = generateRandomEnumValue();
                series.data.push(vals.idx);
                if (!series.enumValuesToNames[vals.idx]) {
                    series.enumValuesToNames[vals.idx] = [vals.name];
                }
            }
            updateWithTimestamp(rn);
        }, sig_cfg.delay));
        return () => intervals.forEach(clearInterval);
    }, [widgetData, isPaused]);

    return dataRef;
}