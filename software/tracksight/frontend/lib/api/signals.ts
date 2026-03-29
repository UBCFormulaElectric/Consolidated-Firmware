import { SignalMetadata, SignalType } from "@/lib/types/Signal";

export const getSignalMetadataType = (signal: Omit<SignalMetadata, "type">): SignalMetadata["type"] => {
  if ("enum" in signal) {
    return SignalType.ENUM;
  } else if (signal.max_val === 1 && signal.min_val === 0) {
    return SignalType.ALERT;
  } else {
    return SignalType.NUMERICAL;
  }
};

const fetchSignalMetadata = async (apiBaseUrl: string): Promise<SignalMetadata[]> => {
  try {
    const response = await fetch(`${apiBaseUrl}/api/v1/signal/metadata`, {
      cache: "force-cache",
    });

    if (!response.ok) {
      throw new Error(`Failed to fetch signals: ${response.statusText}`);
    }

    const data: { [signalName: string]: Omit<SignalMetadata, "type"> } = await response.json();
    
    return Object.entries(data).map(([name, metadata]) => ({ ...metadata, name, type: getSignalMetadataType(metadata) })) as SignalMetadata[];
  } catch (error) {
    console.error("Error fetching signal metadata:", error);
    throw error;
  }
};

export { fetchSignalMetadata };
