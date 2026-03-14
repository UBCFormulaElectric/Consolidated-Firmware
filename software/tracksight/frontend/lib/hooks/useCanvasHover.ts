"use client";

import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import { MouseEvent as MouseEvent_React, RefObject, useCallback } from "react";

/**
 * Converts mouse events on a canvas into time-domain hover values using the
 * chart layout written by the render function.
 */
export function useCanvasHover(
  canvasRef: RefObject<HTMLCanvasElement | null>,
  layoutRef: RefObject<ChartLayout | null>,
  hoverTimestampRef: RefObject<number | null>,
  onHoverTimestampChange?: (timestamp: number | null) => void
) {
  const handleMouseMove = useCallback(
    (event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
      const canvas = canvasRef.current;
      if (!canvas) return;
      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      if (layoutRef.current) {
        const { minTime, timeRange, chartWidth, paddingLeft } = layoutRef.current;
        console.assert(chartWidth > 0, "Chart width must be greater than 0");
        const calculatedTime = minTime + ((x - paddingLeft) / chartWidth) * timeRange;
        hoverTimestampRef.current = calculatedTime;
        onHoverTimestampChange?.(calculatedTime);
      }
    },
    [canvasRef, layoutRef, hoverTimestampRef, onHoverTimestampChange]
  );

  const handleMouseLeave = useCallback(() => {
    hoverTimestampRef.current = null;
    onHoverTimestampChange?.(null);
  }, [hoverTimestampRef, onHoverTimestampChange]);

  return { handleMouseMove, handleMouseLeave };
}
