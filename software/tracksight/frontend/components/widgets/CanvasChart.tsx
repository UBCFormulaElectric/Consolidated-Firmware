"use client";
// note to self: updatemockconfig -> mockwidgets, then pass setwidgets in

import { useRef, useEffect, useCallback, RefObject } from "react";
import { MouseEvent as MouseEvent_React } from "react";
import render, { render_empty } from "@/components/widgets/render";
import { ChartData, ChartLayout } from "./CanvasChartTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

export default function CanvasChart({ chartData: chart_data, height, hoveredSignal, timeTickCount = 6, onHoverTimestampChange }: {
    chartData: RefObject<ChartData>,
    height: number;
    timeTickCount?: number;
    onHoverTimestampChange?: (timestamp: number | null) => void;
    hoveredSignal?: RefObject<string | null>;
}) {
    // TODO highlighted hoveredSignal
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const animationFrameId = useRef<number | null>(null);
    const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
    const tooltipBufferRef = useRef<string[]>([]);
    const layoutRef = useRef<ChartLayout | null>(null);
    const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;
    const { scalePxPerSecRef, globalTimeRangeRef, scrollLeftRef, hoverTimestampRef: externalHoverTimestampRef, XToTime } = useSyncedGraph();

    // RENDER LOOP
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
                render(context, cssWidth, height, layoutRef, chart_data.current, timeTickCount, hoverPixelRef.current, {
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
        scalePxPerSecRef, globalTimeRangeRef, scrollLeftRef
    ]);

    const handleMouseMove = useCallback((event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const rect = canvas.getBoundingClientRect();
        const x = event.clientX - rect.left;
        const y = event.clientY - rect.top;
        hoverPixelRef.current = { x, y };

        if (layoutRef.current) {
            const { minTime, timeRange, chartWidth, paddingLeft } = layoutRef.current;
            console.assert(chartWidth > 0, "Chart width must be greater than 0");
            const calculatedTime = minTime + ((x - paddingLeft) / chartWidth) * timeRange;
            externalHoverTimestampRef.current = calculatedTime;
            onHoverTimestampChange && onHoverTimestampChange(calculatedTime);
        }
    }, [onHoverTimestampChange, externalHoverTimestampRef]);

    const handleMouseLeave = useCallback(() => {
        hoverPixelRef.current = null;
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
