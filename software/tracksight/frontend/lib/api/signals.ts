import { SignalMetadata, SignalType } from "@/lib/types/Signal";

export const getSignalType = (signal: Omit<SignalMetadata, "type">): SignalMetadata["type"] => {
  // Prefer the backend-reported type. Alert signals share booleans' 0/1 range, so
  // they can only be distinguished via this field, not by inference.
  const backendType = (signal as { signal_type?: string }).signal_type;
  switch (backendType) {
    case "alert":
      return SignalType.ALERT;
    case "enum":
      return SignalType.ENUM;
    case "boolean":
      return SignalType.BOOLEAN;
    case "numerical":
      return SignalType.NUMERICAL;
  }

  // Fallback inference for backends that don't report a signal type.
  if ("enum_signal" in signal && signal["enum_signal"] !== null) {
    return SignalType.ENUM;
  } else if (signal.max_val === 1 && signal.min_val === 0) {
    return SignalType.BOOLEAN;
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
    
    return Object.entries(data).map(([name, metadata]) => ({ ...metadata, name, type: getSignalType(metadata) })) as SignalMetadata[];
  } catch (error) {
    console.error("Error fetching signal metadata:", error);
    throw error;
  }
};

export { fetchSignalMetadata };
