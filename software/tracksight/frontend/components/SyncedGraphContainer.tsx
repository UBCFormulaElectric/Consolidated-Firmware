import { AlertHighlight } from "@/lib/alerts";
import { formatTimeSpan } from "@/lib/utils/formatTimeSpan";
import { Search } from "lucide-react";
import { createContext, PointerEvent, ReactNode, RefObject, UIEvent, useCallback, useContext, useEffect, useMemo, useRef, useState } from "react";
import { AlertStatusChip } from "./AlertStatusChip";
import { useDisplayControlContext, ViewportLockButton } from "./PausePlayControl";
import { CHART_PADDING } from "./widgets/render";

export interface TimeRange {
    min: number;
    max: number;
}
export type SyncedGraphContext_t = {
    // internal
    scalePxPerSecRef: RefObject<number>; // a measure of zoom (NOTE: actually px per ms, timestamps are in ms)
    hoverXRef: RefObject<number | null>;
    globalTimeRangeRef: RefObject<TimeRange | null>;
    scrollLeftRef: RefObject<number>;
    isLive: boolean; // follows incoming data rather than showing a fixed historical session
    highlightedAlertRef: RefObject<AlertHighlight | null>; // set by the alert rows on hover, shaded by every chart

    // mutations
    updateWithTimestamp(timestamp: number): void; // NOTE: PLEASE CALL THIS EVERY SINGLE TIME A NEW DATA POINT IS ADDED!!!
    setTimeRange(range: TimeRange, fitViewport?: boolean): void;

    reportAlertStatus(activeCount: number | null): void; // feeds the toolbar chip; callers should only report changes

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
const MIN_SCALE_PX_PER_MS = 0.001;
const MAX_SCALE_PX_PER_MS = 10000;
// browsers stop laying out elements past ~17.9M px (Firefox) / ~33.5M px (Chrome), so cap how wide zoom can make the content
const MAX_CONTENT_WIDTH_PX = 15_000_000;
const WHEEL_ZOOM_SENSITIVITY = 0.005;
const MAX_WHEEL_ZOOM_DELTA = 50; // caps one mouse wheel notch at ~1.28x while leaving small trackpad pinch deltas untouched
const WHEEL_LINE_HEIGHT_PX = 16;

export default function SyncedGraphContainer({ children, initialTimeRange, onViewportSettled }: SyncedGraphContainerProps) {
    const { isViewportLocked } = useDisplayControlContext();

    // object refs
    const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
    const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
    const spanLabelRef = useRef<HTMLSpanElement | null>(null);
    const viewportRef = useRef<HTMLDivElement | null>(null); // sticky, visible-width wrapper every canvas lives in
    const isViewportLockedRef = useRef(isViewportLocked);
    const hasFixedRangeRef = useRef(Boolean(initialTimeRange));
    hasFixedRangeRef.current = Boolean(initialTimeRange);
    const viewportSettleTimeoutRef = useRef<number | null>(null);
    const ignoreProgrammaticScrollRef = useRef(false); //clamping scroll position or fitting the window would cause refetches to jack's api

    // layout work (width growth from live data, wheel/pinch zoom) is coalesced into at most one update per frame
    const layoutFrameRef = useRef<number | null>(null);
    const pendingWidthRef = useRef(false);
    const pendingZoomRef = useRef<{ factor: number; anchorX: number } | null>(null);
    const panRef = useRef<{ pointerId: number; lastX: number } | null>(null);
    const [activeAlertCount, setActiveAlertCount] = useState<number | null>(null);

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

            // the browser rounds scrollLeft, so only expect a scroll event when the position actually moved
            const previousScrollLeft = container.scrollLeft;
            container.scrollLeft = nextScrollLeft;
            if (container.scrollLeft !== previousScrollLeft) {
                ignoreProgrammaticScrollRef.current = true;
            }
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
            if (layoutFrameRef.current !== null) {
                cancelAnimationFrame(layoutFrameRef.current);
            }
        },
        []
    );

    const getScaleBounds = useCallback((container: HTMLDivElement, range: TimeRange) => {
        const span = Math.max(range.max - range.min, 1);
        const fitScale = Math.max(container.clientWidth - LEFT_PAD - RIGHT_PAD, 1) / span;
        const max = Math.min(MAX_SCALE_PX_PER_MS, MAX_CONTENT_WIDTH_PX / span);
        // historical sessions have nothing outside them, so zooming out stops at the whole session
        const min = Math.min(hasFixedRangeRef.current ? fitScale : MIN_SCALE_PX_PER_MS, max);
        return { min, max };
    }, []);

    // Update width when zoom changes
    const updateGraphWidth = useCallback(() => {
        const global_tr = globalTimeRangeRef.current;
        const container = scrollContainerRef.current;
        if (contentRef.current && global_tr && container) {
            // long live sessions keep growing the range; zoom out rather than exceed the browser's max element width
            scalePxPerSecRef.current = Math.min(scalePxPerSecRef.current, getScaleBounds(container, global_tr).max);

            const container_width = scalePxPerSecRef.current * (global_tr.max - global_tr.min);
            contentRef.current.style.width = `${container_width + LEFT_PAD + RIGHT_PAD}px`;

            if (spanLabelRef.current) {
                const spanLabel = formatTimeSpan((container.clientWidth - LEFT_PAD) / scalePxPerSecRef.current);
                if (spanLabelRef.current.textContent !== spanLabel) spanLabelRef.current.textContent = spanLabel;
            }

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
    }, [contentRef, getScaleBounds, globalTimeRangeRef, scalePxPerSecRef, scrollContainerRef, syncContainerScrollLeft]);

    /**
     * Zooms by `factor`, keeping the time under screen-space `anchorX` in place (or the newest data pinned right when locked).
     * Returns whether the scale changed.
     */
    const applyZoom = useCallback(
        (factor: number, anchorX: number) => {
            const container = scrollContainerRef.current;
            const range = globalTimeRangeRef.current;
            if (!container || !range) return false;

            const prevScale = scalePxPerSecRef.current;
            const { min, max } = getScaleBounds(container, range);
            const nextScale = Math.min(Math.max(prevScale * factor, min), max);
            if (nextScale === prevScale) return false;

            if (!isViewportLockedRef.current) {
                // inverse of timeToX: ms from range.min to the anchored time
                const anchorOffsetMs = (anchorX - LEFT_PAD + scrollLeftRef.current) / prevScale;
                scrollLeftRef.current = Math.max(0, anchorOffsetMs * nextScale + LEFT_PAD - anchorX);
            }

            scalePxPerSecRef.current = nextScale;
            updateGraphWidth();
            scheduleViewportSettled();
            return true;
        },
        [getScaleBounds, scheduleViewportSettled, updateGraphWidth]
    );

    const flushPendingLayout = useCallback(() => {
        if (layoutFrameRef.current !== null) {
            cancelAnimationFrame(layoutFrameRef.current);
            layoutFrameRef.current = null;
        }

        const zoom = pendingZoomRef.current;
        const needsWidth = pendingWidthRef.current;
        pendingZoomRef.current = null;
        pendingWidthRef.current = false;

        const zoomed = zoom !== null && applyZoom(zoom.factor, zoom.anchorX);
        if (needsWidth && !zoomed) updateGraphWidth();
    }, [applyZoom, updateGraphWidth]);

    const scheduleLayout = useCallback(() => {
        if (typeof requestAnimationFrame === "undefined" || layoutFrameRef.current !== null) return;
        layoutFrameRef.current = requestAnimationFrame(() => {
            layoutFrameRef.current = null;
            flushPendingLayout();
        });
    }, [flushPendingLayout]);

    const queueZoom = useCallback(
        (factor: number, anchorX: number) => {
            pendingZoomRef.current = { factor: (pendingZoomRef.current?.factor ?? 1) * factor, anchorX };
            scheduleLayout();
        },
        [scheduleLayout]
    );

    const updateWithTimestamp = useCallback(
        (timestamp: number) => {
            if (!globalTimeRangeRef.current || timestamp < globalTimeRangeRef.current.min || timestamp > globalTimeRangeRef.current.max) {
                globalTimeRangeRef.current = {
                    min: Math.min(timestamp, globalTimeRangeRef.current?.min || timestamp),
                    max: Math.max(timestamp, globalTimeRangeRef.current?.max || timestamp),
                };
                pendingWidthRef.current = true;
                scheduleLayout();
            }
        },
        [scheduleLayout]
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
            if (!container) return;
            flushPendingLayout();
            applyZoom(factor, (LEFT_PAD + container.clientWidth) / 2);
        },
        [applyZoom, flushPendingLayout]
    );

    useEffect(() => {
        // wheel/gesture listeners must be non-passive to stop the browser from zooming the whole page
        const container = scrollContainerRef.current;
        if (!container) return;

        const anchorFor = (clientX: number) => clientX - container.getBoundingClientRect().left;

        // Chromium and Firefox report trackpad pinch as ctrl + wheel
        const handleWheel = (event: WheelEvent) => {
            if (!event.ctrlKey) return;
            event.preventDefault();

            const deltaPx = event.deltaMode === WheelEvent.DOM_DELTA_LINE ? event.deltaY * WHEEL_LINE_HEIGHT_PX : event.deltaMode === WheelEvent.DOM_DELTA_PAGE ? event.deltaY * container.clientHeight : event.deltaY;
            const delta = Math.min(Math.max(deltaPx, -MAX_WHEEL_ZOOM_DELTA), MAX_WHEEL_ZOOM_DELTA);
            if (delta === 0) return;

            queueZoom(Math.exp(-delta * WHEEL_ZOOM_SENSITIVITY), anchorFor(event.clientX)); // invert for natural zoom
        };

        // Safari reports trackpad pinch as its non-standard gesture events instead
        let lastGestureScale = 1;
        const handleGestureStart = (event: Event) => {
            event.preventDefault();
            lastGestureScale = 1;
        };
        const handleGestureChange = (event: Event) => {
            event.preventDefault();
            const { scale, clientX } = event as Event & { scale: number; clientX: number };
            queueZoom(scale / lastGestureScale, anchorFor(clientX));
            lastGestureScale = scale;
        };

        container.addEventListener("wheel", handleWheel, { passive: false });
        container.addEventListener("gesturestart", handleGestureStart, { passive: false });
        container.addEventListener("gesturechange", handleGestureChange, { passive: false });
        return () => {
            container.removeEventListener("wheel", handleWheel);
            container.removeEventListener("gesturestart", handleGestureStart);
            container.removeEventListener("gesturechange", handleGestureChange);
        };
    }, [queueZoom]);

    useEffect(() => {
        // Every canvas must span exactly the visible width, or charts spill under the vertical scrollbar and
        // clip their right edge while other canvases don't. clientWidth excludes the scrollbar, whatever its size.
        const container = scrollContainerRef.current;
        const viewport = viewportRef.current;
        if (!container || !viewport) return;

        const observer = new ResizeObserver(() => {
            viewport.style.width = `${container.clientWidth}px`;
            updateGraphWidth(); // keeps the newest data pinned right when following live
        });
        observer.observe(container);
        return () => observer.disconnect();
    }, [updateGraphWidth]);

    // click-and-drag panning for mice; touch and trackpads already pan natively
    const handlePointerDown = useCallback((event: PointerEvent<HTMLDivElement>) => {
        if (event.button !== 0 || event.pointerType !== "mouse" || isViewportLockedRef.current || !(event.target instanceof HTMLCanvasElement)) return;
        event.preventDefault();
        event.currentTarget.setPointerCapture(event.pointerId);
        event.currentTarget.dataset.panning = "";
        panRef.current = { pointerId: event.pointerId, lastX: event.clientX };
        hoverXRef.current = null;
    }, []);

    const handlePointerMove = useCallback((event: PointerEvent<HTMLDivElement>) => {
        const pan = panRef.current;
        if (!pan || pan.pointerId !== event.pointerId) return;
        event.currentTarget.scrollLeft -= event.clientX - pan.lastX; // onScroll picks this up like any user scroll
        pan.lastX = event.clientX;
    }, []);

    const handlePointerUp = useCallback((event: PointerEvent<HTMLDivElement>) => {
        if (panRef.current?.pointerId !== event.pointerId) return;
        panRef.current = null;
        delete event.currentTarget.dataset.panning;
        if (event.currentTarget.hasPointerCapture(event.pointerId)) event.currentTarget.releasePointerCapture(event.pointerId);
    }, []);

    useEffect(() => {
        if (!isViewportLocked) {
            return;
        }

        updateGraphWidth();
    }, [isViewportLocked, updateGraphWidth]);
    // screen space conversions
    // Readers flush pending layout first so every canvas in a frame draws against the same, current scroll/zoom.
    /**
     * given a screen space x, gets the time associated with it based on the current zoom and scroll
     */
    const XToTime = useCallback(
        (x: number) => {
            if (layoutFrameRef.current !== null) flushPendingLayout();
            return (x - LEFT_PAD + scrollLeftRef.current) / scalePxPerSecRef.current + globalTimeRangeRef.current!.min;
        },
        [flushPendingLayout, scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]
    );
    /**
     * given a time, gets the screen space x associated with it based on the current zoom and scroll
     */
    const timeToX = useCallback(
        (t: number) => {
            if (layoutFrameRef.current !== null) flushPendingLayout();
            return (t - globalTimeRangeRef.current!.min) * scalePxPerSecRef.current - scrollLeftRef.current + LEFT_PAD;
        },
        [flushPendingLayout, scrollLeftRef, scalePxPerSecRef, globalTimeRangeRef]
    );

    // hover
    const hoverXRef = useRef<number | null>(null);
    const highlightedAlertRef = useRef<AlertHighlight | null>(null);

    // context
    const CTXVAL = useMemo<SyncedGraphContext_t>(
        () => ({
            scalePxPerSecRef,
            hoverXRef,
            globalTimeRangeRef,
            updateWithTimestamp,
            setTimeRange,
            scrollLeftRef,
            isLive: !initialTimeRange,
            reportAlertStatus: setActiveAlertCount,
            highlightedAlertRef,
            timeToX,
            XToTime,
        }),
        [scalePxPerSecRef, hoverXRef, globalTimeRangeRef, updateWithTimestamp, setTimeRange, scrollLeftRef, initialTimeRange, timeToX, XToTime]
    );

    return (
        <SyncedGraphContext.Provider value={CTXVAL}>
            <div className="flex h-full flex-col">
                <div className="grid shrink-0 grid-cols-[1fr_auto_1fr] items-center gap-3 border-b border-gray-200 px-4 py-2">
                    {/* lives outside the scroll area so active alerts stay visible while reading charts further down.
                        the jump isn't smooth: following live rewrites scrollLeft every frame, which cancels smooth scrolls */}
                    <div className="justify-self-start">
                        <AlertStatusChip activeCount={activeAlertCount} isLive={!initialTimeRange} onClick={() => scrollContainerRef.current?.scrollTo({ top: 0 })} />
                    </div>
                    <div className="flex items-center rounded-full border border-gray-300 bg-white" title="Zoom · or Ctrl + scroll / pinch over the charts">
                        <Search className="ml-3 size-4 text-gray-500" aria-hidden />
                        <button type="button" onClick={() => zoomBy(1 / 1.5)} className="px-3 py-1 text-sm hover:bg-gray-50" aria-label="Zoom out">
                            −
                        </button>
                        <span ref={spanLabelRef} className="min-w-16 text-center text-sm tabular-nums" aria-label="Visible time window" />
                        <button type="button" onClick={() => zoomBy(1.5)} className="rounded-r-full px-3 py-1 text-sm hover:bg-gray-50" aria-label="Zoom in">
                            +
                        </button>
                    </div>
                    <div className="flex items-center gap-2 justify-self-end">
                        {initialTimeRange && (
                            <button type="button" onClick={() => setTimeRange(initialTimeRange, true)} className="rounded border px-3 py-1.5 text-sm hover:bg-gray-50">
                                Fit session
                            </button>
                        )}
                        {!initialTimeRange && <ViewportLockButton />}
                    </div>
                </div>
                <div ref={scrollContainerRef} className={isViewportLocked ? "min-h-0 w-full flex-1 overflow-x-hidden overflow-y-scroll" : "min-h-0 w-full flex-1 overflow-x-auto overflow-y-scroll [&_canvas]:cursor-grab data-panning:select-none data-panning:[&_canvas]:cursor-grabbing"} style={{ overscrollBehaviorX: "contain" }} onScroll={updateLeftScroll} onPointerDown={handlePointerDown} onPointerMove={handlePointerMove} onPointerUp={handlePointerUp} onPointerCancel={handlePointerUp}>
                    <div ref={contentRef} className="min-w-full relative">
                        {/* width tracks the container's visible width (see the ResizeObserver above), never 100vw */}
                        <div ref={viewportRef} className="sticky left-0">
                            {children}
                        </div>
                    </div>
                </div>
            </div>
        </SyncedGraphContext.Provider>
    );
}
