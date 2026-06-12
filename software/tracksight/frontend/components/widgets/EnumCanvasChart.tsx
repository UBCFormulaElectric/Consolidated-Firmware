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
    const hoverXRef = useRef<number | null>(null);
    const { globalTimeRangeRef, hoverTimestampRef: externalHoverTimestampRef, hoverXRef: contextHoverXRef, XToTime } = useSyncedGraph();

    const { height, timeTickCount } = options;
    const canvasHeight = Math.max(height, CHART_PADDING.top + 30 + signals.length * 40 + Math.max(0, signals.length - 1) * 40 + CHART_PADDING.bottom);

    const chartData = useSignalDataStores(signals);

    useCanvasRenderLoop(canvasRef, canvasHeight, (context, cssWidth) => {
        if (!globalTimeRangeRef.current) {
            render_empty(context, cssWidth, canvasHeight);
            return;
        }

        // Recompute hover time from the stored canvas-x using the latest scrollLeft
        // so the tooltip stays exactly under the cursor even if scroll drifts.
        // Only the chart currently being hovered owns externalHoverTimestampRef.
        if (hoverXRef.current !== null) {
            externalHoverTimestampRef.current = XToTime(hoverXRef.current);
        }

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
            externalHoverTimestampRef.current,
            hoveredSignal,
            {
                min: XToTime(CHART_PADDING.left),
                max: XToTime(cssWidth - CHART_PADDING.right),
            },
            getVisibleTelemetryMarkers(XToTime(CHART_PADDING.left), XToTime(cssWidth - CHART_PADDING.right))
        );
    });

    const { handleMouseMove, handleMouseLeave } = useCanvasHover(canvasRef, hoverXRef, (x) => {
        contextHoverXRef.current = x;
        const t = x === null ? null : XToTime(x);
        externalHoverTimestampRef.current = t;
        onHoverTimestampChange?.(t);
    });

    return <canvas className="block w-full" ref={canvasRef} style={{ height: canvasHeight }} onMouseMove={handleMouseMove} onMouseLeave={handleMouseLeave} />;
}
