import { useQuery, useQueryClient } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";
import { SignalMetadata } from "@/lib/types/Signal";

/**
 * Hook to fetch metadata for multiple signals matching a selector pattern.
 *
 * @param signalSelector - A selector regex pattern as a string to match signals (e.g., "^BMS_.+")
 * @returns React Query result with an array of signal metadata
 */
const useSignalMetadataList = (signalSelector: string) => {
  const queryClient = useQueryClient();

  return useQuery({
    queryKey: ["signal-metadata", signalSelector],
    queryFn: async () => {
      const response = await fetch(
        `${API_BASE_URL}/signal/metadata?name=${encodeURIComponent(signalSelector)}`
      );

      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      const data = (await response.json()) as Record<string, SignalMetadata>;

      Object.entries(data).forEach(([signalName, signal]) => {
        // NOTE(evan): Cache individual signal metadata to prevent flashing when switching between
        //             individual signals with the selector.
        queryClient.setQueryData(["signal-metadata", signalName], signal);
      });

      return Object.values(data);
    },
    retryOnMount: false,
    retry: (failureCount, error) => {
      if (error instanceof Error && error.message.includes("404")) {
        return false;
      }

      if (error instanceof Error && error.message.includes("No signals found")) {
        return false;
      }

      return failureCount < 2;
    },
  });
};

export default useSignalMetadataList;
