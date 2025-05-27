'use client'

import React, { createContext, useContext, useState, ReactNode } from 'react'
import { Play, Pause } from 'lucide-react'
import { cn } from '@/lib/utils'

// Context for global pause/play state
interface PausePlayContextType {
  isPaused: boolean
  togglePause: () => void
}

const PausePlayContext = createContext<PausePlayContextType | undefined>(undefined)

// Export the context for use in other components
export { PausePlayContext, type PausePlayContextType }

// Provider component to wrap the entire app
export function PausePlayProvider({ children }: { children: ReactNode }) {
  const [isPaused, setIsPaused] = useState(true) // Start in paused state

  const togglePause = () => {
    setIsPaused(prev => !prev)
  }

  return (
    <PausePlayContext.Provider value={{ isPaused, togglePause }}>
      {children}
    </PausePlayContext.Provider>
  )
}

// Hook to use the pause/play state
export function usePausePlay() {
  const context = useContext(PausePlayContext)
  if (!context) {
    throw new Error('usePausePlay must be used within a PausePlayProvider')
  }
  return context
}

// The circular play/pause button component
export function PausePlayButton() {
  const { isPaused, togglePause } = usePausePlay()

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
