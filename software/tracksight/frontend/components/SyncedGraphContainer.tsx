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
  const hoverTimestamp = useRef<number | null>(null);
  const scrollContainerRef = useRef<HTMLDivElement | null>(null); // NEW: Ref for the scrolling wrapper
  const contentRef = useRef<HTMLDivElement | null>(null); // Renamed from containerRef, this one grows
  const scrollLeftRef = useRef<number>(0);
  const timeRangeRef = useRef<TimeRange | null>(null);
  
  // zoom management (attach wheel listener to the scroll container)
  const scalePxPerSec = useZoomManager(scrollContainerRef);
  const scalePxPerSecRef = useRef<number>(scalePxPerSec);
  
  const updateGraphWidth = useCallback(() => {
    const tr = timeRangeRef.current;
    if (contentRef.current && tr) {
      const width = scalePxPerSecRef.current * (tr.max - tr.min);
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
    //console.log("called setTimeRange", tr);
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
      scrollLeftRef.current = container.scrollLeft;
    };

    container.addEventListener("scroll", handleScroll, { passive: true });
    return () => {
      container.removeEventListener("scroll", handleScroll);
    };
  }, []);

  // measure viewport width for sticky container
  /*useEffect(() => {
    const container = containerRef.current;
    if (!container) return;
    const observer = new ResizeObserver((entries) => {
      for (const entry of entries) {
        if (entry.contentRect) {
          //setViewportWidth(entry.contentRect.width);
          console.log("viewport width", entry.contentRect.width);
        }
      }
    });
    observer.observe(container);
    return () => observer.disconnect();
  }, []);*/

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
        className="w-full overflow-x-auto overflow-y-hidden"
      >
        {/* inner content grows in width */}
        <div 
          ref={contentRef}
          className="min-w-full min-h-60 relative" 
          style={{ width: 4000 }} 
        >
          <div className="sticky left-0 top-0 w-screen">
            {children}
          </div>
        </div>
      </div>
    </SyncedGraphContext.Provider >
  );
}
