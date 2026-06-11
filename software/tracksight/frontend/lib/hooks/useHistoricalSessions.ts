import { useQuery } from "@tanstack/react-query";

import { HistoricalSignalSource } from "@/lib/api/historicalSignals";
import { fetchHistoricalSessionsForDate } from "@/lib/api/historicalSessions";

export function useHistoricalSessions(dateKey: string, source: HistoricalSignalSource) {
    return useQuery({
        queryKey: ["historical-sessions", dateKey, source],
        queryFn: async () => fetchHistoricalSessionsForDate(dateKey, source),
        staleTime: 5 * 60 * 1000,
        gcTime: 30 * 60 * 1000,
        retry: (failureCount) => failureCount < 2,
    });
}
