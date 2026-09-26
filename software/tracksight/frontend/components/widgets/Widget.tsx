"use client";

import chroma, { Color } from "chroma-js";
import { ReactNode, RefObject, useCallback, useState } from "react";

import { EditButton } from "@/components/icons/EditButton";
import { PlusButton } from "@/components/icons/PlusButton";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { BooleanSignalMetadata, EnumSignalMetadata, isEnumSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { EnumTimelineWidgetData, NumericalGraphWidgetData, WidgetData } from "@/lib/types/Widget";
import EnumCanvasChart from "./EnumCanvasChart";
import { EnumSignalPicker } from "./EnumSignalPicker";
import NumericalCanvasChart from "./NumericalCanvasChart";
import { NumericalSignalPicker } from "./NumericalSignalPicker";
import { useWidgetManager } from "./WidgetManagerContext";

function buildEnumPalette(signal: EnumSignalMetadata | BooleanSignalMetadata): { color: Color; enumValueColors: Record<number, Color> } {
    const enumValues = isEnumSignalMetadata(signal) ? Object.values(signal.enum_signal.enum_values).sort((left, right) => left - right) : [0, 1];
    const startingHue = Math.random() * 360;
    const hueStep = enumValues.length > 0 ? 360 / enumValues.length : 360;
    const enumValueColors: Record<number, Color> = {};

    enumValues.forEach((enumValue, index) => {
        enumValueColors[enumValue] = chroma.hsl((startingHue + hueStep * index) % 360, 0.72, 0.56);
    });

    return {
        color: chroma.hsl(startingHue, 0.72, 0.56),
        enumValueColors,
    };
}

function removeColorPaletteEntry<T>(palette: Record<string, T>, signalName: string): Record<string, T> {
    const nextPalette = { ...palette };
    delete nextPalette[signalName];
    return nextPalette;
}

function SignalButton(props: { signal: SignalMetadata; handleRemoveSignal: (signalName: string) => void; hoverSignalName: RefObject<string | null>; color: Color }) {
    const { signal, handleRemoveSignal, hoverSignalName, color } = props;

    return (
        <div
            className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity cursor-crosshair"
            style={{ backgroundColor: color.brighten(1).hex(), borderColor: color.darken(1).hex() }}
            onMouseEnter={() => {
                hoverSignalName.current = signal.name;
            }}
            onMouseLeave={() => {
                hoverSignalName.current = null;
            }}
        >
            <div className="w-3 h-3 rounded-full" style={{ backgroundColor: color.hex() }} />
            <span className="font-medium">{signal.name}</span>
            {signal.cycle_time_ms !== null && <span className="text-xs text-gray-500">({signal.cycle_time_ms}ms)</span>}
            <button type="button" onClick={() => handleRemoveSignal(signal.name)} className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none" title="Remove signal">
                ×
            </button>
        </div>
    );
}

function WidgetConfiguration(props: { id: string; children?: ReactNode }) {
    const { id, children } = props;
    const { removeWidget } = useWidgetManager();

    const deleteSelfWidget = useCallback(() => {
        removeWidget(id);
    }, [id, removeWidget]);

    return (
        <div className="px-6">
            <div className="flex items-center gap-2 mb-4">
                <h3 className="font-semibold">Widget {id}</h3>
                <button type="button" onClick={deleteSelfWidget} title="Remove graph" className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer">
                    ×
                </button>
            </div>
            <div className="flex flex-wrap items-center gap-3">{children}</div>
        </div>
    );
}

function EmptyWidgetState(props: { message: string }) {
    return <div className="mx-6 mt-4 rounded-lg border border-dashed border-gray-300 bg-gray-50 px-6 py-12 text-center text-sm text-gray-500">{props.message}</div>;
}

function haveSameSignalNames(left: SignalMetadata[], right: SignalMetadata[]): boolean {
    const leftNames = new Set(left.map((signal) => signal.name));

    return left.length === right.length && right.every((signal) => leftNames.has(signal.name));
}

// NOTE: Keeps the existing colour for signals that stay on the widget so editing doesn't reshuffle the graph.
function buildNextColorPalette<S extends SignalMetadata, T>(previousPalette: Record<string, T>, signals: S[], createEntry: (signal: S) => T): Record<string, T> {
    return Object.fromEntries(signals.map((signal) => [signal.name, previousPalette[signal.name] ?? createEntry(signal)]));
}

function NumericalWidgetEditSignalsModal(props: { widget: NumericalGraphWidgetData }) {
    const { widget } = props;
    const [modalOpen, setModalOpen] = useState(false);
    const [selectedSignals, setSelectedSignals] = useState<NumericalSignalMetadata[]>([]);
    const { updateWidget } = useWidgetManager();

    const hasChanges = !haveSameSignalNames(widget.signals, selectedSignals);
    const hasNoSignals = widget.signals.length === 0;

    const handleOpenChange = (isOpen: boolean) => {
        setModalOpen(isOpen);
        setSelectedSignals(isOpen ? widget.signals : []);
    };

    const handleSave = (signals = selectedSignals) => {
        const nextColorPalette = buildNextColorPalette(widget.options.colorPalette, signals, () => chroma.random());

        updateWidget(widget, (previousWidget) => ({
            ...previousWidget,
            signals,
            options: {
                ...previousWidget.options,
                colorPalette: nextColorPalette,
            },
        }));
        handleOpenChange(false);
    };

    return (
        <Dialog open={modalOpen} onOpenChange={handleOpenChange}>
            <DialogTrigger asChild>
                <button type="button" className="cursor-pointer" title={hasNoSignals ? "Add numerical signals" : "Edit numerical signals"}>
                    {hasNoSignals ? <PlusButton /> : <EditButton />}
                </button>
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Edit Numerical Signals</DialogTitle>
                    <DialogDescription>Choose which live numerical signals this graph shows.</DialogDescription>
                </DialogHeader>
                <div className="min-w-0 space-y-4">
                    <NumericalSignalPicker selectedSignals={selectedSignals} onSelectedSignalsChange={setSelectedSignals} onConfirm={handleSave} />
                    <div className="flex justify-end gap-2 pt-2">
                        <button type="button" onClick={() => handleOpenChange(false)} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
                            Cancel
                        </button>
                        <button type="button" onClick={() => handleSave()} disabled={!hasChanges} className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded cursor-pointer disabled:cursor-not-allowed disabled:opacity-60">
                            Save
                        </button>
                    </div>
                </div>
            </DialogContent>
        </Dialog>
    );
}

function EnumWidgetEditSignalsModal(props: { widget: EnumTimelineWidgetData }) {
    const { widget } = props;
    const [modalOpen, setModalOpen] = useState(false);
    const [selectedSignals, setSelectedSignals] = useState<(EnumSignalMetadata | BooleanSignalMetadata)[]>([]);
    const { updateWidget } = useWidgetManager();

    const hasChanges = !haveSameSignalNames(widget.signals, selectedSignals);
    const hasNoSignals = widget.signals.length === 0;

    const handleOpenChange = (isOpen: boolean) => {
        setModalOpen(isOpen);
        setSelectedSignals(isOpen ? widget.signals : []);
    };

    const handleSave = (signals = selectedSignals) => {
        const nextColorPalette = buildNextColorPalette(widget.options.colorPalette, signals, buildEnumPalette);

        updateWidget(widget, (previousWidget) => ({
            ...previousWidget,
            signals,
            options: {
                ...previousWidget.options,
                colorPalette: nextColorPalette,
            },
        }));
        handleOpenChange(false);
    };

    return (
        <Dialog open={modalOpen} onOpenChange={handleOpenChange}>
            <DialogTrigger asChild>
                <button type="button" className="cursor-pointer" title={hasNoSignals ? "Add enum signals" : "Edit enum signals"}>
                    {hasNoSignals ? <PlusButton /> : <EditButton />}
                </button>
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Edit Enum Signals</DialogTitle>
                    <DialogDescription>Choose which live enum signals this timeline shows.</DialogDescription>
                </DialogHeader>
                <div className="min-w-0 space-y-4">
                    <EnumSignalPicker selectedSignals={selectedSignals} onSelectedSignalsChange={setSelectedSignals} onConfirm={handleSave} />
                    <div className="flex justify-end gap-2 pt-2">
                        <button type="button" onClick={() => handleOpenChange(false)} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
                            Cancel
                        </button>
                        <button type="button" onClick={() => handleSave()} disabled={!hasChanges} className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded cursor-pointer disabled:cursor-not-allowed disabled:opacity-60">
                            Save
                        </button>
                    </div>
                </div>
            </DialogContent>
        </Dialog>
    );
}

export function Widget(props: WidgetData & { hoveredSignal: RefObject<string | null> }) {
    const { hoveredSignal } = props;
    const { updateWidget } = useWidgetManager();

    switch (props.type) {
        case "numericalGraph": {
            const widget = props as NumericalGraphWidgetData;
            const handleRemoveSignal = (signalName: string) => {
                updateWidget(widget, (previousWidget) => ({
                    ...previousWidget,
                    signals: previousWidget.signals.filter((signal) => signal.name !== signalName),
                    options: {
                        ...previousWidget.options,
                        colorPalette: removeColorPaletteEntry(previousWidget.options.colorPalette, signalName),
                    },
                }));
            };

            return (
                <>
                    <WidgetConfiguration id={widget.id}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name] ?? chroma("#ffffff")} />
                        ))}
                        <NumericalWidgetEditSignalsModal widget={widget} />
                    </WidgetConfiguration>
                    {widget.signals.length === 0 ? <EmptyWidgetState message="Add a numerical signal to start graphing live data." /> : <NumericalCanvasChart {...widget} hoveredSignal={hoveredSignal} />}
                </>
            );
        }
        case "enumTimeline": {
            const widget = props as EnumTimelineWidgetData;
            const handleRemoveSignal = (signalName: string) => {
                updateWidget(widget, (previousWidget) => ({
                    ...previousWidget,
                    signals: previousWidget.signals.filter((signal) => signal.name !== signalName),
                    options: {
                        ...previousWidget.options,
                        colorPalette: removeColorPaletteEntry(previousWidget.options.colorPalette, signalName),
                    },
                }));
            };

            return (
                <>
                    <WidgetConfiguration id={widget.id}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name]?.color ?? chroma("#ffffff")} />
                        ))}
                        <EnumWidgetEditSignalsModal widget={widget} />
                    </WidgetConfiguration>
                    {widget.signals.length === 0 ? <EmptyWidgetState message="Add an enum signal to start the timeline." /> : <EnumCanvasChart {...widget} hoveredSignal={hoveredSignal} />}
                </>
            );
        }
    }
}
