'use client'

import React, { createContext, useContext, useState, useEffect, ReactNode } from 'react'
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

const PAUSE_STATE_STORAGE_KEY = "tracksight_pause_state_v1";

// retain pause state between refreshes
function loadPauseStateFromStorage(): boolean {
  if (typeof window === "undefined") return true; 
  const saved = localStorage.getItem(PAUSE_STATE_STORAGE_KEY);
  if (saved === null) return true; 
  try {
    return JSON.parse(saved); 
  } catch {
    return true; 
  }
}

// Provider component to wrap the entire app
export function DisplayControlProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useState(() => loadPauseStateFromStorage());
  const [horizontalScale, setHorizontalScale] = useState(100) // Default to 100%
  const [isAutoscrollEnabled, setIsAutoscrollEnabled] = useState(false) // Default to disabled

  useEffect(() => {
    if (typeof window !== "undefined") {
      localStorage.setItem(PAUSE_STATE_STORAGE_KEY, JSON.stringify(isPaused));
    }
  }, [isPaused]);

  const togglePause = () => {
    setIsPaused(prev => !prev)
  }

  const toggleAutoscroll = () => {
    setIsAutoscrollEnabled(prev => !prev)
  }

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

// Maintain backward compatibility
export const PausePlayProvider = DisplayControlProvider

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
