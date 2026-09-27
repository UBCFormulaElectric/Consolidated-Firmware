/**
 * Knobs for the frontend-only mock (NEXT_PUBLIC_USE_MOCK_DATA=true), used to stress the UI without a backend.
 * Each can be set as a URL param (e.g. /live?mockSignals=200&mockHz=500) or an env var; URL params win so
 * volumes can change without restarting the dev server.
 *
 *   mockSignals  / NEXT_PUBLIC_MOCK_SIGNAL_COUNT  signals offered in "Add chart"            (default 60)
 *   mockAlerts   / NEXT_PUBLIC_MOCK_ALERT_COUNT   alerts that can fire                      (default 12)
 *   mockHz       / NEXT_PUBLIC_MOCK_SAMPLE_HZ     samples per second per charted signal     (default 100)
 *   mockCharts                                    replaces the live page's charts with this many random ones, once
 */

const ENV_DEFAULTS = {
    mockSignals: process.env.NEXT_PUBLIC_MOCK_SIGNAL_COUNT,
    mockAlerts: process.env.NEXT_PUBLIC_MOCK_ALERT_COUNT,
    mockHz: process.env.NEXT_PUBLIC_MOCK_SAMPLE_HZ,
};

const FALLBACKS = { mockSignals: 60, mockAlerts: 12, mockHz: 100 };

function readCount(key: keyof typeof FALLBACKS): number {
    const fromUrl = typeof window !== "undefined" ? new URLSearchParams(window.location.search).get(key) : null;
    for (const raw of [fromUrl, ENV_DEFAULTS[key]]) {
        const value = Number(raw);
        if (raw != null && raw !== "" && Number.isFinite(value) && value >= 0) return Math.floor(value);
    }
    return FALLBACKS[key];
}

export function getMockConfig() {
    return {
        signalCount: readCount("mockSignals"),
        alertCount: readCount("mockAlerts"),
        sampleHz: Math.max(1, readCount("mockHz")),
    };
}
