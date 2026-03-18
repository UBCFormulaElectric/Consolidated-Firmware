"use client";
import { RefObject, useCallback, useRef } from "react";

import NumericalCanvasChart from "./NumericalCanvasChart";
import EnumCanvasChart from "./EnumCanvasChart";
import { useWidgetManager } from "./WidgetManagerContext";
import { EnumWidgetData, NumericalWidgetData, WidgetData, WidgetType } from "@/lib/types/Widget";
import chroma, { Color } from "chroma-js";
import { SignalMetadata } from "@/lib/types/Signal";

function SignalButton({ cfg, handleRemoveSignal, hoverSignalName, color }: {
    cfg: SignalMetadata,
    idx: number,
    handleRemoveSignal: (signalName: string) => void,
    hoverSignalName: RefObject<string | null>,
    color: Color,
}) {
    return (
        <div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity cursor-crosshair"
            style={{ backgroundColor: color.brighten(1).hex(), borderColor: color.darken(1).hex() }}
            onMouseEnter={() => hoverSignalName.current = cfg.name} onMouseLeave={() => hoverSignalName.current = null}
        >
            {/* Left circle */}
            <div className="w-3 h-3 rounded-full" style={{ backgroundColor: color.hex() }} />
            <span className="font-medium">{cfg.name}</span>
            <button
                onClick={() => handleRemoveSignal(cfg.name)}
                className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none"
                title="Remove signal"
            >
                ×
            </button>
        </div>
    );
}

const NumericalWidgetSignalConfig = ({ widget, handleRemoveSignal, hoverSignalName }: {
    widget: NumericalWidgetData
    handleRemoveSignal: (signalName: string) => void,
    hoverSignalName: RefObject<string | null>,
}) => {
    return (
        <>
            {widget.signals.map((cfg, idx) => (
                <SignalButton
                    key={cfg.name}
                    cfg={cfg}
                    idx={idx}
                    handleRemoveSignal={handleRemoveSignal}
                    hoverSignalName={hoverSignalName}
                    color={widget.options.colorPalette[cfg.name] ? widget.options.colorPalette[cfg.name] : chroma("#fff")}
                />
            ))}
        </>
    );
}

const EnumWidgetSignalConfig = ({ widget, handleRemoveSignal, hoverSignalName }: {
    widget: EnumWidgetData
    handleRemoveSignal: (signalName: string) => void,
    hoverSignalName: RefObject<string | null>,
}) => {
    return (
        <>
            {widget.signals.map((cfg, idx) => (
                <SignalButton
                    key={cfg.name}
                    cfg={cfg}
                    idx={idx}
                    handleRemoveSignal={handleRemoveSignal}
                    hoverSignalName={hoverSignalName}
                    color={widget.options.colorPalette[cfg.name] ? widget.options.colorPalette[cfg.name].color : chroma("#fff")}
                />
            ))}
        </>
    );
}

export function WidgetConfiguration(
    props: { id: string, children?: React.ReactNode }
) {
    const { id, children } = props;

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

            {/* TODO(evan): These need to be split out */}
            <div className="flex flex-wrap items-center gap-3">
                {children}
            </div>
        </div>
    )
}

export function Widget(
    props: WidgetData & { hoveredSignal: RefObject<string | null> }
) {
    const { type, signals, id, hoveredSignal, options, data } = props;

    switch (type) {
        case "numericalGraph":
            return (
                <>
                    <WidgetConfiguration id={id}>
                        <NumericalWidgetSignalConfig
                            widget={props as NumericalWidgetData}
                            handleRemoveSignal={() => { }}
                            hoverSignalName={hoveredSignal}
                        />
                    </WidgetConfiguration>
                    <NumericalCanvasChart
                        id={id}
                        signals={signals}
                        hoveredSignal={hoveredSignal}
                        onHoverTimestampChange={() => { }}
                        data={data}
                        options={options}
                        type="numericalGraph"
                    />
                </>
            );
        case "enumTimeline":
            return (
                <>
                    <WidgetConfiguration id={id}>
                        {/* TODO(evan): Enum signal config */}
                    </WidgetConfiguration>
                    <EnumCanvasChart
                        id={id}
                        signals={signals}
                        hoveredSignal={hoveredSignal}
                        options={options}
                        type="enumTimeline"
                        data={data}
                    />
                </>
            );
    }

    return <div>Unknown widget type: {type}</div>
}
