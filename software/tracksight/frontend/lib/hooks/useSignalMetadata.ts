import { useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";
import { SignalMetadata } from "@/lib/types/Signal";

/**
 * Hook to fetch metadata for a specific signal.
 *
 * @param signalName - The name of the signal to fetch metadata for
 * @returns React Query result with the signal metadata
 */
export default function useSignalMetadata(signalName: string | null) {
  return useQuery({
    queryKey: ["signal-metadata", signalName],
    queryFn: async () => {
      if (signalName === null) {
        return null;
      }

      const response = await fetch(`${API_BASE_URL}/signal?name=${encodeURIComponent(signalName)}`);
      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      // TODO when the query/find signal endpoints are seperated, change this to recieve a single SignalMetadata object
      const json = (await response.json()) as SignalMetadata[];
      const signal = json.find((s) => s.name === signalName);
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
}
