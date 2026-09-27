/**
 * URL params for the frontend-only mock (NEXT_PUBLIC_USE_MOCK_DATA=true), e.g. /live?mockCharts=40&mockAlerts=30
 *
 *   mockAlerts  alerts that can fire (default 12)
 *   mockCharts  replaces the live page's charts with this many random ones, once
 */
export function readMockParam(name: "mockAlerts" | "mockCharts"): number | null {
    if (typeof window === "undefined") return null;

    const raw = new URLSearchParams(window.location.search).get(name);
    const value = Number(raw);
    return raw !== null && raw !== "" && Number.isInteger(value) && value >= 0 ? value : null;
}
