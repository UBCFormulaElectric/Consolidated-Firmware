"use client";

import { useRef } from "react";
import render, { render_empty } from "@/components/widgets/render";
import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useCanvasRenderLoop } from "@/lib/hooks/useCanvasRenderLoop";
import { useCanvasHover } from "@/lib/hooks/useCanvasHover";
import { EnumTimelineWidgetData } from "@/lib/types/Widget";

// TODO(evan): This can probably be merged into NumericalCanvasChart w some type magic 🙏🏽🙏🏽🙏🏽.
export default function EnumCanvasChart({
    id,
    options,
    signals,
    hoveredSignal,
    onHoverTimestampChange,
}: EnumTimelineWidgetData) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const layoutRef = useRef<ChartLayout | null>(null);
    const {
        globalTimeRangeRef,
        hoverTimestampRef: externalHoverTimestampRef,
        XToTime,
    } = useSyncedGraph();

    const { height, timeTickCount } = options;

    const chartData = useSignalDataStores(signals);

    console.log(signals, options);

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
                    type: "enumTimeline",
                    signals,
                    options,
                    data: chartData.current,
                    id,
                },
                timeTickCount,
                externalHoverTimestampRef.current,
                hoveredSignal,
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
