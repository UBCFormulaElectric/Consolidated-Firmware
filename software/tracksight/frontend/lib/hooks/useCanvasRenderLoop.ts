"use client";

import { RefObject, useEffect, useRef } from "react";

/**
 * Drives a requestAnimationFrame loop on a <canvas>, handling DPR scaling
 * and automatic resize. The `draw` callback is invoked every frame with a
 * correctly-scaled 2D context and the current CSS width.
 */
export function useCanvasRenderLoop(
  canvasRef: RefObject<HTMLCanvasElement | null>,
  height: number,
  draw: (ctx: CanvasRenderingContext2D, cssWidth: number) => void
) {
  // Always call the latest draw without restarting the loop
  const drawRef = useRef(draw);
  drawRef.current = draw;

  const dpr = typeof window !== "undefined" ? window.devicePixelRatio || 1 : 1;
  const animationFrameId = useRef<number | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const context = canvas.getContext("2d");
    if (!context) return;

    const renderFrame = () => {
      const cssWidth = canvas.getBoundingClientRect().width;
      const nextCanvasWidth = Math.max(1, Math.floor(cssWidth * dpr));
      const nextCanvasHeight = Math.max(1, Math.floor(height * dpr));
      if (canvas.width !== nextCanvasWidth || canvas.height !== nextCanvasHeight) {
        canvas.width = nextCanvasWidth;
        canvas.height = nextCanvasHeight;
      }
      context.setTransform(1, 0, 0, 1, 0, 0);
      context.scale(dpr, dpr);

      drawRef.current(context, cssWidth);

      animationFrameId.current = requestAnimationFrame(renderFrame);
    };

    animationFrameId.current = requestAnimationFrame(renderFrame);

    return () => {
      if (animationFrameId.current !== null) {
        cancelAnimationFrame(animationFrameId.current);
        animationFrameId.current = null;
      }
    };
  }, [canvasRef, height, dpr]);
}
