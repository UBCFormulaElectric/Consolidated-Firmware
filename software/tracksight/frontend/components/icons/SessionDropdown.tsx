"use client";

import { ChevronDown, Clock3, Loader2 } from "lucide-react";
import { useMemo, useState } from "react";

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
                <button type="button" disabled={isDisabled} className={cn("flex min-w-72 items-center justify-between gap-4 rounded-3xl border border-gray-200 bg-white px-5 py-4 text-left shadow-[0_14px_40px_rgba(15,23,42,0.12)] transition-colors hover:bg-gray-50", isDisabled && "cursor-not-allowed opacity-70", !isDisabled && "cursor-pointer")}>
                    <div className="flex items-center gap-4">
                        <div className="flex size-11 items-center justify-center rounded-2xl bg-gray-100 text-gray-900">{isLoading ? <Loader2 className="size-6 animate-spin" strokeWidth={2.2} /> : <Clock3 className="size-6" strokeWidth={2.2} />}</div>
                        <div>
                            <div className="text-sm font-semibold uppercase tracking-[0.16em] text-gray-500">Session</div>
                            <div className="text-[1.45rem] font-semibold leading-none text-gray-950">{isLoading ? "Loading..." : error ? "Unavailable" : (selectedSession?.label ?? "No sessions")}</div>
                        </div>
                    </div>
                    <ChevronDown className={cn("size-6 shrink-0 text-gray-500 transition-transform", sessionOpen && "rotate-180")} strokeWidth={2.25} />
                </button>
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
