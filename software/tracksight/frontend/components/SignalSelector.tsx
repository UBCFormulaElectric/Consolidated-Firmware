"use client";

import { useSignalsMetadataList } from "@/lib/contexts/SignalsContext";
import { SignalMetadata } from "@/lib/types/Signal"
import Selector from "./common/Selector";

type SignalSelectorProps = {
    filter: (signal: SignalMetadata) => boolean;

    selectedSignal: SignalMetadata | null;
    onSelect: (signal: SignalMetadata) => void;
}

type SignalItemRendererProps = {
    data: SignalMetadata;
}

const SignalItemRenderer: React.FC<SignalItemRendererProps> = ({ data }) => {
    return (
        <div className="flex gap-2">
            <p>
                {data.name}
            </p>
            {
                data.unit && <p className="text-gray-500">
                    ({data.unit})
                </p>
            }
        </div>
    );
}

const SignalSelector: React.FC<SignalSelectorProps> = (props) => {
    const {
        filter,
        selectedSignal,
        onSelect
    } = props;

    const { signalMetadata } = useSignalsMetadataList();


    const filteredSignals = Array.from(signalMetadata.values().filter(filter));

    const signalCategories = filteredSignals.reduce((categories, signal) => {
        const category = signal.tx_node || "Uncategorized";
        if (!categories[category]) {
            categories[category] = [];
        }

        categories[category].push(signal);
        return categories;
    }, {} as Record<string, SignalMetadata[]>);

    const options = Object.entries(signalCategories).map(([category, signals]) => ({
        label: category,
        next: signals as any
    }));

    return (
        <Selector
            options={options}
            selectedOption={selectedSignal}
            onSelect={onSelect}
            ItemRenderer={SignalItemRenderer}
        />
    );
}

export default SignalSelector;