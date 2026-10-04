"use client";

import chroma, { Color } from "chroma-js";
import { ReactNode, RefObject, useCallback, useState } from "react";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { BooleanSignalMetadata, EnumSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { EnumTimelineWidgetData, NumericalGraphWidgetData, WidgetData } from "@/lib/types/Widget";
import EnumCanvasChart from "./EnumCanvasChart";
import NumericalCanvasChart from "./NumericalCanvasChart";
import { isStateSignal, SignalPicker } from "./SignalPicker";
import { useWidgetManager } from "./WidgetManagerContext";

export function buildEnumPalette(signal: EnumSignalMetadata | BooleanSignalMetadata): { color: Color; enumValueColors: Record<number, Color> } {
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

function WidgetConfiguration(props: { id: string; title: string; children?: ReactNode }) {
    const { id, title, children } = props;
    const { removeWidget } = useWidgetManager();

    const deleteSelfWidget = useCallback(() => {
        removeWidget(id);
    }, [id, removeWidget]);

    return (
        <div className="px-6">
            <div className="flex items-center gap-2 mb-4">
                <h3 className="truncate text-sm font-semibold" title={title}>
                    {title}
                </h3>
                <button type="button" onClick={deleteSelfWidget} className="shrink-0 rounded px-2 py-1 text-xs font-medium text-red-700 hover:bg-red-50 focus-visible:outline-2 focus-visible:outline-red-600">
                    Remove chart
                </button>
            </div>
            <div className="flex flex-wrap items-center gap-3">{children}</div>
        </div>
    );
}

function EmptyWidgetState(props: { message: string }) {
    return <div className="mx-6 mt-4 rounded-lg border border-dashed border-gray-300 bg-gray-50 px-6 py-12 text-center text-sm text-gray-500">{props.message}</div>;
}

function AddSignalModal<T extends SignalMetadata>(props: { title: string; description: string; accept: (signal: SignalMetadata) => signal is T; addedSignalNames: string[]; onAdd: (signal: T) => void; onRemove: (signal: T) => void }) {
    const { title, description, accept, addedSignalNames, onAdd, onRemove } = props;
    const [modalOpen, setModalOpen] = useState(false);

    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger asChild>
                <button type="button" className="rounded border border-blue-600 px-3 py-1.5 text-sm font-medium text-blue-700 hover:bg-blue-50 focus-visible:outline-2 focus-visible:outline-blue-600">
                    Add signal
                </button>
            </DialogTrigger>
            <DialogContent className="max-w-2xl">
                <DialogHeader>
                    <DialogTitle className="text-xl">{title}</DialogTitle>
                    <DialogDescription className="text-base text-gray-700">{description}</DialogDescription>
                </DialogHeader>
                <SignalPicker
                    accept={accept}
                    addedSignalNames={addedSignalNames}
                    onPick={(signal, keepOpen) => {
                        onAdd(signal);
                        if (!keepOpen) setModalOpen(false);
                    }}
                    onRemove={onRemove}
                    onPickAll={(signals) => {
                        signals.forEach(onAdd);
                        setModalOpen(false);
                    }}
                />
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
                    <WidgetConfiguration id={widget.id} title={widget.signals.map((signal) => signal.name).join(" · ") || "Numerical chart"}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name] ?? chroma("#ffffff")} />
                        ))}
                        <AddSignalModal
                            title="Add numerical signal"
                            description="Plot another numerical signal on this graph. Hold Shift to pick several."
                            accept={isNumericalSignalMetadata}
                            addedSignalNames={widget.signals.map((signal) => signal.name)}
                            onRemove={(signal) => handleRemoveSignal(signal.name)}
                            onAdd={(signal) =>
                                updateWidget(widget, (previousWidget) => ({
                                    ...previousWidget,
                                    signals: [...previousWidget.signals, signal],
                                    options: { ...previousWidget.options, colorPalette: { ...previousWidget.options.colorPalette, [signal.name]: chroma.random() } },
                                }))
                            }
                        />
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
                    <WidgetConfiguration id={widget.id} title={widget.signals.map((signal) => signal.name).join(" · ") || "State timeline"}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name]?.color ?? chroma("#ffffff")} />
                        ))}
                        <AddSignalModal
                            title="Add state signal"
                            description="Show another enum or on/off signal on this timeline. Hold Shift to pick several."
                            accept={isStateSignal}
                            addedSignalNames={widget.signals.map((signal) => signal.name)}
                            onRemove={(signal) => handleRemoveSignal(signal.name)}
                            onAdd={(signal) =>
                                updateWidget(widget, (previousWidget) => ({
                                    ...previousWidget,
                                    signals: [...previousWidget.signals, signal],
                                    options: { ...previousWidget.options, colorPalette: { ...previousWidget.options.colorPalette, [signal.name]: buildEnumPalette(signal) } },
                                }))
                            }
                        />
                    </WidgetConfiguration>
                    {widget.signals.length === 0 ? <EmptyWidgetState message="Add an enum signal to start the timeline." /> : <EnumCanvasChart {...widget} hoveredSignal={hoveredSignal} />}
                </>
            );
        }
    }
}
