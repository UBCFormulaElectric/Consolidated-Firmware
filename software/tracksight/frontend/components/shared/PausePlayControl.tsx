'use client'

import React, { createContext, useContext, useState, ReactNode } from 'react'
import { Play, Pause } from 'lucide-react'

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

// Provider component to wrap the entire app
export function DisplayControlProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useState(true) // Start in paused state
  const [horizontalScale, setHorizontalScale] = useState(100) // Default to 100%
  const [isAutoscrollEnabled, setIsAutoscrollEnabled] = useState(false) // Default to disabled

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

export function PausePlayButton() {
  const { isPaused, togglePause } = useDisplayControl()

  return (
    <button
      onClick={togglePause}
      className="w-10 h-10 rounded-full flex items-center justify-center transition-all duration-200 shadoww -lg border-2 hover:scale-105 hover:cursor-pointer"
      title={isPaused ? "Resume data updates" : "Pause data updates"}
    >
      {isPaused ? (
        <Play className="w-5 h-5 fill-current" />
      ) : (
        <Pause className="w-5 h-5 fill-current" />
      )}
    </button>
  )
}
