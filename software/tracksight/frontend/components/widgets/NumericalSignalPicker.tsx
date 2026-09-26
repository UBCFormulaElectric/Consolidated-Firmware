"use client";

import { isNumericalSignalMetadata, NumericalSignalMetadata } from "@/lib/types/Signal";
import SignalPicker from "./SignalPicker";

function getSearchableText(signal: NumericalSignalMetadata): string {
    return [signal.name, signal.msg_name, signal.tx_node, signal.unit].filter(Boolean).join(" ");
}

function getSignalSubtitle(signal: NumericalSignalMetadata): string {
    const parts = [signal.msg_name, signal.tx_node];

    if (signal.unit) {
        parts.push(signal.unit);
    }

    if (signal.cycle_time_ms !== null) {
        parts.push(`${signal.cycle_time_ms}ms`);
    }

    return parts.join(" • ");
}

function NumericalSignalItem({ data: signal }: { data: NumericalSignalMetadata }) {
    return (
        <>
            <div className="flex items-center justify-between gap-3">
                <span className="min-w-0 truncate font-medium text-gray-900">{signal.name}</span>
                {signal.unit && <span className="max-w-1/4 shrink-0 truncate text-xs uppercase tracking-wide text-gray-500">{signal.unit}</span>}
            </div>
            <p className="mt-1 truncate text-xs text-gray-500">{getSignalSubtitle(signal)}</p>
        </>
    );
}

export function NumericalSignalPicker(props: { selectedSignals: NumericalSignalMetadata[]; onSelectedSignalsChange: (signals: NumericalSignalMetadata[]) => void; onConfirm: (signals: NumericalSignalMetadata[]) => void }) {
    return <SignalPicker {...props} filter={isNumericalSignalMetadata} getSearchableText={getSearchableText} ItemRenderer={NumericalSignalItem} placeholder="Search by signal, message, unit, or node" />;
}
