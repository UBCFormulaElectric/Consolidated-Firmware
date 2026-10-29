"use client";

import { RefObject, useEffect, useRef } from "react";

/**
 * Drives a requestAnimationFrame loop on a <canvas>, handling DPR scaling
 * and automatic resize. While visible, `draw` is invoked every frame with a
 * correctly-scaled 2D context and the current CSS width.
 */
export function useCanvasRenderLoop(canvasRef: RefObject<HTMLCanvasElement | null>, height: number, draw: (ctx: CanvasRenderingContext2D, cssWidth: number) => void) {
    // Always call the latest draw without restarting the loop
    const drawRef = useRef(draw);
    drawRef.current = draw;

    const dpr = typeof window !== "undefined" ? window.devicePixelRatio || 1 : 1;
    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const context = canvas.getContext("2d");
        if (!context) return;

        let animationFrameId: number | null = null;
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

            animationFrameId = requestAnimationFrame(renderFrame);
        };

        const observer = new IntersectionObserver((entries) => {
            const entry = entries[entries.length - 1]; // entries are chronological; only the latest state matters
            if (entry.isIntersecting && animationFrameId === null) animationFrameId = requestAnimationFrame(renderFrame);
            if (!entry.isIntersecting && animationFrameId !== null) {
                cancelAnimationFrame(animationFrameId);
                animationFrameId = null;
            }
        });
        observer.observe(canvas);

        return () => {
            observer.disconnect();
            if (animationFrameId !== null) cancelAnimationFrame(animationFrameId);
        };
    }, [canvasRef, height, dpr]);
}
