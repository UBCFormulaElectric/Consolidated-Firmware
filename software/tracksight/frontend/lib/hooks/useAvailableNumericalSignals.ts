import { useQuery } from "@tanstack/react-query";

import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL } from "@/lib/constants";
import { isNumericalSignalMetadata, NumericalSignalMetadata } from "@/lib/types/Signal";

export function useAvailableNumericalSignals() {
    return useQuery({
        queryKey: ["available-numerical-signals"],
        queryFn: async () => fetchSignalMetadata(API_BASE_URL),
        select: (signals) => signals.filter(isNumericalSignalMetadata).sort((left, right) => left.name.localeCompare(right.name)) as NumericalSignalMetadata[],
        staleTime: 5 * 60 * 1000,
        gcTime: 30 * 60 * 1000,
        retry: (failureCount) => failureCount < 2,
    });
}
