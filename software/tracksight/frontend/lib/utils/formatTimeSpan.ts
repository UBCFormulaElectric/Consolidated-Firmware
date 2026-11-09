/** compact human duration, e.g. "180 ms", "4.2 s", "12 min" */
export function formatTimeSpan(ms: number) {
    if (ms < 1000) return `${ms < 10 ? ms.toFixed(1) : Math.round(ms)} ms`;
    if (ms < 60_000) return `${(ms / 1000).toFixed(ms < 10_000 ? 1 : 0)} s`;
    if (ms < 3_600_000) return `${(ms / 60_000).toFixed(ms < 600_000 ? 1 : 0)} min`;
    return `${(ms / 3_600_000).toFixed(1)} h`;
}
