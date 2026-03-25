import { createContext, useCallback, useContext, useEffect, useMemo, useRef, RefObject, ReactNode, UIEvent } from "react";
import { useDisplayControlContext } from "./PausePlayControl";

export interface TimeRange {
    min: number;
    max: number;
}
export type SyncedGraphContext_t = {
    // internal
    scalePxPerSecRef: RefObject<number>; // a measure of zoom
    hoverTimestampRef: RefObject<number | null>; // TODO maybe not here?
    globalTimeRangeRef: RefObject<TimeRange | null>;
    scrollLeftRef: RefObject<number>;

    // mutations
    updateWithTimestamp(timestamp: number): void; // NOTE: PLEASE CALL THIS EVERY SINGLE TIME A NEW DATA POINT IS ADDED!!!

    // transformations
    timeToX(t: number): number;
    XToTime(x: number): number;
};
const SyncedGraphContext = createContext<SyncedGraphContext_t | null>(null);
export function useSyncedGraph() {
    const ctx = useContext(SyncedGraphContext);
    if (!ctx) {
        throw new Error("useSyncedGraph must be used within a SyncedGraphContainer");
    }
    return ctx;
}

const RIGHT_PAD = 5 * 60; // 5 secs right 
const SCROLL_PAD = 15;
const MIN_SCALE_PX_PER_SEC = 0.001;
const MAX_SCALE_PX_PER_SEC = 10000;

function useSuppressScrollWhileLocked(containerRef: RefObject<HTMLDivElement | null>) {
    const { isViewportLocked } = useDisplayControlContext();

    useEffect(() => { // handle scroll events (needs to be here because of passive: false)
        const container = containerRef.current;
        if (!container) return;

        const suppressScroll = (e: WheelEvent) => {
            if (isViewportLocked && Math.abs(e.deltaX) > 0 && !e.ctrlKey) {
                e.preventDefault();
            }
        }

        container.addEventListener("wheel", suppressScroll, { passive: false });

        return () => {
            container.removeEventListener("wheel", suppressScroll);
        };
    }, [containerRef, isViewportLocked]);
}

export default function SyncedGraphContainer({ children }: { children: ReactNode }) {
    const { isViewportLocked } = useDisplayControlContext();

    // object refs
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
    const isViewportLockedRef = useRef(isViewportLocked);

    // zoom management
    const scalePxPerSecRef = useRef<number>(1);
    useSuppressScrollWhileLocked(scrollContainerRef);

    // glokbal time range
    const globalTimeRangeRef = useRef<TimeRange | null>(null);
    // manage left scroll variable
    const scrollLeftRef = useRef<number>(0);

    useEffect(() => {
        isViewportLockedRef.current = isViewportLocked;
    }, [isViewportLocked]);

    // Update width when zoom changes
    const updateGraphWidth = useCallback(() => {
        const global_tr = globalTimeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && global_tr && container) {
            const container_width = scalePxPerSecRef.current * (global_tr.max - global_tr.min);
            const nextScrollLeft = Math.max(container_width + SCROLL_PAD - container.clientWidth, 0);
            contentRef.current.style.width = `${container_width + RIGHT_PAD}px`;

            if (isViewportLockedRef.current) {
                scrollLeftRef.current = nextScrollLeft;
                container.scrollLeft = nextScrollLeft;
                return;
            }

            const clampedScrollLeft = Math.min(scrollLeftRef.current, nextScrollLeft);
            scrollLeftRef.current = clampedScrollLeft;
            if (container.scrollLeft !== clampedScrollLeft) {
                container.scrollLeft = clampedScrollLeft;
            }
        }
    }, [contentRef, globalTimeRangeRef, scalePxPerSecRef, scrollContainerRef, scrollLeftRef]);

    const updateWithTimestamp = useCallback((timestamp: number) => {
        if (!globalTimeRangeRef.current || timestamp < globalTimeRangeRef.current.min || timestamp > globalTimeRangeRef.current.max) {
            globalTimeRangeRef.current = {
                min: Math.min(timestamp, globalTimeRangeRef.current?.min || timestamp),
                max: Math.max(timestamp, globalTimeRangeRef.current?.max || timestamp)
            };
            updateGraphWidth();
        }
    }, [updateGraphWidth])

    const updateLeftScroll = useCallback((e: UIEvent<HTMLDivElement>) => {
        const container = e.target as HTMLDivElement;

        if (isViewportLockedRef.current) {
            const latestScrollLeft = Math.max(container.scrollWidth - container.clientWidth, 0);
            if (container.scrollLeft !== latestScrollLeft) {
                container.scrollLeft = latestScrollLeft;
            }
            scrollLeftRef.current = latestScrollLeft;
            return;
        }

        scrollLeftRef.current = container.scrollLeft;
    }, [scrollLeftRef]);

    useEffect(() => {
        const container = scrollContainerRef.current;
        if (!container) return;

        const handleWheelZoom = (event: WheelEvent) => {
            if (!event.ctrlKey) {
                return;
            }

            event.preventDefault();
            const deltaScale = event.deltaY * -0.005; // invert for natural zoom
            if (deltaScale === 0) return;

            const nextScale = Math.min(
                Math.max(scalePxPerSecRef.current * Math.exp(deltaScale), MIN_SCALE_PX_PER_SEC),
                MAX_SCALE_PX_PER_SEC
            );
            if (nextScale === scalePxPerSecRef.current) {
                return;
            }

            scalePxPerSecRef.current = nextScale;
            updateGraphWidth();
        };

        container.addEventListener("wheel", handleWheelZoom, { passive: false });
        return () => {
            container.removeEventListener("wheel", handleWheelZoom);
        };
    }, [scrollContainerRef, scalePxPerSecRef, updateGraphWidth]);

    useEffect(() => {
        if (!isViewportLocked) {
            return;
        }

        updateGraphWidth();
    }, [isViewportLocked, updateGraphWidth]);

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

    // hover
    const hoverTimestampRef = useRef<number | null>(null);

    // context
    const CTXVAL = useMemo<SyncedGraphContext_t>(() => ({
        scalePxPerSecRef,
        hoverTimestampRef,
        globalTimeRangeRef,
        updateWithTimestamp,
        scrollLeftRef,
        timeToX,
        XToTime
    }), [scalePxPerSecRef, hoverTimestampRef, globalTimeRangeRef, updateWithTimestamp, scrollLeftRef, timeToX, XToTime]);

    return (
        <SyncedGraphContext.Provider value={CTXVAL}>
            {/* outer wrapper handles overflow/scrolling; this the viewport */}
            <div ref={scrollContainerRef} className={isViewportLocked ? "w-full overflow-x-hidden overflow-y-scroll h-full" : "w-full overflow-x-auto overflow-y-scroll h-full"} onScroll={updateLeftScroll}>
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
