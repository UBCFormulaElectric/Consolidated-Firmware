"use client";
// note to self: updatemockconfig -> mockwidgets, then pass setwidgets in

// react
import { useRef, useEffect, useCallback, RefObject, MouseEvent as MouseEvent_React } from "react";
// functions
import render, { render_empty } from "@/components/widgets/render";
// types
import { ChartData, ChartLayout } from "./CanvasChartTypes";
import { WidgetData } from "./WidgetTypes";
// hooks
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

export default function CanvasChart({ chartData: chart_data, widgetData, height, hoveredSignal, timeTickCount = 6, onHoverTimestampChange }: {
    widgetData: WidgetData;
    chartData: RefObject<ChartData>,
    height: number;
    timeTickCount?: number;
    onHoverTimestampChange?: (timestamp: number | null) => void;
    hoveredSignal?: RefObject<string | null>;
}) {
    // TODO highlighted hoveredSignal
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    // const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
    const tooltipBufferRef = useRef<string[]>([]);
    const layoutRef = useRef<ChartLayout | null>(null);
    const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;
    const { scalePxPerSecRef, globalTimeRangeRef, scrollLeftRef, hoverTimestampRef: externalHoverTimestampRef, XToTime } = useSyncedGraph();

    // RENDER LOOP
    const animationFrameId = useRef<number | null>(null);
    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) {
            console.error("Canvas element not found");
            return;
        }
        const context = canvas.getContext("2d");
        if (!context) {
            console.error("Render context not found");
            return;
        }
        const render_call = () => {
            // update width and height from css
            const cssWidth = canvas.getBoundingClientRect().width;
            const nextCanvasWidth = Math.max(1, Math.floor(cssWidth * dpr));
            const nextCanvasHeight = Math.max(1, Math.floor(height * dpr));
            if (canvas.width !== nextCanvasWidth || canvas.height !== nextCanvasHeight) {
                canvas.width = nextCanvasWidth;
                canvas.height = nextCanvasHeight;
            }
            // transform to the original size
            context.setTransform(1, 0, 0, 1, 0, 0);
            context.scale(dpr, dpr);
            if (!globalTimeRangeRef.current) {
                render_empty(context, cssWidth, height);
            } else {
                render(context, cssWidth, height, layoutRef, chart_data.current, widgetData, timeTickCount, externalHoverTimestampRef.current, hoveredSignal, {
                    min: XToTime(0),
                    max: XToTime(cssWidth),
                });
            }
            // cleanup
            animationFrameId.current = requestAnimationFrame(render_call);
        }
        animationFrameId.current = requestAnimationFrame(render_call);
        // console.log("starting animation frame");
        return () => {
            // console.log("cancelling animation frame");
            if (animationFrameId.current === null) return;
            cancelAnimationFrame(animationFrameId.current);
            animationFrameId.current = null;
        };
    }, [
        canvasRef.current, chart_data, height,
        timeTickCount, onHoverTimestampChange,
        scalePxPerSecRef, globalTimeRangeRef, scrollLeftRef, externalHoverTimestampRef, animationFrameId,
        render, render_empty // these two are mostly for dev purposes
    ]);

    const handleMouseMove = useCallback((event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const rect = canvas.getBoundingClientRect();
        const x = event.clientX - rect.left;
        if (layoutRef.current) {
            const { minTime, timeRange, chartWidth, paddingLeft } = layoutRef.current;
            console.assert(chartWidth > 0, "Chart width must be greater than 0");
            const calculatedTime = minTime + ((x - paddingLeft) / chartWidth) * timeRange;
            externalHoverTimestampRef.current = calculatedTime;
            onHoverTimestampChange && onHoverTimestampChange(calculatedTime);
        }
    }, [onHoverTimestampChange, externalHoverTimestampRef]);

    const handleMouseLeave = useCallback(() => {
        externalHoverTimestampRef.current = null;
        onHoverTimestampChange && onHoverTimestampChange(null);
    }, [onHoverTimestampChange, externalHoverTimestampRef]);

    return (
        <canvas
            className="block w-full"
            ref={canvasRef}
            style={{ height: height }}
            onMouseMove={handleMouseMove}
            onMouseLeave={handleMouseLeave}
        />
    );
}
