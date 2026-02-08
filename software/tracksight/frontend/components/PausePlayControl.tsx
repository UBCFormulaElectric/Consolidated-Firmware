'use client'

import { createContext, useContext, useState, useEffect, ReactNode, Dispatch, SetStateAction, useCallback, useMemo, useRef, RefObject } from 'react'
import { Play, Pause } from 'lucide-react'
import { cn } from '@/lib/utils'
import type { DownsampleInfo, DownsampleOptions } from '@/components/widgets/render'

// Context for global display control state (pause/play and visual settings)
interface DisplayControlContextType {
  isPaused: boolean
  togglePause: () => void
  horizontalScale: number
  setHorizontalScale: (scale: number) => void
  isAutoscrollEnabled: boolean
  toggleAutoscroll: () => void
  downsampleOptions: DownsampleOptions
  setDownsampleOptions: Dispatch<SetStateAction<DownsampleOptions>>
  downsampleInfoRef: RefObject<DownsampleInfo | null>
}
const DisplayControlContext = createContext<DisplayControlContextType | undefined>(undefined)
// Hook to use the display control state
export function useDisplayControl() {
  const context = useContext(DisplayControlContext)
  if (!context) {
    throw new Error('useDisplayControl must be used within a DisplayControlProvider')
  }
  return context
}
// Maintain backward compatibility
export const usePausePlay = useDisplayControl

// TODO move to a util folder?
function useLocalState<T>(name: string, defaultValue: T): [T, Dispatch<SetStateAction<T>>] {
  const [state, setState] = useState<T>(defaultValue);
  useEffect(() => {
    if (typeof window === "undefined") {
      return;
    }
    const saved = localStorage.getItem(name);
    if (saved === null) {
      return;
    }
    try {
      setState(JSON.parse(saved));
    } catch {
      localStorage.removeItem(name);
      setState(defaultValue);
    }
  }, [])

  const setLocalState: Dispatch<SetStateAction<T>> = (action) => {
    if (typeof window === "undefined") {
      setState(action);
      return;
    }

    setState((prev) => {
      const next =
        typeof action === "function"
          ? (action as (prevState: T) => T)(prev)
          : action;
      localStorage.setItem(name, JSON.stringify(next));
      return next;
    });
  };

  return [state, setLocalState];
}

const PAUSE_STATE_STORAGE_KEY = "tracksight_pause_state_v1";
const DOWNSAMPLE_OPTIONS_STORAGE_KEY = "tracksight_downsample_options_v1";

const DEFAULT_DOWNSAMPLE_OPTIONS: DownsampleOptions = {
  minVisiblePoints: 5_000,
  minPointsPerPixel: 8,
};
// Provider component to wrap the entire app
export function DisplayControlProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useLocalState<boolean>(PAUSE_STATE_STORAGE_KEY, true);
  const [horizontalScale, setHorizontalScale] = useState(100)
  const [isAutoscrollEnabled, setIsAutoscrollEnabled] = useState(false)
  const [downsampleOptions, setDownsampleOptions] = useLocalState<DownsampleOptions>(
    DOWNSAMPLE_OPTIONS_STORAGE_KEY,
    DEFAULT_DOWNSAMPLE_OPTIONS
  );

  const downsampleInfoRef = useRef<DownsampleInfo | null>(null);

  useEffect(() => {
    if (typeof window !== "undefined") {
      localStorage.setItem(PAUSE_STATE_STORAGE_KEY, JSON.stringify(isPaused));
    }
  }, [isPaused]);

  const togglePause = useCallback(() => { setIsPaused(prev => !prev) }, [setIsPaused])
  const toggleAutoscroll = useCallback(() => { setIsAutoscrollEnabled(prev => !prev) }, [setIsAutoscrollEnabled])

  const ctxValue = useMemo<DisplayControlContextType>(() => ({
    isPaused,
    togglePause,
    horizontalScale,
    setHorizontalScale,
    isAutoscrollEnabled,
    toggleAutoscroll,
    downsampleOptions,
    setDownsampleOptions,
    downsampleInfoRef,
  }), [
    isPaused,
    togglePause,
    horizontalScale,
    isAutoscrollEnabled,
    toggleAutoscroll,
    downsampleOptions,
    setDownsampleOptions,
  ]);

  return (
    <DisplayControlContext.Provider value={ctxValue}>
      {children}
    </DisplayControlContext.Provider>
  )
}

// The circular play/pause button component
export function PausePlayButton() {
  const { isPaused, togglePause } = useDisplayControl()
  return (
    <button
      onClick={togglePause}
      className={cn(
        "w-10 h-10 rounded-full flex items-center justify-center transition-all duration-200 shadow-lg border-2 hover:scale-105 active:scale-95",
        isPaused
          ? "bg-green-500 border-green-600 hover:bg-green-600 text-white"
          : "bg-red-500 border-red-600 hover:bg-red-600 text-white"
      )}
      title={isPaused ? "Resume data updates" : "Pause data updates"}
    >
      {isPaused ? (
        <Play className="w-5 h-5 ml-0.5" fill="currentColor" />
      ) : (
        <Pause className="w-5 h-5" fill="currentColor" />
      )}
    </button>
  )
}

export function DownsampleControls() {
  const { downsampleOptions, setDownsampleOptions, downsampleInfoRef } = useDisplayControl();
  const [open, setOpen] = useState(false);
  const [info, setInfo] = useState<DownsampleInfo | null>(null);

  useEffect(() => {
    const id = window.setInterval(() => {
      setInfo(downsampleInfoRef.current);
    }, 250);
    return () => window.clearInterval(id);
  }, [downsampleInfoRef]);

  const visible = info?.visiblePoints ?? 0;
  const rendered = info?.renderedPoints ?? 0;
  const mode = info?.downsampled ? "downsampled" : "full";
  const seriesLabel = info?.seriesLabel;

  return (
    <div className="flex items-center gap-2">
      <button
        type="button"
        onClick={() => setOpen(v => !v)}
        className={cn(
          "px-2 py-1 rounded border text-xs font-medium",
          open ? "bg-gray-900 text-white border-gray-900" : "bg-white text-gray-900 border-gray-300 hover:bg-gray-50"
        )}
        title="Downsampling settings"
      >
        Downsample
      </button>

      <div className="text-xs text-gray-600 whitespace-nowrap">
        {seriesLabel ? <span className="mr-2 text-gray-800">{seriesLabel}</span> : null}
        <span>visible {visible.toLocaleString()} pts</span>
        <span className="mx-1">·</span>
        <span>rendered ~{rendered.toLocaleString()} ({mode})</span>
      </div>

      {open ? (
        <div className="flex items-center gap-3 px-2 py-1 rounded border bg-white shadow-sm">
          <label className="flex items-center gap-2 text-xs text-gray-700">
            min pts
            <input
              className="w-24 px-2 py-1 border rounded text-xs"
              type="number"
              min={0}
              step={1000}
              value={downsampleOptions.minVisiblePoints}
              onChange={(e) => {
                const v = Number(e.target.value);
                if (!Number.isFinite(v)) return;
                setDownsampleOptions(prev => ({ ...prev, minVisiblePoints: Math.max(0, Math.floor(v)) }));
              }}
            />
          </label>

          <label className="flex items-center gap-2 text-xs text-gray-700">
            min pts/px
            <input
              className="w-16 px-2 py-1 border rounded text-xs"
              type="number"
              min={1}
              step={1}
              value={downsampleOptions.minPointsPerPixel}
              onChange={(e) => {
                const v = Number(e.target.value);
                if (!Number.isFinite(v)) return;
                setDownsampleOptions(prev => ({ ...prev, minPointsPerPixel: Math.max(1, v) }));
              }}
            />
          </label>
        </div>
      ) : null}
    </div>
  );
}
