import { createContext, useCallback, useContext, useEffect, useMemo, useRef, useState, RefObject, ReactNode } from "react";

export type SyncedGraphContext = {
  scalePxPerSecRef: RefObject<number>;
  hoverTimestamp: RefObject<number | null>;
  timeRangeRef: RefObject<TimeRange | null>;
  setTimeRange: (tr: TimeRange) => void;
  scrollLeftRef: RefObject<number>;
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

export interface TimeRange {
  min: number;
  max: number;
}

const MIN_SCALE_PX_PER_SEC = 0;
const MAX_SCALE_PX_PER_SEC = 10000;

function useZoomManager(containerRef: RefObject<HTMLDivElement | null>) {
  const [scalePxPerSec, setScalePxPerSec] = useState(100);

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
  const hoverTimestamp = useRef<number | null>(null);
  const containerRef = useRef<HTMLDivElement | null>(null);
  const scrollLeftRef = useRef<number>(0);
  const [viewportWidth, setViewportWidth] = useState(0);

  const [timeRange, setTimeRange] = useState<TimeRange | null>(null);
  const timeRangeRef = useRef(timeRange);
  useEffect(() => { timeRangeRef.current = timeRange; }, [timeRange]);

  // zoom management
  const scalePxPerSec = useZoomManager(containerRef);
  const scalePxPerSecRef = useRef<number>(scalePxPerSec);
  
  const graphWidthPx = useMemo(() => {
    scalePxPerSecRef.current = scalePxPerSec;
    return timeRange ? scalePxPerSec * (timeRange.max - timeRange.min) : null;
  }, [timeRange, scalePxPerSec]);

  // Handle scroll events
  useEffect(() => {
    const container = containerRef.current;
    if (!container) return;

    const handleScroll = () => {
      scrollLeftRef.current = container.scrollLeft;
    };

    container.addEventListener("scroll", handleScroll, { passive: true });
    return () => {
      container.removeEventListener("scroll", handleScroll);
    };
  }, []);

  // Measure viewport width for sticky container
  useEffect(() => {
    const container = containerRef.current;
    if (!container) return;
    const observer = new ResizeObserver((entries) => {
      for (const entry of entries) {
        if (entry.contentRect) {
          setViewportWidth(entry.contentRect.width);
        }
      }
    });
    observer.observe(container);
    return () => observer.disconnect();
  }, []);

  const CTXVAL = useMemo<SyncedGraphContext>(() => ({
    scalePxPerSecRef,
    hoverTimestamp,
    timeRangeRef,
    setTimeRange,
    scrollLeftRef
  }), [setTimeRange]);

  return (
    <SyncedGraphContext.Provider value={CTXVAL}>
      <div 
        className="w-full overflow-x-auto overflow-y-hidden" 
        ref={containerRef}
      >
        <div style={{ width: graphWidthPx || '100%', minWidth: '100%' }} className="relative">
          <div 
            className="sticky left-0 top-0" 
            style={{ width: viewportWidth > 0 ? viewportWidth : '100%' }}
          >
            {children}
          </div>
        </div>
      </div>
    </SyncedGraphContext.Provider >
  );
}
