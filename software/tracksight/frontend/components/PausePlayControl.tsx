'use client'

import { createContext, useContext, ReactNode, useCallback } from 'react'
import { Lock, LockOpen, Pause, Play } from 'lucide-react'
import { cn } from '@/lib/utils'
import { useLocalState } from '../lib/hooks/useLocalState'

interface DisplayControlContextType {
    isPaused: boolean
    isViewportLocked: boolean
    togglePause: () => void
    toggleViewportLock: () => void
}

const DisplayControlContext = createContext<DisplayControlContextType | undefined>(undefined)

export function useDisplayControlContext() {
    const context = useContext(DisplayControlContext)
    if (!context) {
        throw new Error('useDisplayControl must be used within a DisplayControlProvider')
    }
    return context
}

const PAUSE_STATE_STORAGE_KEY = 'tracksight_pause_state_v2'
const VIEWPORT_LOCK_STORAGE_KEY = 'tracksight_viewport_lock_state_v1'

export function DisplayControlProvider({ children }: { children: ReactNode }) {
    const [isPaused, setIsPaused] = useLocalState<boolean>(PAUSE_STATE_STORAGE_KEY, false)
    const [isViewportLocked, setIsViewportLocked] = useLocalState<boolean>(VIEWPORT_LOCK_STORAGE_KEY, true)

    const togglePause = useCallback(() => {
        setIsPaused((previousState) => !previousState)
    }, [setIsPaused])

    const toggleViewportLock = useCallback(() => {
        setIsViewportLocked((previousState) => !previousState)
    }, [setIsViewportLocked])

    return (
        <DisplayControlContext.Provider value={{
            isPaused,
            isViewportLocked,
            togglePause,
            toggleViewportLock,
        }}>
            {children}
        </DisplayControlContext.Provider>
    )
}

function ControlButton(props: {
    className: string
    onClick: () => void
    title: string
    children: ReactNode
}) {
    return (
        <button
            onClick={props.onClick}
            className={cn(
                "size-16 p-4 rounded-full flex items-center justify-center transition-all duration-200 shadow-lg border-2 hover:scale-105 active:scale-95 cursor-pointer",
                props.className
            )}
            title={props.title}
        >
            {props.children}
        </button>
    )
}

export function ViewportLockButton() {
    const { isViewportLocked, toggleViewportLock } = useDisplayControlContext()

    return (
        <ControlButton
            onClick={toggleViewportLock}
            className={isViewportLocked ? "bg-blue-500 border-blue-600 hover:bg-blue-600 text-white" : "bg-amber-500 border-amber-600 hover:bg-amber-600 text-white"}
            title={isViewportLocked ? "Unlock viewport" : "Lock viewport to newest data"}
        >
            {isViewportLocked ? <Lock className="w-full h-full" /> : <LockOpen className="w-full h-full" />}
        </ControlButton>
    )
}
