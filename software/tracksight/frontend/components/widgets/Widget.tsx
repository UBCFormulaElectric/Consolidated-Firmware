"use client";

import chroma, { Color } from "chroma-js";
import { memo, ReactNode, RefObject, useCallback, useState } from "react";
import { useDrag } from "react-dnd";

import { EditButton } from "@/components/icons/EditButton";
import { PlusButton } from "@/components/icons/PlusButton";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { DRAGGABLE_TYPES } from "@/lib/constants";
import { BooleanSignalMetadata, EnumSignalMetadata, isEnumSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { EnumTimelineWidgetData, NumericalGraphWidgetData, WidgetData } from "@/lib/types/Widget";
import { GripVertical } from "lucide-react";
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

function WidgetConfiguration(props: { id: string; children?: ReactNode; dragHandle: React.RefObject<HTMLDivElement> }) {
    const { id, children, dragHandle } = props;
    const { removeWidget } = useWidgetManager();

    const deleteSelfWidget = useCallback(() => {
        removeWidget(id);
    }, [id, removeWidget]);

    return (
        <div className="px-6">
            <div className="flex items-center gap-2 mb-4">
                <div ref={dragHandle}>
                    <GripVertical className="w-6 h-6 cursor-grab" />
                </div>
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

function buildNextColorPalette<S extends SignalMetadata, T>(previousPalette: Record<string, T>, signals: S[], createEntry: (signal: S) => T): Record<string, T> {
    return Object.fromEntries(signals.map((signal) => [signal.name, previousPalette[signal.name] ?? createEntry(signal)]));
}

function NumericalWidgetEditSignalsModal(props: { widget: NumericalGraphWidgetData }) {
    const { widget } = props;
    const [modalOpen, setModalOpen] = useState(false);
    const { updateWidget } = useWidgetManager();

    const hasNoSignals = widget.signals.length === 0;

    const handleSave = (signals: NumericalSignalMetadata[]) => {
        const nextColorPalette = buildNextColorPalette(widget.options.colorPalette, signals, () => chroma.random());

        updateWidget(widget, (previousWidget) => ({
            ...previousWidget,
            signals,
            options: {
                ...previousWidget.options,
                colorPalette: nextColorPalette,
            },
        }));
        setModalOpen(false);
    };

    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger asChild>{hasNoSignals ? <PlusButton title="Add numerical signals" /> : <EditButton title="Edit numerical signals" />}</DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Edit Numerical Signals</DialogTitle>
                    <DialogDescription>Choose which live numerical signals this graph shows.</DialogDescription>
                </DialogHeader>
                <div className="min-w-0">
                    <NumericalSignalPicker initialSignals={widget.signals} onConfirm={handleSave} onCancel={() => setModalOpen(false)} />
                </div>
            </DialogContent>
        </Dialog>
    );
}

function EnumWidgetEditSignalsModal(props: { widget: EnumTimelineWidgetData }) {
    const { widget } = props;
    const [modalOpen, setModalOpen] = useState(false);
    const { updateWidget } = useWidgetManager();

    const hasNoSignals = widget.signals.length === 0;

    const handleSave = (signals: (EnumSignalMetadata | BooleanSignalMetadata)[]) => {
        const nextColorPalette = buildNextColorPalette(widget.options.colorPalette, signals, buildEnumPalette);

        updateWidget(widget, (previousWidget) => ({
            ...previousWidget,
            signals,
            options: {
                ...previousWidget.options,
                colorPalette: nextColorPalette,
            },
        }));
        setModalOpen(false);
    };

    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger asChild>{hasNoSignals ? <PlusButton title="Add enum signals" /> : <EditButton title="Edit enum signals" />}</DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle className="text-lg font-bold mb-4">Edit Enum Signals</DialogTitle>
                    <DialogDescription>Choose which live enum signals this timeline shows.</DialogDescription>
                </DialogHeader>
                <div className="min-w-0">
                    <EnumSignalPicker initialSignals={widget.signals} onConfirm={handleSave} onCancel={() => setModalOpen(false)} />
                </div>
            </DialogContent>
        </Dialog>
    );
}

export const Widget = memo(function Widget(props: WidgetData & { hoveredSignal: RefObject<string | null> }) {
    const { hoveredSignal, id } = props;
    const { updateWidget, removeSignal } = useWidgetManager();

    const [{ isDragging }, drag] = useDrag(
        () => ({
            type: DRAGGABLE_TYPES.WIDGET,
            item: { id: id },
            collect: (monitor) => ({
                isDragging: !!monitor.isDragging(),
            }),
        }),
        [id]
    );

    switch (props.type) {
        case "numericalGraph": {
            const widget = props as NumericalGraphWidgetData;
            return (
                <>
                    <WidgetConfiguration id={widget.id} dragHandle={drag as unknown as React.RefObject<HTMLDivElement>}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={(signalName) => removeSignal(widget, signalName)} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name] ?? chroma("#ffffff")} />
                        ))}
                        <NumericalWidgetEditSignalsModal widget={widget} />
                    </WidgetConfiguration>
                    {widget.signals.length === 0 ? <EmptyWidgetState message="Add a numerical signal to start graphing live data." /> : <NumericalCanvasChart {...widget} hoveredSignal={hoveredSignal} />}
                </>
            );
        }
        case "enumTimeline": {
            const widget = props as EnumTimelineWidgetData;
            return (
                <>
                    <WidgetConfiguration id={widget.id} dragHandle={drag as unknown as React.RefObject<HTMLDivElement>}>
                        {widget.signals.map((signal) => (
                            <SignalButton key={signal.name} signal={signal} handleRemoveSignal={(signalName) => removeSignal(widget, signalName)} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name]?.color ?? chroma("#ffffff")} />
                        ))}
                        <EnumWidgetEditSignalsModal widget={widget} />
                    </WidgetConfiguration>
                    {widget.signals.length === 0 ? <EmptyWidgetState message="Add an enum signal to start the timeline." /> : <EnumCanvasChart {...widget} hoveredSignal={hoveredSignal} />}
                </>
            );
        }
    }
});
