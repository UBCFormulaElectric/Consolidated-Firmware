"use client";

import { cn } from "@/lib/utils";

/** Always-visible count of active alerts; clicking it jumps to the alert rows. */
export function AlertStatusChip({ activeCount, isLive, onClick }: { activeCount: number | null; isLive: boolean; onClick: () => void }) {
    const hasActive = (activeCount ?? 0) > 0;

    return (
        <button type="button" onClick={onClick} title={isLive ? "Alerts active at the latest data. Click to view." : "Alerts active at the cursor, or the right edge of the view. Click to view."} className={cn("flex shrink-0 items-center gap-2 rounded-full border px-3 py-1 text-sm", hasActive ? "border-gray-400 bg-white font-semibold text-gray-900" : "border-gray-200 bg-white font-medium text-gray-500")}>
            <span className={cn("size-2 rounded-full", hasActive ? "bg-gray-900" : "border border-gray-300")} />
            {hasActive ? `${activeCount} active ${activeCount === 1 ? "alert" : "alerts"}` : "No active alerts"}
        </button>
    );
}
