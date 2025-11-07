import { useQuery } from '@tanstack/react-query';

import { API_BASE_URL } from '@/lib/constants';
import { SignalMetadata } from '@/lib/types/Signal';

const useSignalMetadata = (signalName: string) => {
  return useQuery({
    queryKey: ['signal-metadata', signalName],
    queryFn: async () => {
      const response = await fetch(`${API_BASE_URL}/signal?name=${encodeURIComponent(signalName)}`);

      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      const json = await response.json() as SignalMetadata[];

      const signal = json.find((s) => s.name === signalName);

      if (!signal) {
        throw new Error(`Signal not found: ${signalName}`);
      }

      return signal;
    },
    retryOnMount: false,
    retry: (failureCount, error) => {
      if (error instanceof Error && error.message.includes('Signal not found')) {
        return false;
      }

      return failureCount < 2;
    }
  })
};

export default useSignalMetadata;

