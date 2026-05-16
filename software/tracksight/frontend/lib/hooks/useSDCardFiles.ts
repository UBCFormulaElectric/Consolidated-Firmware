import { queryOptions, skipToken, useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";

const fetchSDCardFiles = async (drive: string) => {
    const response = await fetch(`${API_BASE_URL}/api/mock/sd/show?drive=${drive}`);

    if (!response.ok) {
        throw new Error(`Failed to fetch sd card files: ${response.statusText}`);
    }

    return (await response.json()) as string[];
};

export const sdCardFilesQueryOptions = (drive: string | null) =>
    queryOptions({
        queryKey: ["sd-card-files", drive],
        queryFn: drive === null ? skipToken : () => fetchSDCardFiles(drive),
    });

/**
 * Hook to list all the sd card files on one of the currently attached drives.
 */
export default function useSDCardFiles(drive: string | null) {
    return useQuery(sdCardFilesQueryOptions(drive));
}
