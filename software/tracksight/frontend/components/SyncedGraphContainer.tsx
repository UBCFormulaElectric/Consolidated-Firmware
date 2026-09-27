import { createContext, ReactNode, RefObject, UIEvent, useCallback, useContext, useEffect, useMemo, useRef } from "react";
import { useDisplayControlContext, ViewportLockButton } from "./PausePlayControl";
import { CHART_PADDING } from "./widgets/render";

export interface TimeRange {
    min: number;
    max: number;
}
export type SyncedGraphContext_t = {
    // internal
    scalePxPerSecRef: RefObject<number>; // a measure of zoom
    hoverXRef: RefObject<number | null>;
    globalTimeRangeRef: RefObject<TimeRange | null>;
    scrollLeftRef: RefObject<number>;

    // mutations
    updateWithTimestamp(timestamp: number): void; // NOTE: PLEASE CALL THIS EVERY SINGLE TIME A NEW DATA POINT IS ADDED!!!
    setTimeRange(range: TimeRange, fitViewport?: boolean): void;

    // transformations
    timeToX(t: number): number;
    XToTime(x: number): number;
};

type SyncedGraphContainerProps = {
    children: ReactNode;
    initialTimeRange?: TimeRange;
    onViewportSettled?: (range: TimeRange) => void;
};

const SyncedGraphContext = createContext<SyncedGraphContext_t | null>(null);
export function useSyncedGraph() {
    const ctx = useContext(SyncedGraphContext);
    if (!ctx) {
        throw new Error("useSyncedGraph must be used within a SyncedGraphContainer");
    }
    return ctx;
}

const RIGHT_PAD = 10;
const LEFT_PAD = CHART_PADDING.left;
const MIN_SCALE_PX_PER_SEC = 0.001;
const MAX_SCALE_PX_PER_SEC = 10000;

export default function SyncedGraphContainer({ children, initialTimeRange, onViewportSettled }: SyncedGraphContainerProps) {
    const { isViewportLocked } = useDisplayControlContext();

    // object refs
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
    const isViewportLockedRef = useRef(isViewportLocked);
    const viewportSettleTimeoutRef = useRef<number | null>(null);
    const widthUpdateFrameRef = useRef<number | null>(null);
    const ignoreProgrammaticScrollRef = useRef(false); //clamping scroll position or fitting the window would cause refetches to jack's api

    // zoom management
    const scalePxPerSecRef = useRef<number>(1);

    // glokbal time range
    const globalTimeRangeRef = useRef<TimeRange | null>(null);
    // manage left scroll variable
    const scrollLeftRef = useRef<number>(0);

    const getViewportRange = useCallback((): TimeRange | null => {
        const container = scrollContainerRef.current;
        const globalTimeRange = globalTimeRangeRef.current;

        if (!container || !globalTimeRange) {
            return null;
        }

        const min = scrollLeftRef.current / scalePxPerSecRef.current + globalTimeRange.min;
        const max = (scrollLeftRef.current + container.clientWidth) / scalePxPerSecRef.current + globalTimeRange.min;

        return { min, max };
    }, [globalTimeRangeRef, scalePxPerSecRef, scrollContainerRef, scrollLeftRef]);

    const scheduleViewportSettled = useCallback(() => {
        if (!onViewportSettled) {
            return;
        }

        if (viewportSettleTimeoutRef.current !== null) {
            window.clearTimeout(viewportSettleTimeoutRef.current);
        }

        viewportSettleTimeoutRef.current = window.setTimeout(() => {
            const viewportRange = getViewportRange();
            if (viewportRange) {
                onViewportSettled(viewportRange);
            }
        }, 200);
    }, [getViewportRange, onViewportSettled]);

    const syncContainerScrollLeft = useCallback(
        (container: HTMLDivElement, nextScrollLeft: number) => {
            scrollLeftRef.current = nextScrollLeft;

            if (container.scrollLeft === nextScrollLeft) {
                return;
            }

            ignoreProgrammaticScrollRef.current = true;
            container.scrollLeft = nextScrollLeft;
        },
        [scrollLeftRef]
    );

    useEffect(() => {
        isViewportLockedRef.current = isViewportLocked;
    }, [isViewportLocked]);

    useEffect(
        () => () => {
            if (viewportSettleTimeoutRef.current !== null) {
                window.clearTimeout(viewportSettleTimeoutRef.current);
            }
            if (widthUpdateFrameRef.current !== null) {
                cancelAnimationFrame(widthUpdateFrameRef.current);
            }
        },
        []
    );

    // Update width when zoom changes
    const updateGraphWidth = useCallback(() => {
        const global_tr = globalTimeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && global_tr && container) {
            // ponytail: live history grows scroll width; window it if long sessions hit browser limits.
            const container_width = scalePxPerSecRef.current * (global_tr.max - global_tr.min);
            contentRef.current.style.width = `${container_width + LEFT_PAD + RIGHT_PAD}px`;

            if (isViewportLockedRef.current) {
                // When locked, scroll to show the rightmost data at the right edge of the viewport
                const lockedScrollLeft = Math.max(container_width + LEFT_PAD - container.clientWidth, 0);
                syncContainerScrollLeft(container, lockedScrollLeft);
                return;
            }

            // When unlocked, just clamp to valid bounds
            const maxScrollLeft = Math.max(container_width + LEFT_PAD + RIGHT_PAD - container.clientWidth, 0);
            const clampedScrollLeft = Math.min(scrollLeftRef.current, maxScrollLeft);
            syncContainerScrollLeft(container, clampedScrollLeft);
        }
    }, [contentRef, globalTimeRangeRef, scalePxPerSecRef, scrollContainerRef, syncContainerScrollLeft]);

    const scheduleGraphWidth = useCallback(() => {
        if (typeof requestAnimationFrame === "undefined" || widthUpdateFrameRef.current !== null) return;
        widthUpdateFrameRef.current = requestAnimationFrame(() => {
            widthUpdateFrameRef.current = null;
            updateGraphWidth();
        });
    }, [updateGraphWidth]);

    const updateWithTimestamp = useCallback(
        (timestamp: number) => {
            if (!globalTimeRangeRef.current || timestamp < globalTimeRangeRef.current.min || timestamp > globalTimeRangeRef.current.max) {
                globalTimeRangeRef.current = {
                    min: Math.min(timestamp, globalTimeRangeRef.current?.min || timestamp),
                    max: Math.max(timestamp, globalTimeRangeRef.current?.max || timestamp),
                };
                scheduleGraphWidth();
            }
        },
        [scheduleGraphWidth]
    );

    const setTimeRange = useCallback(
        (range: TimeRange, fitViewport = false) => {
            globalTimeRangeRef.current = range;

            if (fitViewport) {
                const container = scrollContainerRef.current;
                if (container) {
                    const timeRange = Math.max(range.max - range.min, 1);
                    const availableWidth = Math.max(container.clientWidth - LEFT_PAD - RIGHT_PAD, 1);
                    scalePxPerSecRef.current = availableWidth / timeRange;
                    syncContainerScrollLeft(container, 0);
                }
            }

            updateGraphWidth();
        },
        [globalTimeRangeRef, scalePxPerSecRef, scrollContainerRef, syncContainerScrollLeft, updateGraphWidth]
    );

    useEffect(() => {
        // init for historical graphs
        if (!initialTimeRange) {
            globalTimeRangeRef.current = null;
            updateGraphWidth();
            return;
        }

        setTimeRange(initialTimeRange, true);
    }, [initialTimeRange, setTimeRange, updateGraphWidth]);

    const updateLeftScroll = useCallback(
        (e: UIEvent<HTMLDivElement>) => {
            const container = e.target as HTMLDivElement;
            const previousScrollLeft = scrollLeftRef.current;

            if (ignoreProgrammaticScrollRef.current) {
                ignoreProgrammaticScrollRef.current = false;
                scrollLeftRef.current = container.scrollLeft;
                return;
            }

            if (isViewportLockedRef.current) {
                const latestScrollLeft = Math.max(container.scrollWidth - container.clientWidth, 0);
                if (container.scrollLeft !== latestScrollLeft) {
                    syncContainerScrollLeft(container, latestScrollLeft);
                    return;
                }
                scrollLeftRef.current = latestScrollLeft;
                return;
            }

            scrollLeftRef.current = container.scrollLeft;
            if (container.scrollLeft !== previousScrollLeft) {
                scheduleViewportSettled();
            }
        },
        [scheduleViewportSettled, scrollLeftRef, syncContainerScrollLeft]
    );

    const zoomBy = useCallback(
        (factor: number) => {
            const container = scrollContainerRef.current;
            if (!container || !globalTimeRangeRef.current) return;
            const prevScale = scalePxPerSecRef.current;
            const nextScale = Math.min(Math.max(prevScale * factor, MIN_SCALE_PX_PER_SEC), MAX_SCALE_PX_PER_SEC);
            if (nextScale === prevScale) return;

            if (!isViewportLockedRef.current) {
                const viewportCenter = scrollLeftRef.current + container.clientWidth / 2;
                const centerTime = viewportCenter / prevScale;
                const newCenterPos = centerTime * nextScale;
                const newScrollLeft = newCenterPos - container.clientWidth / 2;
                scrollLeftRef.current = Math.max(0, newScrollLeft);
            }

            scalePxPerSecRef.current = nextScale;
            updateGraphWidth();
            scheduleViewportSettled();
        },
        [scheduleViewportSettled, updateGraphWidth]
    );

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
    const XToTime = useCallback((x: number) => (x - LEFT_PAD + scrollLeftRef.current) / scalePxPerSecRef.current + globalTimeRangeRef.current!.min, [scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]);
    /**
     * given a time, gets the screen space x associated with it based on the current zoom and scroll
     */
    const timeToX = useCallback((t: number) => (t - globalTimeRangeRef.current!.min) * scalePxPerSecRef.current - scrollLeftRef.current + LEFT_PAD, [scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]);

    // hover
    const hoverXRef = useRef<number | null>(null);

    // context
    const CTXVAL = useMemo<SyncedGraphContext_t>(
        () => ({
            scalePxPerSecRef,
            hoverXRef,
            globalTimeRangeRef,
            updateWithTimestamp,
            setTimeRange,
            scrollLeftRef,
            timeToX,
            XToTime,
        }),
        [scalePxPerSecRef, hoverXRef, globalTimeRangeRef, updateWithTimestamp, setTimeRange, scrollLeftRef, timeToX, XToTime]
    );

    return (
        <SyncedGraphContext.Provider value={CTXVAL}>
            <div className="flex h-full flex-col">
                <div className="flex shrink-0 items-center justify-between gap-3 border-b border-gray-200 px-4 py-2">
                    <span className="hidden text-xs text-gray-500 sm:inline">{isViewportLocked ? "Pause follow to browse" : "Scroll sideways to browse"}</span>
                    <div className="flex items-center gap-2">
                        {initialTimeRange && (
                            <button type="button" onClick={() => setTimeRange(initialTimeRange, true)} className="rounded border px-3 py-1.5 text-sm hover:bg-gray-50">
                                Fit session
                            </button>
                        )}
                        <button type="button" onClick={() => zoomBy(1 / 1.5)} className="rounded border px-3 py-1.5 text-sm hover:bg-gray-50" aria-label="Zoom out">
                            −
                        </button>
                        <span className="text-sm">Zoom</span>
                        <button type="button" onClick={() => zoomBy(1.5)} className="rounded border px-3 py-1.5 text-sm hover:bg-gray-50" aria-label="Zoom in">
                            +
                        </button>
                        {!initialTimeRange && <ViewportLockButton />}
                    </div>
                </div>
                <div ref={scrollContainerRef} className={isViewportLocked ? "min-h-0 w-full flex-1 overflow-x-hidden overflow-y-scroll" : "min-h-0 w-full flex-1 overflow-x-auto overflow-y-scroll"} style={{ overscrollBehaviorX: "contain" }} onScroll={updateLeftScroll}>
                    <div ref={contentRef} className="min-w-full relative">
                        <div
                            className="sticky left-0"
                            style={{
                                width: `calc(100vw - 18px)`, // this is the set width of the scrollbar (global.css)
                            }}
                        >
                            {children}
                        </div>
                    </div>
                </div>
            </div>
        </SyncedGraphContext.Provider>
    );
}
