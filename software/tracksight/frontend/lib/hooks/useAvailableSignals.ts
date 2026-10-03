import { useQuery } from "@tanstack/react-query";

import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL } from "@/lib/constants";

const STALE_TIME_MS = 5 * 60 * 1000;
const GARBAGE_COLLECTION_TIME_MS = 30 * 60 * 1000;
const MAX_FETCH_RETRIES = 2;

export function useAvailableSignals() {
    return useQuery({
        queryKey: ["available-signals"],
        queryFn: async () => fetchSignalMetadata(API_BASE_URL),
        select: (signals) => [...signals].sort((left, right) => left.name.localeCompare(right.name)),
        staleTime: STALE_TIME_MS,
        gcTime: GARBAGE_COLLECTION_TIME_MS,
        retry: (failureCount) => failureCount < MAX_FETCH_RETRIES,
    });
}
