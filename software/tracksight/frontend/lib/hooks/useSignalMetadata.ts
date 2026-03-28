import { useQuery } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";
import { SignalMetadata } from "@/lib/types/Signal";
import { getSignalMetadataType } from "../api/signals";

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

      const response = await fetch(`${API_BASE_URL}/api/v1/signal/metadata?name=${encodeURIComponent(signalName)}`);
      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      // TODO when the query/find signal endpoints are seperated, change this to recieve a single SignalMetadata object
      const json = (await response.json()) as { [signalName: string]: Omit<SignalMetadata, "type"> };
      const signal = Object.values(json).find((s) => s.name === signalName);

      if (!signal) {
        throw new Error(`Signal not found: ${signalName}`);
      }

      const type = getSignalMetadataType(signal);
      return { ...signal, type };
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
