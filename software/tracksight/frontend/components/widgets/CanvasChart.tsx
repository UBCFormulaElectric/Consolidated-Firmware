"use client";
// note to self: updatemockconfig -> mockwidgets, then pass setwidgets in

import { useRef, useEffect, useMemo, useCallback, useState } from "react";
import { MouseEvent as MouseEvent_React } from "react";
import render, { ChartLayout, ChunkStats, PreparedChartData, SeriesMeta } from "@/components/widgets/render";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
export interface AlignedData {
    timestamps: number[];
    series: Array<(number | string | null)[]>;
};

export default function CanvasChart({
    data, series, height, timeTickCount = 6,
    onHoverTimestampChange,
}: {
    data: AlignedData; series: SeriesMeta[];
    height: number;
    timeTickCount?: number;
    onHoverTimestampChange?: (timestamp: number | null) => void;
}) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const animationFrameId = useRef<number | null>(null);
    const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
    const tooltipBufferRef = useRef<string[]>([]);
    const layoutRef = useRef<ChartLayout | null>(null);
    const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;
    const { scalePxPerSecRef, timeRangeRef, scrollLeftRef, hoverTimestamp: externalHoverTimestampRef } = useSyncedGraph();
    const containerWidth = useRef(0);

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

    // handle resize of the container
    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const observer = new ResizeObserver(entries => {
            console.assert(entries.length == 1, "Expected exactly one entry in ResizeObserver");
            containerWidth.current = entries[0].contentRect.width;
        });
        observer.observe(canvas);
        return () => observer.disconnect();
    }, [canvasRef.current]);

    // TODO
    const preparedData = useMemo<PreparedChartData>(() => {
        if (!data) {
            return {
                timestamps: [],
                seriesData: [],
                chunkSize: 0,
                chunkStats: [],
                enumSeriesIndices: [],
                numericalSeriesIndices: [],
                uniqueEnumValues: {},
            };
        }

        const { timestamps: originalTimestamps, series: originalSeries } = data;
        const timestamps = originalTimestamps ?? [];

        let workingTimestamps: number[] = timestamps;
        let workingSeries: Array<(number | string | null)[]> = originalSeries;

        const enumSeriesIndices: number[] = [];
        const numericalSeriesIndices: number[] = [];
        const uniqueEnumValues: Record<number, Set<string>> = {};

        workingSeries.forEach((s, idx) => {
            let isEnum = false;
            for (const val of s) {
                if (val !== null && val !== undefined) {
                    if (typeof val === "string") {
                        isEnum = true;
                    }
                    break;
                }
            }

            if (isEnum) {
                enumSeriesIndices.push(idx);
                uniqueEnumValues[idx] = new Set<string>();
                s.forEach((val) => {
                    if (typeof val === "string") {
                        uniqueEnumValues[idx].add(val);
                    }
                });
            } else {
                numericalSeriesIndices.push(idx);
            }
        });

        const chunkSize =
            workingTimestamps.length > 0
                ? Math.max(32, Math.ceil(workingTimestamps.length / 512))
                : 0;
        const chunkStats: ChunkStats[] = workingSeries.map(() => ({
            min: [],
            max: [],
        }));

        if (chunkSize > 0) {
            const chunkCount = Math.ceil(workingTimestamps.length / chunkSize);
            workingSeries.forEach((seriesPoints, seriesIndex) => {
                if (enumSeriesIndices.includes(seriesIndex)) return;

                const stats = chunkStats[seriesIndex];
                for (let chunkIdx = 0; chunkIdx < chunkCount; chunkIdx++) {
                    const start = chunkIdx * chunkSize;
                    const end = Math.min(start + chunkSize, workingTimestamps.length);
                    let chunkMin = Infinity;
                    let chunkMax = -Infinity;

                    for (let i = start; i < end; i++) {
                        const value = seriesPoints[i];
                        if (
                            value === null ||
                            value === undefined ||
                            typeof value === "string"
                        )
                            continue;
                        if (value < chunkMin) chunkMin = value;
                        if (value > chunkMax) chunkMax = value;
                    }

                    stats.min[chunkIdx] = chunkMin;
                    stats.max[chunkIdx] = chunkMax;
                }
            });
        }

        const uniqueEnumValuesRecord: Record<number, string[]> = {};
        Object.keys(uniqueEnumValues).forEach((k) => {
            const key = Number(k);
            uniqueEnumValuesRecord[key] = Array.from(uniqueEnumValues[key]).sort();
        });

        return {
            timestamps: workingTimestamps,
            seriesData: workingSeries,
            chunkSize,
            chunkStats,
            enumSeriesIndices,
            numericalSeriesIndices,
            uniqueEnumValues: uniqueEnumValuesRecord,
        };
    }, [data]);

    // RENDER LOOP
    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const context = canvas.getContext("2d");
        if (!context) return;
        const render_call = () => {
            // transform to the original size
            context.setTransform(1, 0, 0, 1, 0, 0);
            context.scale(dpr, dpr);
            const scale = scalePxPerSecRef.current;
            const globalTimeRange = timeRangeRef.current;
            if (globalTimeRange) {
                render(context, containerWidth.current, height, layoutRef, preparedData, series, timeTickCount,
                    externalHoverTimestampRef.current, hoverPixelRef, tooltipBufferRef, {
                    min: globalTimeRange.min + (scrollLeftRef.current / scale),
                    max: globalTimeRange.min + (scrollLeftRef.current / scale) + (containerWidth.current / scale)
                });
            }
            animationFrameId.current = requestAnimationFrame(render_call);
        }
        animationFrameId.current = requestAnimationFrame(render_call);
        return () => {
            if (animationFrameId.current === null) return;
            cancelAnimationFrame(animationFrameId.current);
            animationFrameId.current = null;
        };
    }, [preparedData, series, height, containerWidth, timeTickCount,
        onHoverTimestampChange, scalePxPerSecRef, timeRangeRef, scrollLeftRef, externalHoverTimestampRef]);

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
            width={containerWidth.current * dpr}
            height={height * dpr}
            style={{ width: "100%", height: height }}
            onMouseMove={handleMouseMove}
            onMouseLeave={handleMouseLeave}
        />
    );
}
