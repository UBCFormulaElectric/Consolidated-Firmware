import { useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";

/**
 * Hook to list all the sd card files on one of the currently attached drives.
 *
 * @returns React Query result with the
 */
export default function useSDCardFiles(drive: string | null) {
    return useQuery({
        queryKey: ["sd-card-files", drive],
        enabled: drive !== null,
        queryFn: async () => {
            const response = await fetch(`${API_BASE_URL}/api/mock/sd/show?drive=${drive}`);

            if (!response.ok) {
                throw new Error(`Failed to fetch signals: ${response.statusText}`);
            }

            const json = (await response.json()) as string[];

            return json;
        },
    });
}
