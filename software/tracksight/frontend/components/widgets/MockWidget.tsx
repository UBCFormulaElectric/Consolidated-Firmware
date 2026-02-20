"use client";

import { useEffect, useState, useMemo, useRef, memo, FormEvent, useCallback, RefObject } from "react";
import CanvasChart, { AlignedData } from "@/components/widgets/CanvasChart";
import { usePausePlay } from "@/components/PausePlayControl";
import { PlusButton } from "@/components/PlusButton";
import { MockGraphConfig, WidgetDataMock, WidgetData, MockSignalType } from "@/lib/types/Widget";
import { signalColors } from "@/components/widgets/signalColors";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalType } from "@/lib/types/Signal";
import {
    Dialog,
    DialogContent,
    DialogDescription,
    DialogHeader,
    DialogTitle,
    DialogTrigger,
} from "@/components/ui/dialog"

const generateRandomValue = (
    type: MockSignalType,
    time: number,
    index: number = 0,
    min?: number,
    max?: number
) => {
    if (type === MockSignalType.Enumeration) {
        const states = ["IDLE", "ACTIVE", "ERROR", "WAITING", "CHARGING"];
        return states[Math.floor(Math.random() * states.length)];
    } else {
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
};

function SignalButton({ cfg, idx, handleRemoveSignal, onHover, onHoverEnd }: {
    cfg: MockGraphConfig, idx: number,
    handleRemoveSignal: (signalName: string) => void,
    onHover: (signalName: string) => void,
    onHoverEnd: () => void,
}) {
    const color = signalColors[idx % signalColors.length];
    // TODO hover highlights the signal in graph
    return (
        <div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity"
            style={{ backgroundColor: color + "20", borderColor: color + "80" }}
            onMouseEnter={() => onHover(cfg.signalName)} onMouseLeave={() => onHoverEnd()}
        >
            <div className="w-3 h-3 rounded-full" style={{ backgroundColor: color }} />
            <span className="font-medium">{cfg.signalName}</span>
            <span className="text-xs text-gray-500">({cfg.delay}ms)</span>
            <button
                onClick={() => handleRemoveSignal(cfg.signalName)}
                className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer"
                title="Remove signal"
            >
                ×
            </button>
        </div>
    );
}

function BruhModal({ setShowAddModal, configs, dataRef, updateWidget, widgetData }: {
    // writes
    setShowAddModal: (show: boolean) => void,
    updateWidget: (widgetId: string, updater: (prevWidget: WidgetData) => WidgetData) => void,

    // read only
    configs: MockGraphConfig[],
    dataRef: RefObject<{
        timestamps: number[];
        series: Record<string, (number | string | null)[]>;
    }>,
    widgetData: WidgetDataMock,
}) {
    const [newSignalName, setNewSignalName] = useState("");
    const [newSignalType, setNewSignalType] = useState<MockSignalType>(MockSignalType.Numerical);
    const [newSignalDelay, setNewSignalDelay] = useState(100);
    const [newSignalMin, setNewSignalMin] = useState<number>(-10);
    const [newSignalMax, setNewSignalMax] = useState<number>(10);

    const handleAddSignal = useCallback((e: FormEvent) => {
        e.preventDefault();
        const name = newSignalName.trim() || `Signal ${configs.length + 1}`;
        if (configs.some((c) => c.signalName === name)) {
            alert("Signal name must be unique in this graph");
            return;
        }

        const newConfig: MockGraphConfig = {
            signalName: name,
            type: newSignalType, // "numerical" | "enumeration"
            delay: newSignalDelay,
            initialPoints: 0,
            min: newSignalType === MockSignalType.Numerical ? newSignalMin : undefined,
            max: newSignalType === MockSignalType.Numerical ? newSignalMax : undefined,
        };

        const store = dataRef.current;
        store.series[name] = new Array(store.timestamps.length).fill(null);

        updateWidget(widgetData.id, (prev) => {
            if (prev.type === SignalType.MOCK) {
                return { ...prev, configs: [...prev.configs, newConfig] };
            }
            return prev;
        });

        setShowAddModal(false);
        setNewSignalName("");
        setNewSignalType(MockSignalType.Numerical);
        setNewSignalMin(-10);
        setNewSignalMax(10);
    }, [configs, newSignalName, newSignalType, newSignalDelay, updateWidget, widgetData.id, newSignalMin, newSignalMax]);

    return (
        <div
            className="fixed inset-0 bg-black/50 flex items-center justify-center z-100"
            onClick={() => setShowAddModal(false)}
        >
            <div
                className="bg-white rounded-lg shadow-xl p-6 w-96 max-w-full m-4"
                onClick={(e) => e.stopPropagation()}
            >
                <h3 className="text-lg font-bold mb-4">Add Mock Signal</h3>
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
                                setNewSignalType(e.target.value as MockSignalType)
                            }
                            className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        >
                            <option value={MockSignalType.Numerical}>
                                Numerical
                            </option>
                            <option value={MockSignalType.Enumeration}>
                                Enumeration
                            </option>
                        </select>
                    </div>
                    {newSignalType === MockSignalType.Numerical && (
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
                        <input
                            type="number"
                            value={newSignalDelay}
                            onChange={(e) =>
                                setNewSignalDelay(Number(e.target.value))
                            }
                            className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                        />
                    </div>
                    <div className="flex justify-end gap-2 pt-2">
                        <button
                            type="button"
                            onClick={() => setShowAddModal(false)}
                            className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded"
                        >
                            Cancel
                        </button>
                        <button
                            type="submit"
                            className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded"
                        >
                            Add
                        </button>
                    </div>
                </form>
            </div>
        </div>
    );
}

const MockWidget = memo(({ widgetData, updateWidget, onDelete }: {
    widgetData: WidgetDataMock,
    updateWidget: (widgetId: string, updater: (prevWidget: WidgetData) => WidgetData) => void,
    onDelete: () => void
}) => {
    const { isPaused } = usePausePlay();
    const configs = widgetData.configs;

    const [showAddModal, setShowAddModal] = useState(false);

    const dataRef = useRef<{
        timestamps: number[];
        series: Record<string, (number | string | null)[]>;
    }>({ timestamps: [], series: {} });

    const [chartHeight, setChartHeight] = useState(256);

    const { setTimeRange, timeRangeRef } = useSyncedGraph(); // because this widget is also doing the data generation

    // const graphId = widgetData.id;

    // TODO: could add initialPoints loop here (but this is very optional)
    // it'd just be what we use in MockGraph.tsx

    useEffect(() => {
        if (isPaused) return;

        const intervals: ReturnType<typeof setInterval>[] = [];

        configs.forEach((cfg, idx) => {
            const interval = setInterval(() => {
                const now = Date.now();
                const val = generateRandomValue(cfg.type, now, idx, cfg.min, cfg.max);

                const store = dataRef.current;

                configs.forEach((c) => {
                    if (!store.series[c.signalName]) {
                        store.series[c.signalName] = new Array(store.timestamps.length).fill(null);
                    }
                });

                store.timestamps.push(now);

                if (store.timestamps.length > 0) {
                    setTimeRange({
                        min: Math.min(store.timestamps[0], timeRangeRef.current?.min ?? store.timestamps[0]),
                        max: Math.max(store.timestamps[store.timestamps.length - 1], timeRangeRef.current?.max ?? store.timestamps[store.timestamps.length - 1])
                    });
                }

                Object.keys(store.series).forEach((key) => {
                    if (key === cfg.signalName) {
                        store.series[key].push(val);
                    } else {
                        const arr = store.series[key];
                        const lastVal = arr.length > 0 ? arr[arr.length - 1] : null;
                        arr.push(lastVal);
                    }
                });
            }, cfg.delay);
            intervals.push(interval);
        });

        return () => intervals.forEach(clearInterval);
    }, [configs, isPaused]);

    // TODO why is the render loop here?
    useEffect(() => {
        let frameId: number;
        const loop = () => {
            frameId = requestAnimationFrame(loop);
        };
        frameId = requestAnimationFrame(loop);
        return () => cancelAnimationFrame(frameId);
    }, [isPaused]);

    const chartData = useMemo<AlignedData>(() => {
        const { timestamps, series } = dataRef.current;

        configs.forEach((c) => {
            if (!series[c.signalName]) {
                series[c.signalName] = new Array(timestamps.length).fill(null);
            }
        });

        const seriesArrays = configs.map((c) => series[c.signalName]);
        // if (timestamps.length === 0) return { timestamps: [], series: [] };
        return {
            timestamps,
            series: seriesArrays,
        }
    }, [configs, isPaused]);

    const handleRemoveSignal = useCallback((name: string) => {
        if (configs.length <= 1) {
            onDelete();
            return;
        }
        updateWidget(widgetData.id, (prev) => {
            if (prev.type === SignalType.MOCK) {
                return { ...prev, configs: prev.configs.filter((c) => c.signalName !== name) };
            }
            return prev;
        });

        if (dataRef.current.series[name]) {
            delete dataRef.current.series[name];
        }
    }, [configs, onDelete, updateWidget, widgetData.id]);

    return (
        <div className="mb-6 p-4 block w-full relative border border-red-500">
            <div>
                {/* header */}
                <div className="flex items-center gap-2 mb-4">
                    <h3 className="font-semibold">Mock Graph Container (Widget)</h3>
                    <button onClick={onDelete} title="Remove mock graph"
                        className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
                    >
                        ×
                    </button>
                </div>

                {/* TODO remove, keep for debugging for now */}
                <label className="text-sm block">Vertical Scale: {chartHeight}px</label>
                <input type="range" min={100} max={600} step={50} value={chartHeight}
                    onChange={(e) => setChartHeight(+e.target.value)}
                />

                {/* Signal buttons */}
                <div className="flex flex-wrap items-center gap-3 mb-4">
                    {configs.map((cfg, idx) =>
                        <SignalButton key={cfg.signalName} cfg={cfg} idx={idx} handleRemoveSignal={handleRemoveSignal} onHover={() => { }} onHoverEnd={() => { }} />
                    )}
                    <div className="relative">
                        <PlusButton onClick={() => setShowAddModal(true)} variant="rowSide" />
                        {/* MODAL */}
                        {/* {showAddModal && <BruhModal setShowAddModal={setShowAddModal} configs={configs} dataRef={dataRef} updateWidget={updateWidget} widgetData={widgetData} />} */}
                        <Dialog>
                            <DialogTrigger>Open</DialogTrigger>
                            <DialogContent>
                                <DialogHeader>
                                    <DialogTitle>Are you absolutely sure?</DialogTitle>
                                    <DialogDescription>
                                        This action cannot be undone. This will permanently delete your account
                                        and remove your data from our servers.
                                    </DialogDescription>
                                </DialogHeader>
                            </DialogContent>
                        </Dialog>
                    </div>
                </div>
            </div>

            {/* Actual chart */}
            <div style={{ height: chartHeight }} className="overflow-hidden relative">
                <CanvasChart data={chartData} height={chartHeight}
                    series={configs.map((c, idx) => ({
                        label: c.signalName, color: signalColors[idx % signalColors.length],
                        min: c.min, max: c.max,
                    }))}
                />
            </div>
        </div >
    );
});

MockWidget.displayName = "MockWidget";

export default MockWidget;

