import { createContext, useCallback, useContext, useEffect, useMemo, useRef, useState, RefObject, ReactNode, UIEvent } from "react";
import { useDisplayControlContext } from "./PausePlayControl";

export interface TimeRange {
    min: number;
    max: number;
}
export type SyncedGraphContext = {
    scalePxPerSecRef: RefObject<number>; // a measure of zoom
    hoverTimestamp: RefObject<number | null>; // TODO maybe not here?
    globalTimeRangeRef: RefObject<TimeRange | null>;
    updateWithTimestamp(timestamp: number): void; // NOTE: PLEASE CALL THIS EVERY SINGLE TIME A NEW DATA POINT IS ADDED!!!

    scrollLeftRef: RefObject<number>;

    timeToX(t: number): number;
    XToTime(x: number): number;
};
// eslint-disable-next-line no-redeclare
const SyncedGraphContext = createContext<SyncedGraphContext | null>(null);
const RIGHT_PAD = 5 * 60; // 5 secs right 
const SCROLL_PAD = 15;

export function useSyncedGraph() {
    const ctx = useContext(SyncedGraphContext);
    if (!ctx) {
        throw new Error("useSyncedGraph must be used within a SyncedGraphContainer");
    }
    return ctx;
}

const MIN_SCALE_PX_PER_SEC = 0;
const MAX_SCALE_PX_PER_SEC = 10000;

function useZoomPanManager(containerRef: RefObject<HTMLDivElement | null>) {
    const scalePxPerSecRef = useRef<number>(1);

    const handleWheelZoom = useCallback((event: WheelEvent) => {
        if (event.ctrlKey === false) {
            return;
        }
        event.preventDefault();
        const deltaScale = event.deltaY * -0.005; // invert for natural zoom 
        if (deltaScale === 0) return;

        // deltaScale usually in {-1,1}
        scalePxPerSecRef.current = Math.min(Math.max(scalePxPerSecRef.current * Math.exp(deltaScale), MIN_SCALE_PX_PER_SEC), MAX_SCALE_PX_PER_SEC);
    }, [scalePxPerSecRef]);

    useEffect(() => {
        const container = containerRef.current;
        if (!container) return;

        window.addEventListener("wheel", handleWheelZoom, { passive: false });
        return () => {
            window.removeEventListener("wheel", handleWheelZoom);
        };
    }, [handleWheelZoom, containerRef]);

    // suppress scroll when playing
    const { isPaused } = useDisplayControlContext();
    useEffect(() => { // handle scroll events (needs to be here because of passive: false)
        const container = containerRef.current;
        if (!container) return;
        const suppressScroll = (e: Event) => {
            if (!isPaused) {
                e.preventDefault();
            }
        }
        container.addEventListener("wheel", suppressScroll, { passive: false });
        container.addEventListener("mousewheel", suppressScroll, { passive: false });
        return () => {
            container.removeEventListener("wheel", suppressScroll);
            container.removeEventListener("mousewheel", suppressScroll);
        };
    }, [containerRef, isPaused]);

    return scalePxPerSecRef;
}

export default function SyncedGraphContainer({ children }: { children: ReactNode }) {
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
    const hoverTimestamp = useRef<number | null>(null);

    const globalTimeRangeRef = useRef<TimeRange | null>(null);
    // Update width when zoom changes
    const updateGraphWidth = useCallback(() => {
        const global_tr = globalTimeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && global_tr && container) {
            const container_width = scalePxPerSecRef.current * (global_tr.max - global_tr.min);
            container.scrollLeft = Math.max(container_width + SCROLL_PAD - container.clientWidth, 0);
            contentRef.current.style.width = `${container_width + RIGHT_PAD}px`;
        }
    }, []);
    const updateWithTimestamp = useCallback((timestamp: number) => {
        if (!globalTimeRangeRef.current || timestamp < globalTimeRangeRef.current.min || timestamp > globalTimeRangeRef.current.max) {
            globalTimeRangeRef.current = {
                min: Math.min(timestamp, globalTimeRangeRef.current?.min || timestamp),
                max: Math.max(timestamp, globalTimeRangeRef.current?.max || timestamp)
            };
            updateGraphWidth();
        }
    }, [updateGraphWidth])

    // zoom management (attach wheel listener to the scroll container)
    const scalePxPerSecRef = useZoomPanManager(scrollContainerRef);

    // manage left scroll variable
    const scrollLeftRef = useRef<number>(0);
    const updateLeftScroll = useCallback((e: UIEvent<HTMLDivElement>) => {
        scrollLeftRef.current = (e.target as HTMLDivElement).scrollLeft;
    }, [scrollLeftRef]);

    // screen space conversions
    /**
     * given a screen space x, gets the time associated with it based on the current zoom and scroll
     */
    const XToTime = useCallback((x: number) =>
        (x + scrollLeftRef.current) / scalePxPerSecRef.current + globalTimeRangeRef.current!.min
        , [scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]);
    /**
     * given a time, gets the screen space x associated with it based on the current zoom and scroll
     */
    const timeToX = useCallback((t: number) =>
        (t - globalTimeRangeRef.current!.min) * scalePxPerSecRef.current - scrollLeftRef.current
        , [scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]);


    // context
    const CTXVAL = useMemo<SyncedGraphContext>(() => ({
        scalePxPerSecRef,
        hoverTimestamp,
        globalTimeRangeRef,
        updateWithTimestamp,
        scrollLeftRef,
        timeToX,
        XToTime
    }), [updateWithTimestamp, scrollLeftRef, timeToX, XToTime]);

    return (
        <SyncedGraphContext.Provider value={CTXVAL}>
            {/* outer wrapper handles overflow/scrolling; this the viewport */}
            <div ref={scrollContainerRef} className="w-full overflow-x-auto overflow-y-scroll h-full" onScroll={updateLeftScroll}>
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
