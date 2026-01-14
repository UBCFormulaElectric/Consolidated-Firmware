import { createContext, useCallback, useContext, useEffect, useMemo, useRef, useState, RefObject, ReactNode } from "react";
import { usePausePlay } from "./PausePlayControl";

export type SyncedGraphContext = {
    scalePxPerSecRef: RefObject<number>;
    hoverTimestamp: RefObject<number | null>;
    timeRangeRef: RefObject<TimeRange | null>;
    setTimeRange: (tr: TimeRange) => void;
    scrollLeftRef: RefObject<number>;
};
// eslint-disable-next-line no-redeclare
const SyncedGraphContext = createContext<SyncedGraphContext | null>(null);
const RIGHT_PAD = 3600; // 1 min right 

export function useSyncedGraph() {
    const ctx = useContext(SyncedGraphContext);
    if (!ctx) {
        throw new Error("useSyncedGraph must be used within a SyncedGraphContainer");
    }
    return ctx;
}

export interface TimeRange {
    min: number;
    max: number;
}

const MIN_SCALE_PX_PER_SEC = 0;
const MAX_SCALE_PX_PER_SEC = 10000;

function useZoomManager(containerRef: RefObject<HTMLDivElement | null>) {
    const [scalePxPerSec, setScalePxPerSec] = useState(0.05);

    const handleWheelZoom = useCallback((event: WheelEvent) => {
        if (event.ctrlKey === false) {
            return;
        }
        event.preventDefault();
        const deltaScale = event.deltaY * -0.005; // invert for natural zoom 
        if (deltaScale === 0) return;

        // deltaScale usually in {-1,1}
        setScalePxPerSec(prev => Math.min(Math.max(prev * Math.exp(deltaScale), MIN_SCALE_PX_PER_SEC), MAX_SCALE_PX_PER_SEC));
    }, [setScalePxPerSec]);

    // TODO handle touch gestures for zoom
    useEffect(() => {
        const container = containerRef.current;
        if (!container) return;

        window.addEventListener("wheel", handleWheelZoom, { passive: false });
        return () => {
            window.removeEventListener("wheel", handleWheelZoom);
        };
    }, [handleWheelZoom, containerRef]);

    return scalePxPerSec;
}

export default function SyncedGraphContainer({ children }: { children: ReactNode }) {
    const { isPaused } = usePausePlay();
    const hoverTimestamp = useRef<number | null>(null);
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const scrollLeftRef = useRef<number>(0);
    const timeRangeRef = useRef<TimeRange | null>(null);

    const offsetRef = useRef<number>(0);

    // zoom management (attach wheel listener to the scroll container)
    const scalePxPerSec = useZoomManager(scrollContainerRef);
    const scalePxPerSecRef = useRef<number>(scalePxPerSec);

    const updateGraphWidth = useCallback(() => {
        const tr = timeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && tr && container) {
            const width = scalePxPerSecRef.current * (tr.max - tr.min) + RIGHT_PAD; // extra padding by 1min
            const containerWidth = container.clientWidth;

            scrollLeftRef.current = (width - RIGHT_PAD - 50 > containerWidth) ? width - RIGHT_PAD + 50 - containerWidth : 0;
            offsetRef.current = scrollLeftRef.current;

            container.scrollLeft = scrollLeftRef.current;
            contentRef.current.style.width = `${width}px`;
        }
    }, []);

    // Update width when zoom changes
    useEffect(() => {
        scalePxPerSecRef.current = scalePxPerSec;
        updateGraphWidth();
    }, [scalePxPerSec, updateGraphWidth]);

    // update timeRange and width without triggering a re-render
    const setTimeRange = useCallback((tr: TimeRange) => {
        const current = timeRangeRef.current;
        if (!current || current.min !== tr.min || current.max !== tr.max) {
            timeRangeRef.current = tr;
            updateGraphWidth();
        }
    }, [updateGraphWidth]);

    // handle scroll events
    useEffect(() => {
        // Attach listener to the SCROLL CONTAINER, not the content
        const container = scrollContainerRef.current;
        if (!container) return;

        const handleScroll = () => {
            if (!isPaused) return;
            console.log("scrollLeft", container.scrollLeft, "scrollLeftRef", scrollLeftRef.current, "offset", offsetRef.current);
            // scrollLeftRef.current = container.scrollLeft + offsetRef.current;
            scrollLeftRef.current = container.scrollLeft;
        };

        container.addEventListener("scroll", handleScroll, { passive: true });
        return () => {
            container.removeEventListener("scroll", handleScroll);
        };
    }, [isPaused]);

    const CTXVAL = useMemo<SyncedGraphContext>(() => ({
        scalePxPerSecRef,
        hoverTimestamp,
        timeRangeRef,
        setTimeRange,
        scrollLeftRef
    }), [setTimeRange]);

    return (
        <SyncedGraphContext.Provider value={CTXVAL}>
            {/* outer wrapper handles overflow/scrolling; this the viewport */}
            <div
                ref={scrollContainerRef}
                className="w-full overflow-x-auto overflow-y-hidden min-h-screen pt-14"
            >
                {/* inner content grows in width */}
                <div ref={contentRef} className="min-w-full min-h-60 relative">
                    <div className="sticky left-0 top-0 w-screen">
                        {children}
                    </div>
                </div>
            </div>
        </SyncedGraphContext.Provider >
    );
}
