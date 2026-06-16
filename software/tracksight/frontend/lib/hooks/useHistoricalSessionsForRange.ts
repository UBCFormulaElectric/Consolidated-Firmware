import { useMemo } from "react";

import { useQuery } from "@tanstack/react-query";

import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { fetchHistoricalSessionsForRange, HistoricalSession } from "@/lib/api/historicalSessions";

const toTimezoneDateKey = (utcMs: number, timeZone: string) => {
    return new Intl.DateTimeFormat("en-CA", { timeZone, year: "numeric", month: "2-digit", day: "2-digit" }).format(new Date(utcMs));
};

export function useHistoricalSessionsForRange(startUtcMs: number, endUtcMs: number, source: HistoricalSignalSource, timeZone: string, enabled = true) {
    const query = useQuery({
        queryKey: ["historical-sessions-range", startUtcMs, endUtcMs, source, timeZone],
        queryFn: async () => fetchHistoricalSessionsForRange(startUtcMs, endUtcMs, source, timeZone),
        enabled,
        staleTime: 5 * 60 * 1000,
        gcTime: 30 * 60 * 1000,
        retry: (failureCount) => failureCount < 2,
    });

    const sessionsByDay = useMemo(() => {
        const grouped = new Map<string, HistoricalSession[]>();
        for (const session of query.data ?? []) {
            const key = toTimezoneDateKey(session.startUtcMs, timeZone);
            const bucket = grouped.get(key);
            if (bucket) {
                bucket.push(session);
            } else {
                grouped.set(key, [session]);
            }
        }
        return grouped;
    }, [query.data, timeZone]);

    const daysWithSessions = useMemo(() => new Set(sessionsByDay.keys()), [sessionsByDay]);

    return { query, sessionsByDay, daysWithSessions };
}
