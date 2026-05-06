"use client";

import { useDisplayControlContext } from "@/components/PausePlayControl";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { MouseEvent as MouseEvent_React, RefObject, useCallback } from "react";

/**
 * Converts mouse events on a canvas into time-domain hover values using the
 * chart layout written by the render function.
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
      const x = event.clientX - rect.left
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
