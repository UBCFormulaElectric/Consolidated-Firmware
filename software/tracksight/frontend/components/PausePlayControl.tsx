'use client'

import { createContext, useContext, useEffect, ReactNode, useCallback } from 'react'
import { Play, Pause } from 'lucide-react'
import { cn } from '@/lib/utils'
import { useLocalState } from '../lib/hooks/useLocalState'

// Context for global display control state (pause/play and visual settings)
interface DisplayControlContextType {
  isPaused: boolean
  togglePause: () => void
}
const DisplayControlContext = createContext<DisplayControlContextType | undefined>(undefined)
// Hook to use the display control state
export function useDisplayControlContext() {
  const context = useContext(DisplayControlContext)
  if (!context) {
    throw new Error('useDisplayControl must be used within a DisplayControlProvider')
  }
  return context
}

const PAUSE_STATE_STORAGE_KEY = "tracksight_pause_state_v1";
// Provider component to wrap the entire app
export function DisplayControlProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useLocalState<boolean>(PAUSE_STATE_STORAGE_KEY, true);
  useEffect(() => {
    if (typeof window !== "undefined") {
      localStorage.setItem(PAUSE_STATE_STORAGE_KEY, JSON.stringify(isPaused));
    }
  }, [isPaused]);
  const togglePause = useCallback(() => { setIsPaused(prev => !prev) }, [setIsPaused])
  return (
    <DisplayControlContext.Provider value={{
      isPaused,
      togglePause,
    }}>
      {children}
    </DisplayControlContext.Provider>
  )
}

// The circular play/pause button component
export function PausePlayButton() {
  const { isPaused, togglePause } = useDisplayControlContext()
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
