"use client";

import { time } from "console";
import React, { useRef, useEffect, useState } from "react";
// currently trying to replace uPlot, which is imported under /components/pages/live-data/Numerical.tsx

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
}: CanvasChartProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);

  // Calculate initial time range from the data
  const initialStartTime = data[0]?.[0] ?? 0;
  const initialEndTime = data[0]?.[data[0].length - 1] ?? 0;

  const [view, setView] = useState({
    startTime: initialStartTime,
    endTime: initialEndTime,
  });

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) {
      return;
    }

    const context = canvas.getContext("2d");
    if (!context) {
      return;
    }

    const render = () => {
      /*naively, we find the starting index within our data array that
       falls under the startTime with Array.prototype.find() or for loop
       but i'm not naive!!! binary search for o(log n)

       anyway, here's a couple todos:

       1. setup transformations (scaling, panning)
       1a) figure out how to map data coordinates to canvas coordinates
            - calculate min/max of data range, and mapping it to canvas dimensions' width/height
       
        2. draw axes and gridlines
        2a) draw x and y axes, labels, gridlines

        3. draw the data series (skeleton function below)

        4. figure out wtf requestNextAnimationFrame is and how to use it

        5. STRETCH GOAL which shouldn't be primary focus right now:
        downspampling for zooming out. pre-process data to find smaller set of points
        probably via LTTB or M4 or something idk
       */
      context.clearRect(0, 0, width, height);

      // 1.

      // 2.

      // drawing the data series
      const [timestamps, ...seriesData] = data;
      if (!timestamps || timestamps.length === 0) {
        return;
      }

      const startIndex = binarySearchForFirstVisibleIndex(
        timestamps,
        view.startTime
      );
      const endIndex = binarySearchForLastVisibleIndex(
        timestamps,
        view.endTime
      );

      seriesData.forEach((dataPoints, seriesIndex) => {
        const meta = series[seriesIndex];
        context.strokeStyle = meta?.color || "#000";
        context.lineWidth = 1;
        context.beginPath();

        // todo: replace w/ transformed coordinates.
        // placeholder loop; mayb iterate through
        // `timestamps` and `dataPoints` together, convert each (time, value)
        // pair to (x, y) canvas coordinates, and draw lines between them
        for (let i = 0; i < dataPoints.length; i++) {
          const x = (i / (dataPoints.length - 1)) * width; // placeholder X
          const y = height - ((dataPoints[i] ?? 0) / 100) * height; // placeholder Y

          if (i === 0) {
            context.moveTo(x, y);
          } else {
            context.lineTo(x, y);
          }
        }
        context.stroke();
      });
    };

    // start the rendering loop
    animationFrameId.current = requestAnimationFrame(render);

    // cleanup on comp. unmount or dependency change
    return () => {
      if (animationFrameId.current) {
        cancelAnimationFrame(animationFrameId.current);
      }
    };
  }, [data, series, width, height]);

  return (
    <canvas
      ref={canvasRef}
      width={width}
      height={height}
      style={{ display: "block" }}
    />
  );
}
