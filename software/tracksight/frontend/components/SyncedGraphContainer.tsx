import { createContext, useCallback, useContext, useEffect, useMemo, useRef, useState, RefObject } from "react";

type SyncedGraphContext = {
  scalePxPerSecRef: RefObject<number>;
  hoverTimestamp: RefObject<number | null>;
  timeRangeRef: RefObject<TimeRange | null>;
  setTimeRange: (tr: TimeRange) => void;
};
// eslint-disable-next-line no-redeclare
const SyncedGraphContext = createContext<SyncedGraphContext | null>(null);

export function useSyncedGraph() {
  const ctx = useContext(SyncedGraphContext);
  if (!ctx) {
    throw new Error("useSyncedGraph must be used within a SyncedGraphContainer");
  }
  return ctx;
}

interface TimeRange {
  min: number;
  max: number;
}

const MIN_SCALE_PX_PER_SEC = 10;
const MAX_SCALE_PX_PER_SEC = 1000;

function useZoomManager(containerRef: RefObject<HTMLDivElement | null>) {
  const [scalePxPerSec, setScalePxPerSec] = useState(100);

  const handleWheelZoom = useCallback((event: WheelEvent) => {
    if (event.ctrlKey === false) {
      return;
    }
    event.preventDefault();
    const deltaScale = event.deltaY * -0.001; // invert for natural zoom
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
  }, [handleWheelZoom]);

  return scalePxPerSec;
}

export default function SyncedGraphContainer({ children }: { children: React.ReactNode }) {
  const hoverTimestamp = useRef<number | null>(null);
  const containerRef = useRef<HTMLDivElement | null>(null);

  const [timeRange, setTimeRange] = useState<TimeRange | null>({ min: 0, max: 60 });
  const timeRangeRef = useRef(timeRange);
  useEffect(() => { timeRangeRef.current = timeRange; }, [timeRange]);

  // zoom management
  const scalePxPerSec = useZoomManager(containerRef);
  const scalePxPerSecRef = useRef<number>(scalePxPerSec);
  const graphWidthPx = useMemo(() => {
    scalePxPerSecRef.current = scalePxPerSec;
    return timeRange ? scalePxPerSec * (timeRange.max - timeRange.min) : null;
  }, [timeRange, scalePxPerSec]);

  const CTXVAL = useMemo<SyncedGraphContext>(() => ({
    scalePxPerSecRef,
    hoverTimestamp,
    timeRangeRef,
    setTimeRange
  }), [setTimeRange]);

  return (
    <SyncedGraphContext.Provider value={CTXVAL}>
      <div className="min-w-full min-h-60" style={{ width: graphWidthPx || 4000 }} ref={containerRef}>
        <div className="sticky w-screen left-0">
          {children}
        </div>
      </div>
    </SyncedGraphContext.Provider >
  );
}
