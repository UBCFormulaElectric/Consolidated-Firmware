"use client";

import React, { useRef, useEffect, useState } from "react";

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
}: CanvasChartProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const context = canvas.getContext("2d");
    if (!context) return;

    const render = () => {
      /*naively, we find the starting index within our data array that
       falls under the startTime with Array.prototype.find() or for loop
       but i'm not naive!!! binary search for o(log n)

       anyway, here's a couple todos:

       1. setup transformations (scaling, panning)
       1a) figure out how to map data coordinates to canvas coordinates DONE
            - calculate min/max of data range, and mapping it to canvas dimensions' width/height
       
        2. draw axes and gridlines DONE
        2a) draw x and y axes, labels, gridlines DONE

        3. draw the data series (skeleton function below) DONE

        4. figure out wtf requestNextAnimationFrame is and how to use it DONE kinda

        5. STRETCH GOAL which shouldn't be primary focus right now:
        downspampling for zooming out. pre-process data to find smaller set of points
        probably via LTTB or M4 or something idk
       */

      context.clearRect(0, 0, width, height);

      const [timestamps, ...seriesData] = data;
      if (!timestamps || timestamps.length === 0) return;

      const padding = { top: 20, right: 20, bottom: 40, left: 60 };
      const chartWidth = width - padding.left - padding.right;
      const chartHeight = height - padding.top - padding.bottom;

      const latestTime = timestamps[timestamps.length - 1];
      const earliestTime = timestamps[0];
      const totalTimeRange = latestTime - earliestTime;

      // visible time window based on zoom and panOffset
      const zoomFactor = 100 / zoomLevel; // zoomLevel: 100 = show all data, 200 = show half the data (2x zoom), 50 = show double (0.5x zoom) etc.
      const visibleTimeRange = totalTimeRange * zoomFactor;

      // panOffset is in pixels from the right edge (latest data)
      const timePerPixel = visibleTimeRange / chartWidth;
      const timeOffset = panOffset * timePerPixel;

      const visibleEndTime = latestTime - timeOffset;
      const visibleStartTime = visibleEndTime - visibleTimeRange;

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
        return (
          padding.left + ((time - minTime) / (maxTime - minTime)) * chartWidth
        );
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
  }, [data, series, width, height, panOffset, zoomLevel]);

  return (
    <canvas
      ref={canvasRef}
      width={width}
      height={height}
      style={{ display: "block" }}
    />
  );
}
