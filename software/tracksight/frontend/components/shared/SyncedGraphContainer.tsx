import React, {
  createContext,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useRef,
  useState,
} from "react";

type SyncedGraphScrollContextValue = {
  progress: number;
  setProgress: (next: number) => void;
  zoomLevel: number;
  setZoomLevel: (next: number) => void;
  hoverTimestamp: number | null;
  setHoverTimestamp: (ts: number | null) => void;
  registerTimeRange: (id: string, min: number, max: number) => void;
  unregisterTimeRange: (id: string) => void;
  globalTimeRange: { min: number; max: number } | null;
};

const SyncedGraphScrollContext =
  createContext<SyncedGraphScrollContextValue | null>(null);

const DEFAULT_SCROLL_WIDTH = 4000;
const DEFAULT_STRIP_HEIGHT = 64;
const SCROLL_LABEL = "Scroll timeline";

type ScrollControllerProps = {
  onScrollChange: (nextProgress: number) => void;
  progress: number;
  scrollWidth?: number;
  style?: React.CSSProperties;
};

const clamp01 = (value: number) => Math.min(Math.max(value, 0), 1);

function ScrollController({
  onScrollChange,
  progress,
  scrollWidth = DEFAULT_SCROLL_WIDTH,
  style,
}: ScrollControllerProps) {
  const scrollRef = useRef<HTMLDivElement | null>(null);
  const isSyncingRef = useRef(false);

  const handleScroll = useCallback(
    (event: React.UIEvent<HTMLDivElement>) => {
      if (isSyncingRef.current) {
        return;
      }

      const { scrollLeft, scrollWidth, clientWidth } = event.currentTarget;
      const maxScrollLeft = Math.max(scrollWidth - clientWidth, 1);
      const progressValue = clamp01(scrollLeft / maxScrollLeft);
      onScrollChange(progressValue);
    },
    [onScrollChange]
  );

  useEffect(() => {
    const node = scrollRef.current;
    if (!node) return;

    const maxScrollLeft = Math.max(node.scrollWidth - node.clientWidth, 0);
    const desiredScrollLeft = maxScrollLeft * clamp01(progress);

    if (Math.abs(node.scrollLeft - desiredScrollLeft) < 1) {
      return;
    }

    isSyncingRef.current = true;
    node.scrollLeft = desiredScrollLeft;
    // release sync lock on next frame to keep user scroll responsive
    requestAnimationFrame(() => {
      isSyncingRef.current = false;
    });
  }, [progress]);

  return (
    <div
      aria-hidden="true"
      ref={scrollRef}
      onScroll={handleScroll}
      style={{
        ...style,
        overflowX: "scroll",
        overflowY: "hidden",
        cursor: "ew-resize",
        WebkitOverflowScrolling: "touch",
      }}
    >
      <div
        style={{
          width: `${scrollWidth}px`,
          height: "1px",
          pointerEvents: "none",
        }}
      />
    </div>
  );
}

export function useSyncedGraphScroll() {
  const ctx = useContext(SyncedGraphScrollContext);
  if (!ctx) {
    throw new Error(
      "useSyncedGraphScroll must be used within a SyncedGraphContainer"
    );
  }
  return ctx;
}

type SyncedGraphContainerProps = {
  children: React.ReactNode;
  className?: string;
  scrollStripHeight?: number;
  scrollWidth?: number;
  scrollLabel?: string;
};

export default function SyncedGraphContainer({
  children,
  className,
  scrollStripHeight = DEFAULT_STRIP_HEIGHT,
  scrollWidth = DEFAULT_SCROLL_WIDTH,
  scrollLabel = SCROLL_LABEL,
}: SyncedGraphContainerProps) {
  const [progress, setProgress] = useState(1);
  const [zoomLevel, setZoomLevel] = useState(100);
  const [hoverTimestamp, setHoverTimestamp] = useState<number | null>(null);
  const [timeRanges, setTimeRanges] = useState<
    Map<string, { min: number; max: number }>
  >(new Map());

  const rafRef = useRef<number | null>(null);
  const pendingRef = useRef(progress);
  const latestRef = useRef(progress);

  const scheduleProgressUpdate = useCallback((next: number) => {
    const clamped = clamp01(next);
    pendingRef.current = clamped;

    if (Math.abs(clamped - latestRef.current) < 0.0005) {
      return;
    }

    if (rafRef.current !== null) {
      return;
    }

    rafRef.current = requestAnimationFrame(() => {
      rafRef.current = null;
      latestRef.current = pendingRef.current;
      setProgress(pendingRef.current);
    });
  }, []);

  useEffect(() => {
    return () => {
      if (rafRef.current !== null) {
        cancelAnimationFrame(rafRef.current);
      }
    };
  }, []);

  const applyDelta = useCallback(
    (deltaPixels: number) => {
      if (deltaPixels === 0) return;
      const deltaProgress = deltaPixels / scrollWidth;
      const base = pendingRef.current;
      const next = base + deltaProgress;
      scheduleProgressUpdate(next);
    },
    [scrollWidth, scheduleProgressUpdate]
  );

  const handleWheel = useCallback(
    (event: React.WheelEvent<HTMLDivElement>) => {
      const horizontalDelta =
        Math.abs(event.deltaX) >= Math.abs(event.deltaY)
          ? event.deltaX
          : event.shiftKey
          ? event.deltaY
          : 0;

      if (horizontalDelta === 0) {
        return;
      }

      event.preventDefault();
      applyDelta(horizontalDelta);
    },
    [applyDelta]
  );

  const registerTimeRange = useCallback(
    (id: string, min: number, max: number) => {
      setTimeRanges((prev) => {
        const current = prev.get(id);
        // only update if changed significantly to avoid too many re-renders
        // precision of 1ms is fine
        if (
          current &&
          Math.abs(current.min - min) < 1 &&
          Math.abs(current.max - max) < 1
        ) {
          return prev;
        }
        const next = new Map(prev);
        next.set(id, { min, max });
        return next;
      });
    },
    []
  );

  const unregisterTimeRange = useCallback((id: string) => {
    setTimeRanges((prev) => {
      if (!prev.has(id)) return prev;
      const next = new Map(prev);
      next.delete(id);
      return next;
    });
  }, []);

  const globalTimeRange = useMemo(() => {
    if (timeRanges.size === 0) return null;
    let min = Infinity;
    let max = -Infinity;
    for (const range of timeRanges.values()) {
      if (range.min < min) min = range.min;
      if (range.max > max) max = range.max;
    }
    if (min === Infinity || max === -Infinity) return null;
    return { min, max };
  }, [timeRanges]);

  const contextValue = useMemo<SyncedGraphScrollContextValue>(
    () => ({
      progress,
      setProgress: scheduleProgressUpdate,
      zoomLevel,
      setZoomLevel,
      hoverTimestamp,
      setHoverTimestamp,
      registerTimeRange,
      unregisterTimeRange,
      globalTimeRange,
    }),
    [
      progress,
      scheduleProgressUpdate,
      zoomLevel,
      hoverTimestamp,
      registerTimeRange,
      unregisterTimeRange,
      globalTimeRange,
    ]
  );

  const containerClassName = ["relative w-full", className]
    .filter(Boolean)
    .join(" ");

  const trackHeight = Math.max(scrollStripHeight - 32, 16);

  return (
    <SyncedGraphScrollContext.Provider value={contextValue}>
      <div
        className={containerClassName}
        style={{ paddingTop: scrollStripHeight }}
        onWheel={handleWheel}
      >
        <div
          style={{
            position: "absolute",
            top: 0,
            left: 0,
            right: 0,
            height: scrollStripHeight,
            zIndex: 30,
            padding: "8px 16px",
            pointerEvents: "none",
            display: "flex",
            flexDirection: "column",
            gap: 6,
          }}
        >
          <div
            style={{
              fontSize: 11,
              letterSpacing: "0.08em",
              textTransform: "uppercase",
              color: "rgba(226,232,240,0.78)",
            }}
          >
            {scrollLabel}
          </div>
          <div style={{ pointerEvents: "auto" }}>
            <ScrollController
              progress={progress}
              onScrollChange={scheduleProgressUpdate}
              scrollWidth={scrollWidth}
              style={{
                height: trackHeight,
                borderRadius: 999,
                border: "1px solid rgba(148,163,184,0.4)",
                background: "rgba(30,41,59,0.55)",
                boxShadow:
                  "inset 0 0 0 1px rgba(15,23,42,0.35), 0 4px 12px rgba(15,23,42,0.25)",
              }}
            />
          </div>
        </div>
        <div>{children}</div>
      </div>
    </SyncedGraphScrollContext.Provider>
  );
}
