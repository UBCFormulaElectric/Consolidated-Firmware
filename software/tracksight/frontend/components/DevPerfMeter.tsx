"use client";

import { perfStats } from "@/lib/perfStats";
import { useEffect, useRef } from "react";

const REPORT_INTERVAL_MS = 1000;
const JANK_MS = 50; // a frame this long is visible stutter
const SLOW_MS = 25;

/**
 * Dev-only readout for stress testing: frames per second, the worst frame in the last second, and samples
 * ingested per second. Updates the DOM directly once a second so measuring doesn't add React renders.
 */
export function DevPerfMeter() {
    const labelRef = useRef<HTMLSpanElement>(null);

    useEffect(() => {
        let frameId = 0;
        let frames = 0;
        let worstFrameMs = 0;
        let lastFrameAt = performance.now();
        let windowStart = lastFrameAt;
        let samplesAtWindowStart = perfStats.samplesIngested;

        const onFrame = (now: number) => {
            frames++;
            worstFrameMs = Math.max(worstFrameMs, now - lastFrameAt);
            lastFrameAt = now;

            const elapsed = now - windowStart;
            if (elapsed >= REPORT_INTERVAL_MS && labelRef.current) {
                const samplesPerSecond = ((perfStats.samplesIngested - samplesAtWindowStart) * 1000) / elapsed;
                labelRef.current.textContent = `${Math.round((frames * 1000) / elapsed)} fps · worst ${Math.round(worstFrameMs)} ms · ${samplesPerSecond >= 1000 ? `${(samplesPerSecond / 1000).toFixed(1)}k` : Math.round(samplesPerSecond)} samples/s`;
                labelRef.current.style.color = worstFrameMs >= JANK_MS ? "#b91c1c" : worstFrameMs >= SLOW_MS ? "#b45309" : "#4b5563";

                frames = 0;
                worstFrameMs = 0;
                windowStart = now;
                samplesAtWindowStart = perfStats.samplesIngested;
            }

            frameId = requestAnimationFrame(onFrame);
        };

        // hidden tabs get no frames; start a fresh window on return instead of reporting the gap as one huge frame
        const onVisibilityChange = () => {
            if (document.visibilityState !== "visible") return;
            frames = 0;
            worstFrameMs = 0;
            lastFrameAt = windowStart = performance.now();
            samplesAtWindowStart = perfStats.samplesIngested;
        };

        frameId = requestAnimationFrame(onFrame);
        document.addEventListener("visibilitychange", onVisibilityChange);
        return () => {
            cancelAnimationFrame(frameId);
            document.removeEventListener("visibilitychange", onVisibilityChange);
        };
    }, []);

    return <span ref={labelRef} className="font-mono text-xs tabular-nums text-gray-600" title="Dev performance meter: frames per second, worst frame in the last second, samples ingested per second" />;
}
