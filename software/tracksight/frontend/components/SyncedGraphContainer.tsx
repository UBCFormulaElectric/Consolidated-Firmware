import { createContext, useCallback, useContext, useEffect, useMemo, useRef, useState, RefObject, ReactNode } from "react";
import { useDisplayControl } from "./PausePlayControl";

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
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const offsetRef = useRef<number>(0);
    const updateGraphWidth = useCallback(() => {
        const tr = timeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && tr && container) {
            const width = scalePxPerSecRef.current * (tr.max - tr.min) + RIGHT_PAD; // extra padding by 1min
            const containerWidth = container.clientWidth;

            scrollLeftRef.current = (width - RIGHT_PAD + 15 > containerWidth) ? width - RIGHT_PAD + 15 - containerWidth : 0;
            offsetRef.current = scrollLeftRef.current;

            container.scrollLeft = scrollLeftRef.current;
            contentRef.current.style.width = `${width}px`;
        }
    }, []);
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper

    // zoom management (attach wheel listener to the scroll container)
    const scalePxPerSec = useZoomManager(scrollContainerRef);
    const scalePxPerSecRef = useRef<number>(scalePxPerSec);
    // Update width when zoom changes
    useEffect(() => {
        scalePxPerSecRef.current = scalePxPerSec;
        updateGraphWidth();
    }, [scalePxPerSec, updateGraphWidth]);

    const hoverTimestamp = useRef<number | null>(null);
    const timeRangeRef = useRef<TimeRange | null>(null);
    // update timeRange and width without triggering a re-render
    const setTimeRange = useCallback((tr: TimeRange) => {
        const current = timeRangeRef.current;
        if (!current || current.min !== tr.min || current.max !== tr.max) {
            timeRangeRef.current = tr;
            updateGraphWidth();
        }
    }, [updateGraphWidth]);

    // manage left scroll variable
    const scrollLeftRef = useRef<number>(0);
    const { isPaused } = useDisplayControl();
    useEffect(() => { // handle scroll events (needs to be here because of passive: true)
        const container = scrollContainerRef.current;
        if (!container) return;
        const handleScroll = () => {
            if (!isPaused) return;
            scrollLeftRef.current = container.scrollLeft;
        };
        container.addEventListener("scroll", handleScroll, { passive: true });
        return () => { container.removeEventListener("scroll", handleScroll) }
    }, [isPaused]);


    // context
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
            <div ref={scrollContainerRef} className="w-full overflow-x-auto overflow-y-auto h-full graph-scrollbar">
                {/* inner content grows in width */}
                <div ref={contentRef} className="min-w-full relative">
                    <div className="sticky left-0"
                        style={{
                            width: `calc(100vw - 18px)` // this is the set width of the scrollbar (global.css)
                        }}
                    >
                        {children}
                    </div>
                </div>
            </div>
        </SyncedGraphContext.Provider >
    );
}
