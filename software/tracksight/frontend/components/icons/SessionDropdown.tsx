"use client";

import { Clock3, Loader2 } from "lucide-react";
import { useMemo, useState } from "react";

import DropdownTrigger from "@/components/icons/DropdownTrigger";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { HistoricalSession } from "@/lib/api/historicalSessions";
import { cn } from "@/lib/utils";

type SessionDropdownProps = {
    sessions: HistoricalSession[];
    selectedSessionId: string | null;
    isLoading?: boolean;
    error?: Error | null;
    onSessionSelect: (sessionId: string) => void;
};

const SessionDropdown = ({ sessions, selectedSessionId, isLoading = false, error = null, onSessionSelect }: SessionDropdownProps) => {
    const [sessionOpen, setSessionOpen] = useState(false);
    const selectedSession = useMemo(() => sessions.find((session) => session.id === selectedSessionId) ?? null, [selectedSessionId, sessions]);
    const isDisabled = isLoading || sessions.length === 0 || Boolean(error);

    const handleSessionSelect = (sessionId: string) => {
        onSessionSelect(sessionId);
        setSessionOpen(false);
    };

    return (
        <Popover open={sessionOpen} onOpenChange={setSessionOpen}>
            <PopoverTrigger asChild>
                <DropdownTrigger
                    className="w-full"
                    disabled={isDisabled}
                    open={sessionOpen}
                    label="Session"
                    valueClassName="text-base leading-tight"
                    value={isLoading ? "Loading..." : error ? "Unavailable" : (selectedSession?.label ?? "No sessions")}
                    icon={isLoading ? <Loader2 className="size-5 animate-spin" strokeWidth={2.2} /> : <Clock3 className="size-5" strokeWidth={2.2} />}
                />
            </PopoverTrigger>

            <PopoverContent align="start" sideOffset={14} className="w-80 rounded-[1.5rem] border border-gray-200 bg-white p-3 shadow-[0_28px_70px_rgba(15,23,42,0.16)]">
                <div className="grid gap-2">
                    {sessions.map((session) => {
                        const isSelected = session.id === selectedSessionId;

                        return (
                            <button key={session.id} type="button" className={cn("flex items-center justify-between rounded-xl px-4 py-3 text-left transition-colors", isSelected && "bg-blue-600 text-white shadow-[0_8px_18px_rgba(37,99,235,0.28)]", !isSelected && "text-gray-800 hover:bg-gray-100")} onClick={() => handleSessionSelect(session.id)}>
                                <span className="text-base font-semibold">{session.label}</span>
                                <span className={cn("text-xs font-semibold uppercase tracking-[0.16em]", isSelected ? "text-blue-100" : "text-gray-400")}>{session.timeZoneLabel}</span>
                            </button>
                        );
                    })}
                </div>
            </PopoverContent>
        </Popover>
    );
};

export default SessionDropdown;
