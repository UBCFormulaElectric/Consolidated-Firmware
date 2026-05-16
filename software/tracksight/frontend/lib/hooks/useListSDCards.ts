import { queryOptions, useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";

const fetchSDCards = async () => {
    const response = await fetch(`${API_BASE_URL}/api/mock/sd/list`);

    if (!response.ok) {
        throw new Error(`Failed to fetch sd cards: ${response.statusText}`);
    }

    return (await response.json()) as string[];
};

export const listSDCardsQueryOptions = () =>
    queryOptions({
        queryKey: ["sd-cards"],
        queryFn: fetchSDCards,
    });

/**
 * Hook to list all currently mounted sd cards.
 *
 * @returns React Query result with the list of sd card paths.
 */
export default function useListSDCards() {
    return useQuery(listSDCardsQueryOptions());
}
