export type ParsedAlertName = {
    node: string | null;
    /** the rest of the name after the node prefix */
    label: string;
};

// jsoncan names every alert {node}_{Fault|Warning|Info}_{name} (scripts/code_generation/jsoncan/src/json_parsing/parse_alert.py).
// Only the node is used for now: how alerts should be categorised is still being settled with the other subteams.
const ALERT_NODE_PATTERN = /^([A-Za-z0-9]+)_(.+)$/;

export function parseAlertName(name: string): ParsedAlertName {
    const match = ALERT_NODE_PATTERN.exec(name);
    return match ? { node: match[1], label: match[2] } : { node: null, label: name };
}

// no red, amber or green, so a node's colour never reads as a severity
const NODE_COLORS: Record<string, string> = { BMS: "#2563eb", CRIT: "#475569", DAM: "#0891b2", FSM: "#0d9488", RSM: "#db2777", VC: "#7c3aed" };
const FALLBACK_NODE_COLORS = ["#c026d3", "#92400e", "#1e3a8a", "#6b21a8"];

export function alertNodeColor(node: string | null): string {
    if (!node) return FALLBACK_NODE_COLORS[0];
    if (NODE_COLORS[node]) return NODE_COLORS[node];

    let hash = 0;
    for (let i = 0; i < node.length; i++) hash = (hash * 31 + node.charCodeAt(i)) | 0;
    return FALLBACK_NODE_COLORS[Math.abs(hash) % FALLBACK_NODE_COLORS.length];
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
