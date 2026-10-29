"use client";

import { Lock, LockOpen } from "lucide-react";
import { createContext, ReactNode, useCallback, useContext } from "react";
import { useLocalState } from "../lib/hooks/useLocalState";

interface DisplayControlContextType {
    isViewportLocked: boolean;
    toggleViewportLock: () => void;
}

const DisplayControlContext = createContext<DisplayControlContextType | undefined>(undefined);

export function useDisplayControlContext() {
    const context = useContext(DisplayControlContext);
    if (!context) {
        throw new Error("useDisplayControl must be used within a DisplayControlProvider");
    }
    return context;
}

const VIEWPORT_LOCK_STORAGE_KEY = "tracksight_viewport_lock_state_v1";

type DisplayControlProviderProps = {
    children: ReactNode;
    defaultViewportLocked?: boolean;
    viewportLockStorageKey?: string;
};

export function DisplayControlProvider({ children, defaultViewportLocked = true, viewportLockStorageKey = VIEWPORT_LOCK_STORAGE_KEY }: DisplayControlProviderProps) {
    const [isViewportLocked, setIsViewportLocked] = useLocalState<boolean>(viewportLockStorageKey, defaultViewportLocked);

    const toggleViewportLock = useCallback(() => {
        setIsViewportLocked((previousState) => !previousState);
    }, [setIsViewportLocked]);

    return (
        <DisplayControlContext.Provider
            value={{
                isViewportLocked,
                toggleViewportLock,
            }}
        >
            {children}
        </DisplayControlContext.Provider>
    );
}

export function ViewportLockButton() {
    const { isViewportLocked, toggleViewportLock } = useDisplayControlContext();

    return (
        <button type="button" onClick={toggleViewportLock} className="flex items-center gap-2 rounded-full border border-gray-300 bg-white px-3 py-1.5 text-sm font-medium hover:bg-gray-50 focus-visible:outline-2 focus-visible:outline-blue-600">
            {isViewportLocked ? <Lock className="size-4 text-blue-600" /> : <LockOpen className="size-4 text-amber-600" />}
            <span>{isViewportLocked ? "Following live" : "Browsing history"}</span>
            <span className="border-l border-gray-300 pl-2 text-blue-700">{isViewportLocked ? "Pause follow" : "Jump to live"}</span>
        </button>
    );
}
