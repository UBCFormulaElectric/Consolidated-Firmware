import { SignalMetadata } from "@/lib/types/Signal";

const fetchSignalMetadata = async (apiBaseUrl: string): Promise<SignalMetadata[]> => {
  try {
    const response = await fetch(`${apiBaseUrl}/signal`, {
      cache: "force-cache",
    });

    if (!response.ok) {
      throw new Error(`Failed to fetch signals: ${response.statusText}`);
    }

    const data: SignalMetadata[] = await response.json();
    return data;
  } catch (error) {
    console.error("Error fetching signal metadata:", error);
    throw error;
  }
};

export { fetchSignalMetadata };
