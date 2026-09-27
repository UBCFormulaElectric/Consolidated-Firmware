export type AlertSeverity = "fault" | "warning" | "info";

export const ALERT_SEVERITY_ORDER: Record<AlertSeverity, number> = { fault: 0, warning: 1, info: 2 };
export const ALERT_SEVERITY_COLOR: Record<AlertSeverity, string> = { fault: "#dc2626", warning: "#d97706", info: "#64748b" };

export type ParsedAlertName = {
    node: string | null;
    severity: AlertSeverity;
    shortName: string;
};

// jsoncan names every alert {node}_{Fault|Warning|Info}_{name} (scripts/code_generation/jsoncan/src/json_parsing/parse_alert.py)
const ALERT_NAME_PATTERN = /^([A-Za-z0-9]+)_(Fault|Warning|Info)_(.+)$/;

export function parseAlertName(name: string): ParsedAlertName {
    const match = ALERT_NAME_PATTERN.exec(name);
    if (!match) return { node: null, severity: "info", shortName: name };

    return { node: match[1], severity: match[2].toLowerCase() as AlertSeverity, shortName: match[3] };
}

export type AlertInterval = { start: number; end: number };

/** the alert row being hovered, shaded across every chart so its effect on the signals is visible */
export type AlertHighlight = {
    tracker: AlertIntervals;
    color: string;
    /** newest data time; an alert still active at its last sample is shaded up to here */
    latestTime: number;
};

/**
 * Active intervals of one alert series at one LOD. Live samples are appended to the same arrays,
 * so only new samples are scanned; a different array (LOD switch, historical merge) rebuilds from scratch.
 */
export class AlertIntervals {
    intervals: AlertInterval[] = [];
    /** the alert was still active at the latest sample */
    open = false;

    private timestamps: number[] | null = null;
    private processed = 0;

    update(timestamps: number[], values: number[]) {
        if (timestamps !== this.timestamps || timestamps.length < this.processed) {
            this.timestamps = timestamps;
            this.processed = 0;
            this.intervals = [];
            this.open = false;
        }

        for (let i = this.processed; i < timestamps.length; i++) {
            const timestamp = timestamps[i];
            if (values[i] >= 0.5) {
                if (this.open) this.intervals[this.intervals.length - 1].end = timestamp;
                else this.intervals.push({ start: timestamp, end: timestamp });
                this.open = true;
            } else if (this.open) {
                this.intervals[this.intervals.length - 1].end = timestamp;
                this.open = false;
            }
        }
        this.processed = timestamps.length;
    }

    /** index of the first interval that ends at or after `time` (intervals never overlap, so ends are sorted) */
    firstEndingAfter(time: number): number {
        let low = 0;
        let high = this.intervals.length;
        while (low < high) {
            const mid = (low + high) >> 1;
            if (this.intervals[mid].end < time) low = mid + 1;
            else high = mid;
        }
        // the open interval keeps going past its last sample
        return low === this.intervals.length && this.open ? low - 1 : low;
    }

    isActiveAt(time: number): boolean {
        const index = this.firstEndingAfter(time);
        const interval = this.intervals[index];
        return interval !== undefined && interval.start <= time;
    }
}
