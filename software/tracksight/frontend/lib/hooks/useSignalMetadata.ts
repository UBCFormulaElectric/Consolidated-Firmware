import { useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";
import { SignalMetadata } from "@/lib/types/Signal";

/**
 * Hook to fetch metadata for a specific signal.
 *
 * @param signalName - The name of the signal to fetch metadata for
 * @returns React Query result with the signal metadata
 */
const useSignalMetadata = (signalName: string) => {
  return useQuery({
    queryKey: ["signal-metadata", signalName],
    queryFn: async () => {
      const response = await fetch(
        `${API_BASE_URL}signal/metadata?name=${encodeURIComponent(signalName)}`
      );

      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      const json = (await response.json()) as Record<string, SignalMetadata>;

      const signal = json[signalName];

      if (!signal) {
        throw new Error(`Signal not found: ${signalName}`);
      }

      return signal;
    },
    retryOnMount: false,
    retry: (failureCount, error) => {
      if (error instanceof Error && error.message.includes("Signal not found")) {
        return false;
      }

      return failureCount < 2;
    },
  });
};

export default useSignalMetadata;
