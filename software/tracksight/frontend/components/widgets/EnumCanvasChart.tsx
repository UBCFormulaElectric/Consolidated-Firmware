"use client";

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import render, { CHART_PADDING, render_empty } from "@/components/widgets/render";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useCanvasHover } from "@/lib/hooks/useCanvasHover";
import { useCanvasRenderLoop } from "@/lib/hooks/useCanvasRenderLoop";
import { getVisibleTelemetryMarkers } from "@/lib/telemetryMarkers";
import { EnumTimelineWidgetData } from "@/lib/types/Widget";
import { useRef } from "react";

// TODO(evan): This can probably be merged into NumericalCanvasChart w some type magic.
export default function EnumCanvasChart({ id, options, signals, hoveredSignal, onHoverTimestampChange }: EnumTimelineWidgetData) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const layoutRef = useRef<ChartLayout | null>(null);
    const { globalTimeRangeRef, hoverXRef, XToTime } = useSyncedGraph();

    const { height, timeTickCount } = options;
    const canvasHeight = Math.max(height, CHART_PADDING.top + 30 + signals.length * 40 + Math.max(0, signals.length - 1) * 40 + CHART_PADDING.bottom);

    const chartData = useSignalDataStores(signals);

    useCanvasRenderLoop(canvasRef, canvasHeight, (context, cssWidth) => {
        if (!globalTimeRangeRef.current) {
            render_empty(context, cssWidth, canvasHeight);
            return;
        }

        const hoverTimestamp = hoverXRef.current === null ? null : XToTime(hoverXRef.current);

        render(
            context,
            cssWidth,
            canvasHeight,
            layoutRef,
            {
                type: "enumTimeline",
                signals,
                options,
                data: chartData.current,
                id,
            },
            timeTickCount,
            hoverTimestamp,
            hoveredSignal,
            {
                min: XToTime(CHART_PADDING.left),
                max: XToTime(cssWidth - CHART_PADDING.right),
            },
            getVisibleTelemetryMarkers(XToTime(CHART_PADDING.left), XToTime(cssWidth - CHART_PADDING.right))
        );
    });

    const { handleMouseMove, handleMouseLeave } = useCanvasHover(canvasRef, hoverXRef, (x) => onHoverTimestampChange?.(x === null ? null : XToTime(x)));

    return <canvas className="block w-full" ref={canvasRef} style={{ height: canvasHeight }} onMouseMove={handleMouseMove} onMouseLeave={handleMouseLeave} />;
}
