"use client";

import { useRef, RefObject } from "react";
import render, { render_empty } from "@/components/widgets/render";
import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useCanvasRenderLoop } from "@/lib/hooks/useCanvasRenderLoop";
import { useCanvasHover } from "@/lib/hooks/useCanvasHover";
import { SignalType } from "@/lib/types/Signal";
import { WidgetData } from "@/lib/types/Widget";

type NumericalCanvasChartProps = Extract<WidgetData, { type: "numericalGraph" }> & {
    hoveredSignal?: RefObject<string | null>;
    onHoverTimestampChange?: (timestamp: number | null) => void;
};

export default function NumericalCanvasChart({
    id,
    options,
    signals,
    hoveredSignal,
    onHoverTimestampChange,
}: NumericalCanvasChartProps) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const layoutRef = useRef<ChartLayout | null>(null);
    const {
        globalTimeRangeRef,
        hoverTimestampRef: externalHoverTimestampRef,
        XToTime,
    } = useSyncedGraph();

    const { height, timeTickCount } = options;

    const chartData = useSignalDataStores(signals);

    useCanvasRenderLoop(canvasRef, height, (context, cssWidth) => {
        if (!globalTimeRangeRef.current) {
            render_empty(context, cssWidth, height);
        } else {
            render(
                context,
                cssWidth,
                height,
                layoutRef,
                {
                    type: "numericalGraph",
                    signals,
                    options,
                    data: chartData.current,
                    id,
                },
                timeTickCount,
                externalHoverTimestampRef.current,
                {
                    min: XToTime(0),
                    max: XToTime(cssWidth),
                }
            );
        }
    });

    const { handleMouseMove, handleMouseLeave } = useCanvasHover(
        canvasRef,
        layoutRef,
        externalHoverTimestampRef,
        onHoverTimestampChange
    );

    return (
        <canvas
            className="block w-full"
            ref={canvasRef}
            style={{ height }}
            onMouseMove={handleMouseMove}
            onMouseLeave={handleMouseLeave}
        />
    );
}
