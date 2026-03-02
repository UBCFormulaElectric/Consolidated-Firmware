"use client";
// note to self: updatemockconfig -> mockwidgets, then pass setwidgets in

import { useRef, useEffect, useCallback, RefObject } from "react";
import { MouseEvent as MouseEvent_React } from "react";
import render, { render_empty } from "@/components/widgets/render";
import { ChartData, ChartLayout } from "./CanvasChartTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

function useObserveResize(ref: RefObject<HTMLCanvasElement | null>) {
    const containerWidth = useRef(0);
    // handle resize of the container
    useEffect(() => {
        const canvas = ref.current;
        if (!canvas) return;
        const observer = new ResizeObserver(entries => {
            console.assert(entries.length == 1, "Expected exactly one entry in ResizeObserver");
            containerWidth.current = entries[0].contentRect.width;
        });
        observer.observe(canvas);
        return () => observer.disconnect();
    }, [ref.current]);
    return containerWidth;
}

export default function CanvasChart({ chartData: chart_data, height, hoveredSignal, timeTickCount = 6, onHoverTimestampChange }: {
    chartData: RefObject<ChartData>,
    height: number;
    timeTickCount?: number;
    onHoverTimestampChange?: (timestamp: number | null) => void;
    hoveredSignal?: RefObject<string | null>;
}) {
    // TODO highlighted hoveredSignal
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const containerWidth = useObserveResize(canvasRef);
    const animationFrameId = useRef<number | null>(null);
    const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
    const tooltipBufferRef = useRef<string[]>([]);
    const layoutRef = useRef<ChartLayout | null>(null);
    const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;
    const { scalePxPerSecRef, timeRangeRef, scrollLeftRef, hoverTimestamp: externalHoverTimestampRef } = useSyncedGraph();

    // const timestamps = data?.timestamps;
    // const firstTimestamp = timestamps && timestamps.length > 0 ? timestamps[0] : null;
    // const lastTimestamp = timestamps && timestamps.length > 0 ? timestamps[timestamps.length - 1] : null;
    // Update global time range based on data
    // useEffect(() => {
    //   if (firstTimestamp === null || lastTimestamp === null) return;
    //   if (firstTimestamp >= lastTimestamp) return;

    //   const current = timeRangeRef.current;
    //   // Update if range is different or not set yet
    //   if (!current || current.min !== firstTimestamp || current.max !== lastTimestamp) {
    //     setTimeRange({ min: firstTimestamp, max: lastTimestamp });
    //   }
    // }, [firstTimestamp, lastTimestamp, setTimeRange, timeRangeRef]);

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
            const cssWidth = containerWidth.current || canvas.getBoundingClientRect().width;
            const nextCanvasWidth = Math.max(1, Math.floor(cssWidth * dpr));
            const nextCanvasHeight = Math.max(1, Math.floor(height * dpr));
            if (canvas.width !== nextCanvasWidth || canvas.height !== nextCanvasHeight) {
                canvas.width = nextCanvasWidth;
                canvas.height = nextCanvasHeight;
            }
            // transform to the original size
            context.setTransform(1, 0, 0, 1, 0, 0);
            context.scale(dpr, dpr);
            const timeRange = timeRangeRef.current;
            if (!timeRange) {
                // If no time range is set, clear the canvas and return early
                render_empty(context, containerWidth.current, height);
            } else {
                render(context, containerWidth.current, height, layoutRef, chart_data.current, timeTickCount, hoverPixelRef, {
                    min: timeRange.min, // TODO check this with zedwin
                    max: timeRange.max,
                });
            }
            // cleanup
            animationFrameId.current = requestAnimationFrame(render_call);
        }
        animationFrameId.current = requestAnimationFrame(render_call);
        console.log("starting animation frame");
        return () => {
            console.log("cancelling animation frame");
            if (animationFrameId.current === null) return;
            cancelAnimationFrame(animationFrameId.current);
            animationFrameId.current = null;
        };
    }, [
        canvasRef.current, chart_data, height, containerWidth,
        timeTickCount, onHoverTimestampChange,
        scalePxPerSecRef, timeRangeRef, scrollLeftRef, externalHoverTimestampRef
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
            if (onHoverTimestampChange) {
                onHoverTimestampChange(calculatedTime);
            }
        }
    }, [onHoverTimestampChange, externalHoverTimestampRef]);

    const handleMouseLeave = useCallback(() => {
        hoverPixelRef.current = null;
        externalHoverTimestampRef.current = null;
        if (onHoverTimestampChange) {
            onHoverTimestampChange(null);
        }
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
