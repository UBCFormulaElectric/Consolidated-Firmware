'use client'

import { createContext, useContext, ReactNode, useCallback } from 'react'
import { Lock, LockOpen } from 'lucide-react'
import { cn } from '@/lib/utils'
import { useLocalState } from '../lib/hooks/useLocalState'

interface DisplayControlContextType {
    isViewportLocked: boolean
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

const VIEWPORT_LOCK_STORAGE_KEY = 'tracksight_viewport_lock_state_v1'

type DisplayControlProviderProps = {
    children: ReactNode
    defaultViewportLocked?: boolean
    viewportLockStorageKey?: string
}

export function DisplayControlProvider({
    children,
    defaultViewportLocked = true,
    viewportLockStorageKey = VIEWPORT_LOCK_STORAGE_KEY,
}: DisplayControlProviderProps) {
    const [isViewportLocked, setIsViewportLocked] = useLocalState<boolean>(
        viewportLockStorageKey,
        defaultViewportLocked
    )

    const toggleViewportLock = useCallback(() => {
        setIsViewportLocked((previousState) => !previousState)
    }, [setIsViewportLocked])

    return (
        <DisplayControlContext.Provider value={{
            isViewportLocked,
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
