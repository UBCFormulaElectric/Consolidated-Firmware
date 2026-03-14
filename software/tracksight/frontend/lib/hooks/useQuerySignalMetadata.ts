import { useQuery, useQueryClient } from "@tanstack/react-query";

import { API_BASE_URL } from "@/lib/constants";
import { EnumSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

/**
 * Hook to fetch metadata for multiple signals matching a selector pattern.
 *
 * @param query - A selector pattern to match signals (e.g., "motor_*")
 * @returns React Query result with an array of signal metadata
 */
export default function useQuerySignalMetadata(query: string) {
  const queryClient = useQueryClient();

  return useQuery({
    queryKey: ["signal-query", query],
    queryFn: async () => {
      const response = await fetch(
        `${API_BASE_URL}/signal?name=${encodeURIComponent(query)}` // TODO make this a different endpoint
      );

      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      const data = (await response.json()) as SignalMetadata[];

      if (data.length === 0) {
        throw new Error(`No signals found for selector: ${query}`);
      }

      data.forEach((signal) => {
        // NOTE(evan): Cache individual signal metadata to prevent flashing when switching between
        //             individual signals with the selector.
        queryClient.setQueryData(["signal-metadata", signal.name], signal);
      });

      return data;
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
}

export function useQueryEnumSignalMetadata() {
  return useQuery({
    queryKey: ["enum-signals"],
    queryFn: async () => {
      const response = await fetch(
        `${API_BASE_URL}/signal?type=enum` // TODO
      );
      if (!response.ok) {
        throw new Error(`Failed to fetch enum signals: ${response.statusText}`);
      }
      return (await response.json()) as EnumSignalMetadata[];
    },
    retryOnMount: false,
    retry: (failureCount, error) => {
      if (error instanceof Error && error.message.includes("404")) {
        return false;
      }
      return failureCount < 2;
    },
  })
}