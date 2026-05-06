import { useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";

/**
 * Hook to list all currently mounted sd cards.
 *
 * @returns React Query result with the list of sd card paths.
 */
export default function useListSDCards() {
    return useQuery({
        queryKey: ["sd-cards"],
        queryFn: async () => {
            const response = await fetch(`${API_BASE_URL}/api/mock/sd/list`);

            if (!response.ok) {
                throw new Error(`Failed to fetch signals: ${response.statusText}`);
            }

            const json = (await response.json()) as string[];

            return json;
        },
    });
}
