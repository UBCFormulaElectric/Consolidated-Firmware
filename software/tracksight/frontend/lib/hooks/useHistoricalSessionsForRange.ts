import { useMemo } from "react";

import { useQuery } from "@tanstack/react-query";

import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { fetchHistoricalSessionsForRange, HistoricalSession } from "@/lib/api/historicalSessions";

const toUtcDateKey = (utcMs: number) => {
    const date = new Date(utcMs);
    const year = date.getUTCFullYear();
    const month = String(date.getUTCMonth() + 1).padStart(2, "0");
    const day = String(date.getUTCDate()).padStart(2, "0");
    return `${year}-${month}-${day}`;
};

export function useHistoricalSessionsForRange(startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource, enabled = true) {
    const query = useQuery({
        queryKey: ["historical-sessions-range", startUtcMs, endUtcMs, source],
        queryFn: async () => fetchHistoricalSessionsForRange(startUtcMs, endUtcMs, source),
        enabled,
        staleTime: 5 * 60 * 1000,
        gcTime: 30 * 60 * 1000,
        retry: (failureCount) => failureCount < 2,
    });

    const sessionsByDay = useMemo(() => {
        const grouped = new Map<string, HistoricalSession[]>();
        for (const session of query.data ?? []) {
            const key = toUtcDateKey(session.startUtcMs);
            const bucket = grouped.get(key);
            if (bucket) {
                bucket.push(session);
            } else {
                grouped.set(key, [session]);
            }
        }
        return grouped;
    }, [query.data]);

    const daysWithSessions = useMemo(() => new Set(sessionsByDay.keys()), [sessionsByDay]);

    return { query, sessionsByDay, daysWithSessions };
}
