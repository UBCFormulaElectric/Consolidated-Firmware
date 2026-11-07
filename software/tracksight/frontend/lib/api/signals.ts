import { IS_MOCK } from "@/lib/constants";
import { getMockSignalCatalog } from "@/lib/mock/catalog";
import { SignalMetadata, SignalType } from "@/lib/types/Signal";
import { ENDPOINTS } from "@/lib/api/endpoints";

export const getSignalType = (signal: Omit<SignalMetadata, "type">): SignalMetadata["type"] => {
    if ("enum_signal" in signal && signal["enum_signal"] !== null) {
        return SignalType.ENUM;
    } else if (signal.max_val === 1 && signal.min_val === 0) {
        return SignalType.BOOLEAN;
    } else {
        return SignalType.NUMERICAL;
    }
};

const fetchSignalMetadata = async (): Promise<SignalMetadata[]> => {
    // the frontend-only mock has no backend to ask
    if (IS_MOCK) return getMockSignalCatalog();

    try {
        const response = await fetch(ENDPOINTS.signalMetadata, {
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
