"use client";

import chroma from "chroma-js";
import { Plus } from "lucide-react";
import { useRef, useState } from "react";
import { v4 as uuidv4 } from "uuid";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { ChartableSignalMetadata, isChartableSignal, SignalPicker } from "@/components/widgets/SignalPicker";
import { buildEnumPalette } from "@/components/widgets/Widget";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { isNumericalSignalMetadata } from "@/lib/types/Signal";
import { EnumTimelineWidgetData, NumericalGraphWidgetData } from "@/lib/types/Widget";

const DEFAULT_OPTIONS = { height: 256, timeTickCount: 6 };

type ChartKind = "numerical" | "state";
const kindOf = (signal: ChartableSignalMetadata): ChartKind => (isNumericalSignalMetadata(signal) ? "numerical" : "state");

export function WidgetAdder() {
    const [open, setOpen] = useState(false);
    const { widgets, appendWidget, updateWidget, removeWidget } = useWidgetManager();
    // charts made while the dialog is open: shift-picks of the same kind go onto the same chart. a ref, not state,
    // because "add all" adds several signals before React re-renders
    const buildingRef = useRef<Partial<Record<ChartKind, string>>>({});

    const building = widgets.filter((widget) => widget.id === buildingRef.current.numerical || widget.id === buildingRef.current.state);
    const addedSignalNames = building.flatMap((widget) => widget.signals.map((signal) => signal.name));

    const add = (signal: ChartableSignalMetadata) => {
        const kind = kindOf(signal);
        const id = buildingRef.current[kind];

        if (isNumericalSignalMetadata(signal)) {
            if (id) {
                updateWidget({ id } as NumericalGraphWidgetData, (previous) => ({ ...previous, signals: [...previous.signals, signal], options: { ...previous.options, colorPalette: { ...previous.options.colorPalette, [signal.name]: chroma.random() } } }));
                return;
            }
            const newId = uuidv4();
            appendWidget({ id: newId, type: "numericalGraph", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: chroma.random() } } });
            buildingRef.current[kind] = newId;
            return;
        }

        if (id) {
            updateWidget({ id } as EnumTimelineWidgetData, (previous) => ({ ...previous, signals: [...previous.signals, signal], options: { ...previous.options, colorPalette: { ...previous.options.colorPalette, [signal.name]: buildEnumPalette(signal) } } }));
            return;
        }
        const newId = uuidv4();
        appendWidget({ id: newId, type: "enumTimeline", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: buildEnumPalette(signal) } } });
        buildingRef.current[kind] = newId;
    };

    const remove = (signal: ChartableSignalMetadata) => {
        const kind = kindOf(signal);
        const chart = building.find((widget) => widget.id === buildingRef.current[kind]);
        if (!chart) return;

        // taking off the only signal takes the chart away too, rather than leaving an empty one behind
        if (chart.signals.length === 1) {
            removeWidget(chart.id);
            delete buildingRef.current[kind];
            return;
        }

        const withoutSignal = <P extends Record<string, unknown>>(palette: P) => Object.fromEntries(Object.entries(palette).filter(([name]) => name !== signal.name)) as P;
        if (chart.type === "numericalGraph") {
            updateWidget(chart, (previous) => ({ ...previous, signals: previous.signals.filter((existing) => existing.name !== signal.name), options: { ...previous.options, colorPalette: withoutSignal(previous.options.colorPalette) } }));
        } else {
            updateWidget(chart, (previous) => ({ ...previous, signals: previous.signals.filter((existing) => existing.name !== signal.name), options: { ...previous.options, colorPalette: withoutSignal(previous.options.colorPalette) } }));
        }
    };

    const setDialogOpen = (nextOpen: boolean) => {
        if (!nextOpen) buildingRef.current = {};
        setOpen(nextOpen);
    };

    return (
        <Dialog open={open} onOpenChange={setDialogOpen}>
            <DialogTrigger asChild>
                <button type="button" className="inline-flex items-center gap-2 rounded border border-blue-600 bg-blue-600 px-4 py-2 text-sm font-semibold text-white hover:bg-blue-700 focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600">
                    <Plus className="size-4" /> Add chart
                </button>
            </DialogTrigger>
            <DialogContent className="max-w-2xl">
                <DialogHeader>
                    <DialogTitle className="text-xl">Add chart</DialogTitle>
                    <DialogDescription className="text-base text-gray-700">Pick a signal. Hold Shift to put several on one chart.</DialogDescription>
                </DialogHeader>
                <SignalPicker
                    accept={isChartableSignal}
                    addedSignalNames={addedSignalNames}
                    onPick={(signal, keepOpen) => {
                        add(signal);
                        if (!keepOpen) setDialogOpen(false);
                    }}
                    onRemove={remove}
                    onPickAll={(signals) => {
                        signals.forEach(add);
                        setDialogOpen(false);
                    }}
                />
            </DialogContent>
        </Dialog>
    );
}
