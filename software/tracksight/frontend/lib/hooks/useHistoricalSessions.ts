import { useQuery } from "@tanstack/react-query";

import { fetchHistoricalSessionsForDate } from "@/lib/api/historicalSessions";

export function useHistoricalSessions(dateKey: string) {
    return useQuery({
        queryKey: ["historical-sessions", dateKey],
        queryFn: async () => fetchHistoricalSessionsForDate(dateKey),
        staleTime: 5 * 60 * 1000,
        gcTime: 30 * 60 * 1000,
        retry: (failureCount) => failureCount < 2,
    });
}
