"use client";
// react
import { useEffect, useState, useRef, useCallback, RefObject, SubmitEvent } from "react";
// types
import { EnumSignalConfig, NumericalSignalConfig, WidgetConfigs, WidgetData, WidgetDataNumerical } from "@/components/widgets/WidgetTypes";
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

function NumericalSignalModalForm ({ closeModal, configs, dataRef, widgetData }: {
    closeModal: () => void;
    configs: NumericalSignalConfig[];
    widgetData: WidgetDataNumerical;
    dataRef: RefObject<ChartData>;
}) {
    const [signalName, setSignalName] = useState("");
    const { appendSignal } = useWidgetManager();

    const handleAddSignal = (e: SubmitEvent<HTMLFormElement>) => {
        e.preventDefault();

        if (!dataRef.current || dataRef.current.type !== SignalType.NUMERICAL) {
            alert("Numerical chart data is not initialized yet");
            return;
        }

        const newMin = Math.floor(Math.random() * 20); // TODO : get metadata based off of signal
        const newMax = Math.floor(100 + Math.floor(Math.random() * 50)); // TODO : get metadata based off of signal
        const delay = Math.floor(Math.random() * 100); // TODO : get metadata based off of signal

        if (configs.some((c) => c.signal_name === signalName)) {
            alert("Already listening to this signal");
            return;
        }

        dataRef.current.all_series.push({ label: signalName, timestamps: [], data: new SeriesData(), });
        appendSignal(widgetData.id, {
            delay: delay,
            min: newMin, max: newMax,
            signal_name: signalName,
            color: chroma.random() // todo pallete mayb
        } satisfies NumericalSignalConfig);
        closeModal();
    };

    return (
        <form onSubmit={handleAddSignal}>
            <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                    Signal Name
                </label>
                <input
                    type="text"
                    value={signalName}
                    onChange={(e) => setSignalName(e.target.value)}
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
        </form>);
};

export function NumericalWidgetAddSignalModal({ configs, dataRef, widgetData }: {
    configs: NumericalSignalConfig[];
    widgetData: WidgetDataNumerical;
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
                    <DialogTitle className="text-lg font-bold mb-4">Add Numerical Signal</DialogTitle>
                    <DialogDescription>
                        Configure a new signal to be generated in this numerical graph.
                    </DialogDescription>
                </DialogHeader>
                <NumericalSignalModalForm closeModal={() => setModalOpen(false)} configs={configs} dataRef={dataRef} widgetData={widgetData} />
            </DialogContent>
        </Dialog>
    );
}