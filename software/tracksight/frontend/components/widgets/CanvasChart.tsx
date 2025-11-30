"use client";

import { useRef, useEffect, useMemo, useCallback, useState } from "react";
import { MouseEvent as MouseEvent_React } from "react";
import render, { ChartLayout, ChunkStats, PreparedChartData, SeriesMeta } from "@/components/widgets/render";

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
export interface AlignedData {
  timestamps: number[];
  series: Array<(number | string | null)[]>;
};

export default function CanvasChart({
  data, series, height, panOffset = 0, scrollProgress = 1, zoomLevel = 100, frozenTimeWindow = null,
  downsampleThreshold = 100000, timeTickCount = 6, hoverTimestamp: externalHoverTimestamp,
  onHoverTimestampChange, domainStart, domainEnd,
}: {
  data: AlignedData; series: SeriesMeta[];
  height: number; panOffset?: number; scrollProgress?: number; zoomLevel?: number;
  frozenTimeWindow?: { startTime: number; endTime: number } | null;
  downsampleThreshold?: number; timeTickCount?: number; hoverTimestamp: number | null;
  onHoverTimestampChange?: (timestamp: number | null) => void;
  domainStart?: number; domainEnd?: number;
}) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);
  const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
  const tooltipBufferRef = useRef<string[]>([]);
  const layoutRef = useRef<ChartLayout | null>(null);

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

    if (downsampleThreshold && workingTimestamps.length > downsampleThreshold) {
      const newTimestamps: number[] = [];
      const newSeriesData = workingSeries.map(
        () => [] as (number | string | null)[]
      );
      const step = workingTimestamps.length / downsampleThreshold;

      for (let i = 0; i < downsampleThreshold; i++) {
        const index = Math.floor(i * step);
        newTimestamps.push(workingTimestamps[index]);
        workingSeries.forEach((seriesPoints, seriesIndex) => {
          newSeriesData[seriesIndex].push(seriesPoints[index]);
        });
      }

      if (
        newTimestamps.length > 0 &&
        workingTimestamps.length > 0 &&
        newTimestamps[newTimestamps.length - 1] !==
        workingTimestamps[workingTimestamps.length - 1]
      ) {
        newTimestamps.push(workingTimestamps[workingTimestamps.length - 1]);
        workingSeries.forEach((seriesPoints, seriesIndex) => {
          const source = seriesPoints[seriesPoints.length - 1];
          newSeriesData[seriesIndex].push(source);
        });
      }

      workingTimestamps = newTimestamps;
      workingSeries = newSeriesData;
    }

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
  }, [data, downsampleThreshold]);

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

      render(context, containerWidth, height, preparedData, series,
        panOffset, scrollProgress, zoomLevel, frozenTimeWindow, timeTickCount,
        externalHoverTimestamp, hoverPixelRef, tooltipBufferRef, layoutRef, domainStart, domainEnd);
      animationFrameId.current = requestAnimationFrame(render_call);
    }
    animationFrameId.current = requestAnimationFrame(render_call);
    return () => {
      if (animationFrameId.current === null)
        // throw new Error("animationFrameId.current should not be null here");
        return; 
      cancelAnimationFrame(animationFrameId.current);
      animationFrameId.current = null;
      hoverPixelRef.current = null;
    };
  }, [preparedData, series, height, containerWidth, panOffset, scrollProgress, zoomLevel, frozenTimeWindow, timeTickCount,
    externalHoverTimestamp, onHoverTimestampChange, domainStart, domainEnd]);

  const handleMouseMove = useCallback((event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const rect = canvas.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;
    hoverPixelRef.current = { x, y };

    if (layoutRef.current && onHoverTimestampChange) {
      const { minTime, timeRange, chartWidth, paddingLeft } =
        layoutRef.current;
      if (chartWidth > 0) {
        const calculatedTime =
          minTime + ((x - paddingLeft) / chartWidth) * timeRange;
        onHoverTimestampChange(calculatedTime);
      }
    }
  }, []);

  const handleMouseLeave = useCallback(() => {
    hoverPixelRef.current = null;
    if (onHoverTimestampChange) {
      onHoverTimestampChange(null);
    }
  }, []);

  if (containerWidth === 0) {
    return <canvas className="block w-full" ref={canvasRef} style={{ height }} />;
  }

  // get dpr from window 
  const dpr = typeof window !== 'undefined' ? window.devicePixelRatio || 1 : 1;

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
