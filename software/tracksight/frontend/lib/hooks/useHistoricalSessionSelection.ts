import { useEffect, useMemo, useState } from "react";

import { HistoricalSession } from "@/lib/api/historicalSessions";
import { useHistoricalSessions } from "@/lib/hooks/useHistoricalSessions";

/**
 * Owns the "which session is selected for this date" logic: fetches the
 * sessions for a date key and keeps a selected id that auto-falls-back to the
 * first available session whenever the list changes (e.g. the date changes).
 *
 * Call it once per independent selection surface (e.g. once for the live
 * dashboard, once for the draft state inside the selection modal).
 */
export function useHistoricalSessionSelection(dateKey: string) {
    const query = useHistoricalSessions(dateKey);
    const sessions = useMemo(() => query.data ?? [], [query.data]);
    const [selectedSessionId, setSelectedSessionId] = useState<string | null>(null);

    useEffect(() => {
        if (sessions.length === 0) {
            return;
        }

        setSelectedSessionId((currentSessionId) => (sessions.some((session) => session.id === currentSessionId) ? currentSessionId : sessions[0].id));
    }, [sessions]);

    const selectedSession = useMemo<HistoricalSession | null>(() => sessions.find((session) => session.id === selectedSessionId) ?? null, [selectedSessionId, sessions]);

    return { query, sessions, selectedSessionId, setSelectedSessionId, selectedSession };
}
