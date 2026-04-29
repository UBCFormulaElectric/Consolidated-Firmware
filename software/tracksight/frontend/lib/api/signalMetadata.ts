import { API_BASE_URL } from "@/lib/constants";
import { isNumericalSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

export async function fetchNumericalSignalMetadata(signalName: string): Promise<NumericalSignalMetadata> {
    const response = await fetch(`${API_BASE_URL}/signal?name=${encodeURIComponent(signalName)}`);

    if (!response.ok) {
        throw new Error(`Failed to fetch signal metadata: ${response.statusText}`);
    }

    const payload = (await response.json()) as SignalMetadata[];
    const signal = payload.find((candidate) => candidate.name === signalName);

    if (!signal) {
        throw new Error("Signal not found");
    }

    if (!isNumericalSignalMetadata(signal)) {
        throw new Error("Signal is not numerical");
    }

    return signal;
}
