"use client";

import chroma from "chroma-js";
import { Plus } from "lucide-react";
import { useState } from "react";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { ChartableSignalMetadata, isChartableSignal, isStateSignal, SignalPicker } from "@/components/widgets/SignalPicker";
import { buildEnumPalette } from "@/components/widgets/Widget";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { isNumericalSignalMetadata } from "@/lib/types/Signal";

const DEFAULT_OPTIONS = { height: 256, timeTickCount: 6 };

export function WidgetAdder() {
    const [open, setOpen] = useState(false);
    const { appendWidget } = useWidgetManager();

    // checked graph signals share one new chart and checked state signals another, like shift-picking did before
    const addCharts = (signals: ChartableSignalMetadata[]) => {
        const numerical = signals.filter(isNumericalSignalMetadata);
        const states = signals.filter(isStateSignal);

        if (numerical.length > 0) {
            appendWidget({ id: "", type: "numericalGraph", data: [], signals: numerical, options: { ...DEFAULT_OPTIONS, colorPalette: Object.fromEntries(numerical.map((signal) => [signal.name, chroma.random()])) } });
        }
        if (states.length > 0) {
            appendWidget({ id: "", type: "enumTimeline", data: [], signals: states, options: { ...DEFAULT_OPTIONS, colorPalette: Object.fromEntries(states.map((signal) => [signal.name, buildEnumPalette(signal)])) } });
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
                    <DialogDescription className="text-base text-gray-700">Check the signals to chart. Graph signals go on one chart and state signals on another.</DialogDescription>
                </DialogHeader>
                <SignalPicker accept={isChartableSignal} onAdd={addCharts} />
            </DialogContent>
        </Dialog>
    );
}
