'use client'

import { createContext, useContext, useState, useEffect, ReactNode, Dispatch, SetStateAction, useCallback } from 'react'
import { Play, Pause } from 'lucide-react'
import { cn } from '@/lib/utils'

// Context for global display control state (pause/play and visual settings)
interface DisplayControlContextType {
  isPaused: boolean
  togglePause: () => void
  horizontalScale: number
  setHorizontalScale: (scale: number) => void
  isAutoscrollEnabled: boolean
  toggleAutoscroll: () => void
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

// TODO move to a util folder?
function useLocalState<T>(name: string, defaultValue: T): [T, Dispatch<SetStateAction<T>>] {
  const [state, setState] = useState<T>(defaultValue);
  useEffect(() => {
    if (typeof window === "undefined") {
      return;
    }
    const saved = localStorage.getItem(name);
    if (saved !== null) setState(JSON.parse(saved));
  }, [])

  const setLocalState: Dispatch<SetStateAction<T>> = (value) => {
    localStorage.setItem(name, JSON.stringify(value));
    setState(value);
  }

  return [state, setLocalState];
}

const PAUSE_STATE_STORAGE_KEY = "tracksight_pause_state_v1";
// Provider component to wrap the entire app
export function DisplayControlProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useLocalState<boolean>(PAUSE_STATE_STORAGE_KEY, true);
  const [horizontalScale, setHorizontalScale] = useState(100)
  const [isAutoscrollEnabled, setIsAutoscrollEnabled] = useState(false)

  useEffect(() => {
    if (typeof window !== "undefined") {
      localStorage.setItem(PAUSE_STATE_STORAGE_KEY, JSON.stringify(isPaused));
    }
  }, [isPaused]);

  const togglePause = useCallback(() => { setIsPaused(prev => !prev) }, [setIsPaused])
  const toggleAutoscroll = useCallback(() => { setIsAutoscrollEnabled(prev => !prev) }, [setIsAutoscrollEnabled])

  return (
    <DisplayControlContext.Provider value={{
      isPaused,
      togglePause,
      horizontalScale,
      setHorizontalScale,
      isAutoscrollEnabled,
      toggleAutoscroll
    }}>
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
        "size-16 p-4 rounded-full flex items-center justify-center transition-all duration-200 shadow-lg border-2 hover:scale-105 active:scale-95 cursor-pointer",
        isPaused
          ? "bg-green-500 border-green-600 hover:bg-green-600 text-white"
          : "bg-red-500 border-red-600 hover:bg-red-600 text-white"
      )}
      title={isPaused ? "Resume data updates" : "Pause data updates"}
    >
      {isPaused ? (
        <Play className="w-full h-full" fill="currentColor" />
      ) : (
        <Pause className="w-full h-full" fill="currentColor" />
      )}
    </button>
  )
}
