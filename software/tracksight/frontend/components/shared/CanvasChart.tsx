"use client";

import React, { useRef, useEffect } from "react";

type SeriesMeta = {
  label: string;
  color?: string;
};

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
type AlignedData = [number[], ...Array<(number | null)[]>];

type CanvasChartProps = {
  data: AlignedData;
  series: SeriesMeta[];
  width: number;
  height: number;
  panOffset?: number;
  zoomLevel?: number;
  frozenTimeWindow?: { startTime: number; endTime: number } | null;
  downsampleThreshold?: number;
  timeTickCount?: number;
};

// first index where timestamp >= targetTime
function binarySearchForFirstVisibleIndex(
  timestamps: number[],
  targetTime: number
): number {
  let left = 0;
  let right = timestamps.length - 1;
  let result = 0;

  while (left <= right) {
    const mid = Math.floor((left + right) / 2);
    if (timestamps[mid] >= targetTime) {
      result = mid;
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  return result;
}

// last index where timestamp <= targetTime
function binarySearchForLastVisibleIndex(
  timestamps: number[],
  targetTime: number
): number {
  let left = 0;
  let right = timestamps.length - 1;
  let result = timestamps.length - 1;

  while (left <= right) {
    const mid = Math.floor((left + right) / 2);
    if (timestamps[mid] <= targetTime) {
      result = mid;
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return result;
}

export default function CanvasChart({
  data,
  series,
  width,
  height,
  panOffset = 0,
  zoomLevel = 100,
  frozenTimeWindow = null,
  downsampleThreshold = 100000,
  timeTickCount = 6,
}: CanvasChartProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const context = canvas.getContext("2d");
    if (!context) return;

    // downsample if needed
    let dataToRender: AlignedData = data;
    const [originalTimestamps] = data;
    if (
      downsampleThreshold &&
      originalTimestamps &&
      originalTimestamps.length > downsampleThreshold
    ) {
      const [timestamps, ...seriesData] = data;
      const newTimestamps: number[] = [];
      const newSeriesData = seriesData.map(() => [] as (number | null)[]);
      const step = timestamps.length / downsampleThreshold;

      for (let i = 0; i < downsampleThreshold; i++) {
        const index = Math.floor(i * step);
        newTimestamps.push(timestamps[index]);
        seriesData.forEach((series, seriesIndex) => {
          newSeriesData[seriesIndex].push(series[index]);
        });
      }
      // last point is always included
      if (
        newTimestamps.length > 0 &&
        timestamps.length > 0 &&
        newTimestamps[newTimestamps.length - 1] !==
          timestamps[timestamps.length - 1]
      ) {
        newTimestamps.push(timestamps[timestamps.length - 1]);
        seriesData.forEach((series, seriesIndex) => {
          newSeriesData[seriesIndex].push(series[series.length - 1]);
        });
      }
      dataToRender = [newTimestamps, ...newSeriesData];
    }

    const timeFormatter = new Intl.DateTimeFormat(undefined, {
      hour: "numeric",
      minute: "2-digit",
      second: "2-digit",
      hour12: true,
    });

    const dateFormatter = new Intl.DateTimeFormat(undefined, {
      year: "numeric",
      month: "short",
      day: "2-digit",
    });

    const render = () => {
      context.clearRect(0, 0, width, height);

      const [timestamps, ...seriesData] = dataToRender;
      if (!timestamps || timestamps.length === 0) return;

      const clampedTickCount = Math.max(1, Math.floor(timeTickCount));
      const padding = { top: 20, right: 20, bottom: 56, left: 60 };
      const chartWidth = width - padding.left - padding.right;
      const chartHeight = height - padding.top - padding.bottom;

      const latestTime = timestamps[timestamps.length - 1];
      const earliestTime = timestamps[0];
      const totalTimeRange = latestTime - earliestTime;

      let visibleStartTime: number;
      let visibleEndTime: number;

      if (frozenTimeWindow) {
        // use the frozen time window when manually panning
        visibleStartTime = frozenTimeWindow.startTime;
        visibleEndTime = frozenTimeWindow.endTime;
      } else {
        // otherwise, calculate based on zoom and panOffset and auto follow
        // zoomLevel: 100 = show all data, 200 = show half the data (2x zoom), 50 = show double (0.5x zoom) etc.
        const zoomFactor = 100 / zoomLevel;
        const visibleTimeRange = totalTimeRange * zoomFactor;

        // panOffset is in pixels from the right edge (latest data)
        const timePerPixel = visibleTimeRange / chartWidth;
        const timeOffset = panOffset * timePerPixel;

        visibleEndTime = latestTime - timeOffset;
        visibleStartTime = visibleEndTime - visibleTimeRange;
      }

      const startIndex = binarySearchForFirstVisibleIndex(
        timestamps,
        visibleStartTime
      );

      const endIndex = binarySearchForLastVisibleIndex(
        timestamps,
        visibleEndTime
      );

      // data ranges - only use visible range
      const minTime = visibleStartTime;
      const maxTime = visibleEndTime;
      const rawTimeRange = maxTime - minTime;
      const timeRange = rawTimeRange <= 0 ? 1 : rawTimeRange;

      let minValue = Infinity;
      let maxValue = -Infinity;
      // only calculate min/max for visible data points
      seriesData.forEach((dataPoints) => {
        for (let i = startIndex; i <= endIndex; i++) {
          const val = dataPoints[i];
          if (val !== null) {
            minValue = Math.min(minValue, val);
            maxValue = Math.max(maxValue, val);
          }
        }
      });

      // edge case where all values are the same
      if (minValue === maxValue) {
        minValue -= 1;
        maxValue += 1;
      }

      // transformation functions
      const timeToX = (time: number) => {
        return padding.left + ((time - minTime) / timeRange) * chartWidth;
      };

      const valueToY = (value: number) => {
        return (
          padding.top +
          chartHeight -
          ((value - minValue) / (maxValue - minValue)) * chartHeight
        );
      };

      context.strokeStyle = "#333";
      context.lineWidth = 1;
      context.beginPath();

      // y-axis
      context.moveTo(padding.left, padding.top);
      context.lineTo(padding.left, height - padding.bottom);

      // x-axis
      context.lineTo(width - padding.right, height - padding.bottom);
      context.stroke();

      // grid lines
      context.strokeStyle = "#e0e0e0";
      context.lineWidth = 0.5;
      const numGridLines = 5;
      for (let i = 0; i <= numGridLines; i++) {
        const y = padding.top + (chartHeight / numGridLines) * i;
        context.beginPath();
        context.moveTo(padding.left, y);
        context.lineTo(width - padding.right, y);
        context.stroke();
      }

      // axis labels
      context.fillStyle = "#ffffffff";
      context.font = "12px sans-serif";
      context.textAlign = "right";
      context.textBaseline = "middle";

      // y-axis labels
      for (let i = 0; i <= numGridLines; i++) {
        const value = maxValue - ((maxValue - minValue) / numGridLines) * i;
        const y = padding.top + (chartHeight / numGridLines) * i;
        context.fillText(value.toFixed(2), padding.left - 5, y);
      }

      // x-axis tick marks & labels
      const numTimeTicks = clampedTickCount;
      context.strokeStyle = "#999";
      context.lineWidth = 1;
      context.textAlign = "center";
      context.textBaseline = "top";
      context.fillStyle = "#ffffffff";

      for (let i = 0; i <= numTimeTicks; i++) {
        const timeValue = minTime + (timeRange / numTimeTicks) * i;
        const x = timeToX(timeValue);

        context.beginPath();
        context.moveTo(x, height - padding.bottom);
        context.lineTo(x, height - padding.bottom + 6);
        context.stroke();

        const dateObj = new Date(timeValue);
        const msLabel = dateObj.getMilliseconds().toString().padStart(3, "0");
        const timeLabel = `${timeFormatter.format(dateObj)}.${msLabel}`;
        const dateLabel = dateFormatter.format(dateObj);

        context.fillText(timeLabel, x, height - padding.bottom + 8);
        context.fillText(dateLabel, x, height - padding.bottom + 24);
      }

      // draw data series
      seriesData.forEach((dataPoints, seriesIndex) => {
        const meta = series[seriesIndex];
        context.strokeStyle = meta?.color || "#000";
        context.lineWidth = 2;
        context.beginPath();

        let pathStarted = false;

        // only iterate through visible data points
        for (let i = startIndex; i <= endIndex; i++) {
          const time = timestamps[i];
          const value = dataPoints[i];

          if (value === null) {
            pathStarted = false;
            continue;
          }

          const x = timeToX(time);
          const y = valueToY(value);

          if (!pathStarted) {
            context.moveTo(x, y);
            pathStarted = true;
          } else {
            context.lineTo(x, y);
          }
        }

        context.stroke();
      });
    };

    animationFrameId.current = requestAnimationFrame(render);

    return () => {
      if (animationFrameId.current) {
        cancelAnimationFrame(animationFrameId.current);
      }
    };
  }, [
    data,
    series,
    width,
    height,
    panOffset,
    zoomLevel,
    frozenTimeWindow,
    timeTickCount,
  ]);

  return (
    <canvas
      ref={canvasRef}
      width={width}
      height={height}
      style={{ display: "block" }}
    />
  );
}
