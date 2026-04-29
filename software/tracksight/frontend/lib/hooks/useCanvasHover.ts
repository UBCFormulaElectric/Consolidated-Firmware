"use client";

import { MouseEvent as MouseEvent_React, RefObject, useCallback } from "react";

/**
 * Tracks the cursor's CSS-pixel x within the canvas. The hook stores the raw x
 * (not a time) so each render frame can convert it through the latest XToTime
 * and avoid cross-frame staleness when scrollLeft drifts between mouse events
 * and render frames.
 */
export function useCanvasHover(
  canvasRef: RefObject<HTMLCanvasElement | null>,
  hoverXRef: RefObject<number | null>,
  onHoverXChange?: (x: number | null) => void
) {
  const handleMouseMove = useCallback(
    (event: MouseEvent_React<HTMLCanvasElement, MouseEvent>) => {
      const canvas = canvasRef.current;
      if (!canvas) return;
      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      hoverXRef.current = x;
      onHoverXChange?.(x);
    },
    [canvasRef, hoverXRef, onHoverXChange]
  );

  const handleMouseLeave = useCallback(() => {
    hoverXRef.current = null;
    onHoverXChange?.(null);
  }, [hoverXRef, onHoverXChange]);

  return { handleMouseMove, handleMouseLeave };
}
