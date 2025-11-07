import { useQuery } from '@tanstack/react-query';

import { API_BASE_URL } from '@/lib/constants';
import { SignalMetadata } from '@/lib/types/Signal';

const useSignalMetadataList = (signalSelector: string) => {
  return useQuery({
    queryKey: ['signal-metadata', signalSelector],
    queryFn: async () => {
      const response = await fetch(`${API_BASE_URL}/signal?name=${encodeURIComponent(signalSelector)}`);

      if (!response.ok) {
        throw new Error(`Failed to fetch signals: ${response.statusText}`);
      }

      const data = await response.json() as SignalMetadata[];

      if (data.length === 0) {
        throw new Error(`No signals found for selector: ${signalSelector}`);
      }

      return new Promise<SignalMetadata[]>((resolve) => resolve(data));
    },
    retryOnMount: false,
    retry: (failureCount, error) => {
      if (error instanceof Error && error.message.includes('404')) {
        return false;
      }

      if (error instanceof Error && error.message.includes('No signals found')) {
        return false;
      }

      return failureCount < 2;
    }
  })
};

export default useSignalMetadataList;

