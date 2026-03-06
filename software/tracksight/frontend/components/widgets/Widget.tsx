"use client";
import { RefObject, useCallback, useRef } from "react";

import { EnumSignalConfig, NumericalSignalConfig, WidgetConfigs } from "@/components/widgets/WidgetTypes";
import NumericalCanvasChart from "./NumericalCanvasChart";
import EnumCanvasChart from "./EnumCanvasChart";
import { useWidgetManager } from "./WidgetManagerContext";
import { UnknownWidgetRenderer, UnknownWidgetRendererProps, WidgetRendererProps, WidgetType } from "@/lib/types/Widget";

function SignalButton({ cfg, handleRemoveSignal, hoverSignalName }: {
    cfg: EnumSignalConfig | NumericalSignalConfig, idx: number,
    handleRemoveSignal: (signalName: string) => void,
    hoverSignalName: RefObject<string | null>,
}) {
    return (
        <div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity cursor-crosshair"
            style={{ backgroundColor: cfg.color.brighten(1).hex(), borderColor: cfg.color.darken(1).hex() }}
            onMouseEnter={() => hoverSignalName.current = cfg.signal_name} onMouseLeave={() => hoverSignalName.current = null}
        >
            {/* Left circle */}
            <div className="w-3 h-3 rounded-full" style={{ backgroundColor: cfg.color.hex() }} />
            <span className="font-medium">{cfg.signal_name}</span>
            <span className="text-xs text-gray-500">({cfg.delay}ms)</span>
            <button
                onClick={() => handleRemoveSignal(cfg.signal_name)}
                className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none"
                title="Remove signal"
            >
                ×
            </button>
        </div>
    );
}

export function WidgetConfiguration(
    props: {
        id: string;
        signals: WidgetConfigs;
        hoveredSignal: React.RefObject<string | null>;
    }
) {
    const { id, signals, hoveredSignal } = props;

    const { removeWidget, removeSignal } = useWidgetManager();

    const deleteSelfWidget = useCallback(() => { removeWidget(id); }, [id, removeWidget]);

    return (
        <div className="px-6">
            {/* header */}
            <div className="flex items-center gap-2 mb-4">
                <h3 className="font-semibold">Widget {id}</h3>
                <button onClick={deleteSelfWidget} title="Remove graph"
                    className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
                >
                    ×
                </button>
            </div>

            <div className="flex flex-wrap items-center gap-3">
                {signals.map((cfg, idx) =>
                    <SignalButton
                        key={cfg.signal_name}
                        cfg={cfg}
                        idx={idx}
                        handleRemoveSignal={() => removeSignal(id, cfg.signal_name)}
                        hoverSignalName={hoveredSignal}
                    />
                )}

                {/* TODO(evan): Implement this! */}
                {/* <AddSignalModal configs={signals} id={id} /> */}
            </div>
        </div>
    )
}

export function WidgetChart(
    props: Omit<UnknownWidgetRendererProps & { hoveredSignal: RefObject<string | null> }, "options">
) {
    const { type, signals, id, hoveredSignal } = props;

    switch (type) {
        case "numericalGraph":
            return (
                <NumericalCanvasChart
                    id={id}
                    height={256}
                    signals={signals as NumericalSignalConfig[]}
                    hoveredSignal={hoveredSignal}
                    onHoverTimestampChange={() => { }}
                />
            );
        case "enumTimeline":
            return (
                <EnumCanvasChart
                    id={id}
                    height={256}
                    signals={signals as EnumSignalConfig[]}
                    hoveredSignal={hoveredSignal}
                />
            );
    }

    return <div>Unknown widget type: {type}</div>
}

export function Widget(
    props: Omit<UnknownWidgetRendererProps, "options">
) {
    const { id, signals, type } = props;

    const hoveredSignal = useRef<string | null>(null);

    return (
        <div className="w-full border-red-500 py-4">
            <WidgetConfiguration id={id} signals={signals} hoveredSignal={hoveredSignal} />
            <WidgetChart id={id} signals={signals} type={type} hoveredSignal={hoveredSignal} />
        </div>
    )
}