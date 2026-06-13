"use client";

import { ChevronDown, Loader2 } from "lucide-react";
import Link from "next/link";
import { usePathname } from "next/navigation";

import { ErrorRateIndicator } from "@/components/ErrorRateIndicator";
import { TimezoneSelector } from "@/components/common/TimezoneSelector";
import { useHistoricalSelection } from "@/lib/contexts/HistoricalSelectionContext";

function HistoricalNavButton({ label, value, onClick }: { label: string; value: string; onClick: () => void }) {
    return (
        <button type="button" onClick={onClick} className="flex items-center gap-2 rounded border border-black bg-white py-1.5 pl-3 pr-2 text-left transition-colors hover:cursor-pointer hover:border-black/75">
            <span className="text-[0.6rem] font-semibold uppercase tracking-wide text-gray-500">{label}</span>
            <span className="max-w-44 truncate text-sm font-semibold text-gray-900">{value}</span>
            <ChevronDown className="size-4 shrink-0 text-gray-500" strokeWidth={2.25} />
        </button>
    );
}

function HistoricalNavControls() {
    const { sourceLabel, selectedSession, openModal, isSyncing } = useHistoricalSelection();

    return (
        <div className="flex items-center gap-2">
            {isSyncing ? <Loader2 className="size-4 animate-spin text-blue-500" strokeWidth={2.4} /> : null}
            <HistoricalNavButton label="Source" value={sourceLabel} onClick={() => openModal(1)} />
            <HistoricalNavButton label="Session" value={selectedSession?.label ?? "Select"} onClick={() => openModal(3)} />
        </div>
    );
}

function Navbar() {
    const pathname = usePathname();
    const isHistorical = pathname === "/historical";

    return (
        <nav className="fixed top-0 left-0 z-50 h-min w-screen bg-white border-b border-b-gray-200">
            <div className="flex flex-row items-center justify-between px-8 py-4 select-none">
                <div className="flex flex-row items-center gap-6">
                    <Link href="/">Home</Link>
                    <Link href="/live">Live Data</Link>
                    <Link href="/historical">Historical Data</Link>
                    <Link href="/sd/dump">SD Card Dump</Link>
                </div>
                <div className="flex items-center gap-4">
                    {isHistorical ? <HistoricalNavControls /> : null}
                    <TimezoneSelector />
                    <ErrorRateIndicator />
                </div>
            </div>
        </nav>
    );
}

export default Navbar;
