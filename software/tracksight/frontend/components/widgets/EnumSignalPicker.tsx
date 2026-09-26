"use client";

import { BooleanSignalMetadata, EnumSignalMetadata, isBooleanSignalMetadata, isEnumSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import SignalPicker from "./SignalPicker";

type EnumOrBooleanSignalMetadata = EnumSignalMetadata | BooleanSignalMetadata;

function isEnumOrBooleanSignalMetadata(signal: SignalMetadata): signal is EnumOrBooleanSignalMetadata {
    return isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal);
}

function getSearchableText(signal: EnumOrBooleanSignalMetadata): string {
    const enumName = isEnumSignalMetadata(signal) ? signal.enum_signal.enum_name : "";

    return [signal.name, signal.msg_name, signal.tx_node, enumName].filter(Boolean).join(" ");
}

function getSignalSubtitle(signal: EnumOrBooleanSignalMetadata): string {
    const parts = [signal.msg_name, signal.tx_node];

    if (isEnumSignalMetadata(signal)) {
        parts.push(signal.enum_signal.enum_name);
    }

    if (signal.cycle_time_ms !== null) {
        parts.push(`${signal.cycle_time_ms}ms`);
    }

    return parts.join(" • ");
}

function EnumSignalItem({ data: signal }: { data: EnumOrBooleanSignalMetadata }) {
    return (
        <>
            <div className="flex w-full items-center justify-between gap-3">
                <span className="min-w-0 truncate font-medium text-gray-900">{signal.name}</span>
                <span className="max-w-1/4 shrink-0 truncate text-xs uppercase tracking-wide text-gray-500">{isEnumSignalMetadata(signal) ? signal.enum_signal.enum_name : "BOOLEAN"}</span>
            </div>
            <p className="mt-1 truncate text-xs text-gray-500">{getSignalSubtitle(signal)}</p>
        </>
    );
}

export function EnumSignalPicker(props: { selectedSignals: EnumOrBooleanSignalMetadata[]; onSelectedSignalsChange: (signals: EnumOrBooleanSignalMetadata[]) => void; onConfirm: (signals: EnumOrBooleanSignalMetadata[]) => void }) {
    return <SignalPicker {...props} filter={isEnumOrBooleanSignalMetadata} getSearchableText={getSearchableText} ItemRenderer={EnumSignalItem} placeholder="Search by signal, message, enum name, or node" />;
}
