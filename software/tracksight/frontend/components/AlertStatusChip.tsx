"use client";

import { ALERT_SEVERITY_COLOR, AlertSeverity } from "@/lib/alerts";
import { cn } from "@/lib/utils";

export type AlertStatus = Record<AlertSeverity, number>;

const CHIP_STYLES: Record<AlertSeverity | "none", string> = {
    fault: "border-red-300 bg-red-50 text-red-800",
    warning: "border-amber-300 bg-amber-50 text-amber-900",
    info: "border-gray-300 bg-white text-gray-700",
    none: "border-gray-200 bg-white text-gray-500",
};

const pluralize = (count: number, noun: string) => `${count} ${noun}${count === 1 || noun === "info" ? "" : "s"}`;

/** Always-visible summary of active alerts; clicking it jumps to the alert rows. */
export function AlertStatusChip({ status, isLive, onClick }: { status: AlertStatus | null; isLive: boolean; onClick: () => void }) {
    const worst: AlertSeverity | "none" = !status ? "none" : status.fault > 0 ? "fault" : status.warning > 0 ? "warning" : status.info > 0 ? "info" : "none";
    const parts = status ? [status.fault > 0 && pluralize(status.fault, "fault"), status.warning > 0 && pluralize(status.warning, "warning"), status.info > 0 && pluralize(status.info, "info")].filter(Boolean) : [];

    return (
        <button type="button" onClick={onClick} title={isLive ? "Alerts active at the latest data. Click to view." : "Alerts active at the cursor, or the right edge of the view. Click to view."} className={cn("flex shrink-0 items-center gap-2 rounded-full border px-3 py-1 text-sm font-medium", CHIP_STYLES[worst])}>
            {worst === "none" ? <span className="size-2 rounded-full border border-gray-300" /> : <span className="size-2 rounded-full" style={{ backgroundColor: ALERT_SEVERITY_COLOR[worst] }} />}
            {parts.length > 0 ? parts.join(" · ") : "No active alerts"}
        </button>
    );
}
