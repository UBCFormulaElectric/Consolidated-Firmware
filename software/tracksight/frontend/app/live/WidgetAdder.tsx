"use client";

import chroma from "chroma-js";
import { Plus } from "lucide-react";
import { useState } from "react";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { ChartableSignalMetadata, isChartableSignal, SignalPicker } from "@/components/widgets/SignalPicker";
import { buildEnumPalette } from "@/components/widgets/Widget";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { isNumericalSignalMetadata } from "@/lib/types/Signal";

const DEFAULT_OPTIONS = { height: 256, timeTickCount: 6 };

export function WidgetAdder() {
    const [open, setOpen] = useState(false);
    const { appendWidget } = useWidgetManager();

    const addChart = (signal: ChartableSignalMetadata) => {
        if (isNumericalSignalMetadata(signal)) {
            appendWidget({ id: "", type: "numericalGraph", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: chroma.random() } } });
        } else {
            appendWidget({ id: "", type: "enumTimeline", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: buildEnumPalette(signal) } } });
        }
        setOpen(false);
    };

    return (
        <Dialog open={open} onOpenChange={setOpen}>
            <DialogTrigger asChild>
                <button type="button" className="inline-flex items-center gap-2 rounded border border-blue-600 bg-blue-600 px-4 py-2 text-sm font-semibold text-white hover:bg-blue-700 focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600">
                    <Plus className="size-4" /> Add chart
                </button>
            </DialogTrigger>
            <DialogContent className="max-w-2xl">
                <DialogHeader>
                    <DialogTitle className="text-xl">Add chart</DialogTitle>
                    <DialogDescription className="text-base text-gray-700">Pick a signal; the chart type is chosen automatically.</DialogDescription>
                </DialogHeader>
                <SignalPicker accept={isChartableSignal} onPick={addChart} />
            </DialogContent>
        </Dialog>
    );
}
