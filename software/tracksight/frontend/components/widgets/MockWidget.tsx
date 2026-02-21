"use client";

import { useEffect, useState, useMemo, useRef, memo, FormEvent, useCallback, RefObject } from "react";
import CanvasChart, { AlignedData } from "@/components/widgets/CanvasChart";
import { useDisplayControl } from "@/components/PausePlayControl";
import { PlusButton } from "@/components/icons/PlusButton";
import { MockGraphConfig, WidgetDataMock, WidgetData, MockSignalType } from "@/lib/types/Widget";
import { signalColors } from "@/components/widgets/signalColors";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { SignalType } from "@/lib/types/Signal";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog"

function generateRandomValue(type: MockSignalType, time: number, index: number = 0, min?: number, max?: number) {
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
}

// TODO move this around
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
                className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none"
                title="Remove signal"
            >
                ×
            </button>
        </div>
    );
}

function MockWidgetModal({ closeModal, configs, dataRef, updateWidget, widgetData }: {
    closeModal: () => void,
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
        if (newSignalMin >= newSignalMax) {
            alert("Min value must be less than max value");
            return;
        }

        const newConfig: MockGraphConfig = {
            signalName: name,
            type: newSignalType,
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

const MockWidget = memo(({ widgetData, updateWidget, deleteSelfWidget }: {
    widgetData: WidgetDataMock,
    updateWidget: (widgetId: string, updater: (prevWidget: WidgetData) => WidgetData) => void,
    deleteSelfWidget: () => void
}) => {
    const { isPaused } = useDisplayControl();
    const configs = widgetData.configs;
    const [modalOpen, setModalOpen] = useState(false);
    const [chartHeight, setChartHeight] = useState(256);
    const { setTimeRange, timeRangeRef } = useSyncedGraph(); // because this widget is also doing the data generation
    const dataRef = useRef<{
        timestamps: number[];
        series: Record<string, (number | string | null)[]>;
    }>({ timestamps: [], series: {} });

    // TODO: could add initialPoints loop here (but this is very optional)
    // it'd just be what we use in MockGraph.tsx

    // generate fuck ass mock data here
    useEffect(() => {
        if (isPaused) return;
        const intervals: ReturnType<typeof setInterval>[] = configs.map((cfg, idx) => {
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
            return interval;
        });
        return () => intervals.forEach(clearInterval);
    }, [configs, isPaused]);

    const chartData = useMemo<AlignedData>(() => {
        const { timestamps, series } = dataRef.current;
        configs.forEach((c) => {
            if (!series[c.signalName]) {
                series[c.signalName] = new Array(timestamps.length).fill(null);
            }
        });
        return {
            timestamps,
            series: configs.map((c) => series[c.signalName]),
        }
    }, [configs, isPaused]);

    const handleRemoveSignal = useCallback((name: string) => {
        if (configs.length <= 1) { // delete the whole widget
            deleteSelfWidget();
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
    }, [configs, deleteSelfWidget, updateWidget, widgetData.id]);

    return (
        <div className="w-full border-red-500 py-4">
            <div className="px-6">
                {/* header */}
                <div className="flex items-center gap-2 mb-4">
                    <h3 className="font-semibold">Mock Graph Container (Widget)</h3>
                    <button onClick={deleteSelfWidget} title="Remove mock graph"
                        className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
                    >
                        ×
                    </button>
                </div>

                {/* TODO remove, keep for debugging for now */}
                {/* <label className="text-sm block">Vertical Scale: {chartHeight}px</label>
                <input type="range" min={100} max={600} step={50} value={chartHeight}
                    onChange={(e) => setChartHeight(+e.target.value)}
                /> */}

                {/* Signal buttons */}
                <div className="flex flex-wrap items-center gap-3">
                    {configs.map((cfg, idx) =>
                        <SignalButton key={cfg.signalName} cfg={cfg} idx={idx} handleRemoveSignal={handleRemoveSignal} onHover={() => { }} onHoverEnd={() => { }} />
                    )}
                    <Dialog open={modalOpen} onOpenChange={setModalOpen}>
                        <DialogTrigger>
                            <PlusButton />
                        </DialogTrigger>
                        <DialogContent>
                            <MockWidgetModal configs={configs} dataRef={dataRef} updateWidget={updateWidget} widgetData={widgetData} closeModal={() => setModalOpen(false)} />
                        </DialogContent>
                    </Dialog>
                </div>
            </div>

            {/* Actual chart */}
            {/* <div style={{ height: chartHeight }}> */}
            <CanvasChart data={chartData} height={chartHeight}
                series={configs.map((c, idx) => ({
                    label: c.signalName, color: signalColors[idx % signalColors.length],
                    min: c.min, max: c.max,
                }))}
            />
            {/* </div> */}
        </div >
    );
});

MockWidget.displayName = "MockWidget";

export default MockWidget;

