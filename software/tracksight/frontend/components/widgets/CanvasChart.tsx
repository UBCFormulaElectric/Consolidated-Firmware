"use client";
// note to self: updatemockconfig -> mockwidgets, then pass setwidgets in

import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import render, {
  ChartLayout,
  ChunkStats,
  PreparedChartData,
  SeriesMeta,
} from "@/components/widgets/render";
import {
  MouseEvent as MouseEvent_React,
  useCallback,
  useEffect,
  useMemo,
  useRef,
  useState,
} from "react";

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
export interface AlignedData {
  timestamps: number[];
  series: Array<(number | string | null)[]>;
}

const EMPTY_PREPARED_DATA: PreparedChartData = {
  timestamps: [],
  seriesData: [],
  chunkSize: 0,
  chunkStats: [],
  enumSeriesIndices: [],
  numericalSeriesIndices: [],
  uniqueEnumValues: {},
};

export default function CanvasChart({
  data, series, height,
  timeTickCount = 6,
  onHoverTimestampChange,
  scalingMode = "absolute",
}: {
  data: AlignedData;
  series: SeriesMeta[];
  height: number;
  timeTickCount?: number;
  onHoverTimestampChange?: (timestamp: number | null) => void;
  scalingMode?: "absolute" | "relative";
}) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);
  const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
  const tooltipBufferRef = useRef<string[]>([]);
  const layoutRef = useRef<ChartLayout | null>(null);

  const { scalePxPerSecRef, timeRangeRef, scrollLeftRef, hoverTimestamp: externalHoverTimestampRef, } = useSyncedGraph();
  const [containerWidth, setContainerWidth] = useState(0);

  // handle resize of the container
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const observer = new ResizeObserver((entries) => {
      for (const entry of entries) {
        if (entry.contentRect) {
          setContainerWidth(entry.contentRect.width);
        }
      }
    });

    observer.observe(canvas);
    return () => observer.disconnect();
  }, []);

  const preparedData = useMemo<PreparedChartData>(() => {
    if (!data) {
      return EMPTY_PREPARED_DATA;
    }

    const workingTimestamps: number[] = data.timestamps ?? [];
    const workingSeries: Array<(number | string | null)[]> = data.series;
    const enumSeriesIndices: number[] = [];
    const numericalSeriesIndices: number[] = [];
    const uniqueEnumValues: Record<number, Set<string>> = {};
    const isEnumSeries = new Array<boolean>(workingSeries.length).fill(false); // [false, true, ..., false]
                                                                               // [0, 1, 2, ..., n] 1 is enum 

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
        isEnumSeries[idx] = true;
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

    const chunkSize = workingTimestamps.length > 0 ? Math.max(32, Math.ceil(workingTimestamps.length / 512)) : 0;
    const chunkStats: ChunkStats[] = workingSeries.map(() => ({ min: [], max: [], }));

    if (chunkSize > 0) {
      console.log("Calculating chunk stats with chunk size:", chunkSize);
      const chunkCount = Math.ceil(workingTimestamps.length / chunkSize);
      workingSeries.forEach((seriesPoints, seriesIndex) => {
        if (isEnumSeries[seriesIndex]) return;

        const stats = chunkStats[seriesIndex];
        for (let chunkIdx = 0; chunkIdx < chunkCount; chunkIdx++) {
          const start = chunkIdx * chunkSize;
          const end = Math.min(start + chunkSize, workingTimestamps.length);
          let chunkMin = Infinity;
          let chunkMax = -Infinity;

          for (let i = start; i < end; i++) {
            const value = seriesPoints[i];
            if (value === null || value === undefined || typeof value === "string") continue;
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

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const context = canvas.getContext("2d");
    if (!context) return;
    const render_call = () => {
      const dpr = window.devicePixelRatio || 1;

      // transform to the original size
      context.setTransform(1, 0, 0, 1, 0, 0);
      context.scale(dpr, dpr);

      const scrollLeft = scrollLeftRef.current;
      const scale = scalePxPerSecRef.current;
      const globalTimeRange = timeRangeRef.current;
      const externalHoverTimestamp = externalHoverTimestampRef.current;

      if (globalTimeRange) {
        const visibleStartTime = globalTimeRange.min + scrollLeft / scale;
        const visibleEndTime = visibleStartTime + containerWidth / scale;

        const visibleTimeRange = { min: visibleStartTime, max: visibleEndTime };

        render(
          context,
          containerWidth,
          height,
          preparedData,
          series,
          timeTickCount,
          externalHoverTimestamp,
          hoverPixelRef,
          tooltipBufferRef,
          layoutRef,
          visibleTimeRange,
          scalingMode
        );
      }

      animationFrameId.current = requestAnimationFrame(render_call);
    };
    animationFrameId.current = requestAnimationFrame(render_call);
    return () => {
      if (animationFrameId.current === null) return;
      cancelAnimationFrame(animationFrameId.current);
      animationFrameId.current = null;
    };
  }, [
    preparedData,
    series,
    height,
    containerWidth,
    timeTickCount,
    onHoverTimestampChange,
    scalePxPerSecRef,
    timeRangeRef,
    scrollLeftRef,
    externalHoverTimestampRef,
    scalingMode,
  ]);

  const handleMouseMove = useCallback(
    (event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
      const canvas = canvasRef.current;
      if (!canvas) return;
      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      const y = event.clientY - rect.top;
      hoverPixelRef.current = { x, y };

      if (layoutRef.current) {
        const { minTime, timeRange, chartWidth, paddingLeft } = layoutRef.current;
        if (chartWidth > 0) {
          const calculatedTime = minTime + ((x - paddingLeft) / chartWidth) * timeRange;

          externalHoverTimestampRef.current = calculatedTime;

          if (onHoverTimestampChange) {
            onHoverTimestampChange(calculatedTime);
          }
        }
      }
    },
    [onHoverTimestampChange, externalHoverTimestampRef]
  );

  const handleMouseLeave = useCallback(() => {
    hoverPixelRef.current = null;
    externalHoverTimestampRef.current = null;
    if (onHoverTimestampChange) {
      onHoverTimestampChange(null);
    }
  }, [onHoverTimestampChange, externalHoverTimestampRef]);

  if (containerWidth === 0) {
    return <canvas className="block w-full" ref={canvasRef} style={{ height }} />;
  }

  // get dpr from window
  const dpr = typeof window !== "undefined" ? window.devicePixelRatio || 1 : 1;

  return (
    <canvas
      className="block w-full"
      ref={canvasRef}
      width={containerWidth * dpr}
      height={height * dpr}
      style={{ width: "100%", height: height }}
      onMouseMove={handleMouseMove}
      onMouseLeave={handleMouseLeave}
    />
  );
}
