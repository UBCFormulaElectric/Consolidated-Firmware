"use client";

import { HardDrive, RadioIcon } from "lucide-react";
import { createContext, useCallback, useContext, useMemo, useState, type ReactNode } from "react";

import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { HistoricalSession } from "@/lib/api/historicalSessions";

export const SOURCE_OPTIONS: { value: HistoricalSignalSource; label: string; description: string; Icon: typeof RadioIcon }[] = [
    {
        value: "Radio",
        label: "Radio",
        description: "Radio data",
        Icon: RadioIcon,
    },
    {
        value: "SdCard",
        label: "SD Card",
        description: "Dumped logger data",
        Icon: HardDrive,
    },
];

export const sourceLabelFor = (source: HistoricalSignalSource) => SOURCE_OPTIONS.find((option) => option.value === source)?.label ?? "Historical";

export type SelectionStep = 1 | 2 | 3;

type TimeRange = { min: number; max: number };

type HistoricalSelectionContextValue = {
    source: HistoricalSignalSource;
    selectedSession: HistoricalSession | null;
    selectedRange: TimeRange | null;
    sourceLabel: string;

    isModalOpen: boolean;
    initialStep: SelectionStep;
    setModalOpen: (open: boolean) => void;
    openModal: (step?: SelectionStep) => void;
    
    applySelection: (source: HistoricalSignalSource, session: HistoricalSession) => void;

    isSyncing: boolean;
    setSyncing: (isSyncing: boolean) => void;
};

const HistoricalSelectionContext = createContext<HistoricalSelectionContextValue | null>(null);

export function HistoricalSelectionProvider({ children }: { children: ReactNode }) {
    const [source, setSource] = useState<HistoricalSignalSource>("Radio");
    const [selectedSession, setSelectedSession] = useState<HistoricalSession | null>(null);
    const [isModalOpen, setModalOpen] = useState(false);
    const [initialStep, setInitialStep] = useState<SelectionStep>(1);
    const [isSyncing, setSyncing] = useState(false);

    const selectedRange = useMemo<TimeRange | null>(() => (selectedSession ? { min: selectedSession.startUtcMs, max: selectedSession.endUtcMs } : null), [selectedSession]);

    const openModal = useCallback((step: SelectionStep = 1) => {
        setInitialStep(step);
        setModalOpen(true);
    }, []);

    const applySelection = useCallback((nextSource: HistoricalSignalSource, session: HistoricalSession) => {
        setSource(nextSource);
        setSelectedSession(session);
        setModalOpen(false);
    }, []);

    const value = useMemo<HistoricalSelectionContextValue>(
        () => ({
            source,
            selectedSession,
            selectedRange,
            sourceLabel: sourceLabelFor(source),
            isModalOpen,
            initialStep,
            setModalOpen,
            openModal,
            applySelection,
            isSyncing,
            setSyncing,
        }),
        [source, selectedSession, selectedRange, isModalOpen, initialStep, openModal, applySelection, isSyncing]
    );

    return <HistoricalSelectionContext.Provider value={value}>{children}</HistoricalSelectionContext.Provider>;
}

export function useHistoricalSelection() {
    const context = useContext(HistoricalSelectionContext);
    if (!context) {
        throw new Error("useHistoricalSelection must be used within a HistoricalSelectionProvider");
    }
    return context;
}
