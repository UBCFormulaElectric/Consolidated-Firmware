"use client";

import { AlertStatus } from "@/components/AlertStatusChip";
import { ALERT_SEVERITY_COLOR, ALERT_SEVERITY_ORDER, AlertIntervals, AlertSeverity, parseAlertName, ParsedAlertName } from "@/lib/alerts";
import { useAlertStore } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useTimezone } from "@/lib/contexts/TimezoneContext";
import SignalStore from "@/lib/signals/SignalStore";
import { getVisibleTelemetryMarkers } from "@/lib/telemetryMarkers";
import { formatTimeSpan } from "@/lib/utils/formatTimeSpan";
import { useEffect, useRef, useState } from "react";
import { useSyncedGraph } from "../SyncedGraphContainer";
import { CHART_PADDING, getFormatters, render_hover_line, selectLOD } from "./render";

// One fixed row per alert: names stay still at the left and only the unlabelled bars move with time,
// so the section stays readable while data scrolls quickly.
const ROW_HEIGHT = 20;
const BAR_HEIGHT = 10;
const ROWS_PADDING_Y = 6;
const MAX_VISIBLE_ROWS = 12;

// Bars share the charts' time axis from CHART_PADDING.left, so names are drawn over the left of each row on a
// backdrop (in live view that is the oldest data in view) rather than in a gutter that would push every chart right.
const NAME_COLUMN_WIDTH = 200;
const NAME_FADE_WIDTH = 24;
const NAME_DOT_X = 29; // lines up with the 24px (px-6) inset of the section and chart headers
const NAME_X = 40;
const NAME_FONT = "12px sans-serif";
const COUNT_RIGHT_X = NAME_COLUMN_WIDTH - 10;
const NAME_MAX_WIDTH = COUNT_RIGHT_X - NAME_X - 22; // leaves room for the count
const NAME_BACKDROP_RGB = "249, 250, 251"; // the section's gray-50
const NAME_BACKDROP_ALPHA = 0.92;
const EDGE_MARKER_WIDTH = 6;
const EDGE_TOLERANCE_PX = 4;
const BAR_HIT_SLOP_PX = 3;
const OFFSCREEN_STATUS_INTERVAL_MS = 250;

const ROW_STRIPE_COLOR = "rgba(15, 23, 42, 0.03)";
const ROW_HOVER_COLOR = "rgba(37, 99, 235, 0.08)";
const INACTIVE_DOT_COLOR = "#cbd5e1";
const NODE_TEXT_COLOR = "#9ca3af";
const ACTIVE_TEXT_COLOR = "#111827";
const INACTIVE_TEXT_COLOR = "#6b7280";
const COUNT_FONT = "11px sans-serif";
const MARKER_COLOR = "rgba(220, 38, 38, 0.85)";

const heightForRows = (rows: number) => ROWS_PADDING_Y * 2 + rows * ROW_HEIGHT;

const fittedNameCache = new Map<string, string>();

/** truncates `text` with an ellipsis to fit `maxWidth` in the context's current font; cached since names rarely change */
function fitText(context: CanvasRenderingContext2D, text: string, maxWidth: number): string {
    const key = `${context.font}|${maxWidth}|${text}`;
    const cached = fittedNameCache.get(key);
    if (cached !== undefined) return cached;

    let fitted = text;
    if (context.measureText(text).width > maxWidth) {
        let end = text.length;
        while (end > 0 && context.measureText(`${text.slice(0, end)}…`).width > maxWidth) end--;
        fitted = `${text.slice(0, end)}…`;
    }
    fittedNameCache.set(key, fitted);
    return fitted;
}

type AlertRow = ParsedAlertName & {
    name: string;
    firstSeen: number;
    tracker: AlertIntervals;
};

type OffscreenActive = { count: number; worst: AlertSeverity } | null;

type MousePosition = { x: number; y: number; clientX: number; clientY: number } | null;

const compareRows = (left: AlertRow, right: AlertRow) => ALERT_SEVERITY_ORDER[left.severity] - ALERT_SEVERITY_ORDER[right.severity] || left.firstSeen - right.firstSeen || left.name.localeCompare(right.name);

function formatTimestamp(timestampMs: number, timeZone: string) {
    const date = new Date(timestampMs);
    return `${getFormatters(timeZone).time.format(date)}.${date.getUTCMilliseconds().toString().padStart(3, "0")}`;
}

function OffscreenHint({ direction, active }: { direction: "above" | "below"; active: OffscreenActive }) {
    if (!active) return null;

    return (
        <span className="text-xs font-medium" style={{ color: ALERT_SEVERITY_COLOR[active.worst] }}>
            {direction === "above" ? "▲" : "▼"} {active.count} active {direction}
        </span>
    );
}

function AlertTimeline() {
    const canvasRef = useRef<HTMLCanvasElement>(null);
    const wrapperRef = useRef<HTMLDivElement>(null);
    const sectionRef = useRef<HTMLElement>(null);
    const tooltipRef = useRef<HTMLDivElement>(null);
    const animationFrame = useRef<number | null>(null);
    const mousePos = useRef<MousePosition>(null);

    const [rowCount, setRowCount] = useState(0);
    const [activeAbove, setActiveAbove] = useState<OffscreenActive>(null);
    const [activeBelow, setActiveBelow] = useState<OffscreenActive>(null);

    const { globalTimeRangeRef, XToTime, timeToX, hoverXRef, isLive, reportAlertStatus, highlightedAlertRef } = useSyncedGraph();
    const { timezone } = useTimezone();

    // the render loop is started once, so it reads everything that can change through refs
    const XToTimeRef = useRef(XToTime);
    XToTimeRef.current = XToTime;
    const timeToXRef = useRef(timeToX);
    timeToXRef.current = timeToX;
    const isLiveRef = useRef(isLive);
    isLiveRef.current = isLive;
    const timezoneRef = useRef(timezone);
    timezoneRef.current = timezone;
    const reportAlertStatusRef = useRef(reportAlertStatus);
    reportAlertStatusRef.current = reportAlertStatus;

    const storeRef = useAlertStore();

    useEffect(() => {
        const canvas = canvasRef.current;
        if (!canvas) return;
        const ctx = canvas.getContext("2d");
        if (!ctx) return;

        const dpr = window.devicePixelRatio || 1;

        let trackedStore: SignalStore | null = null;
        const trackers = new Map<string, AlertIntervals>();
        let rows: AlertRow[] = [];
        let offscreenKey = "";
        let tooltipKey = "";
        let statusKey = "";

        const setTooltip = (lines: string[] | null, clientX = 0, clientY = 0) => {
            const tooltip = tooltipRef.current;
            const section = sectionRef.current;
            if (!tooltip || !section) return;

            const key = lines ? `${lines.join("\n")}@${clientX},${clientY}` : "";
            if (key === tooltipKey) return;
            tooltipKey = key;

            if (!lines) {
                tooltip.hidden = true;
                return;
            }

            tooltip.textContent = lines.join("\n");
            tooltip.hidden = false;
            const sectionRect = section.getBoundingClientRect();
            const left = Math.min(clientX - sectionRect.left + 12, sectionRect.width - tooltip.offsetWidth - 8);
            tooltip.style.transform = `translate(${Math.max(left, 8)}px, ${clientY - sectionRect.top + 14}px)`;
        };

        const syncRows = (store: SignalStore, leftEdge: number, rightEdge: number, plotWidth: number) => {
            if (store !== trackedStore) {
                trackedStore = store;
                trackers.clear();
                rows = [];
            }

            let rowsChanged = false;
            Object.entries(store.getAlertSeries()).forEach(([name, series]) => {
                if (series.lods.length === 0) return;

                let tracker = trackers.get(name);
                if (!tracker) {
                    tracker = new AlertIntervals();
                    trackers.set(name, tracker);
                }

                const lod = series.lods[selectLOD(series, leftEdge, rightEdge, plotWidth)];
                tracker.update(lod.timestamps, lod.data);

                // an alert gets a row the first time it fires and keeps it, so rows never shuffle under the reader
                if (tracker.intervals.length > 0 && !rows.some((row) => row.name === name)) {
                    rows.push({ name, ...parseAlertName(name), firstSeen: tracker.intervals[0].start, tracker });
                    rowsChanged = true;
                }
            });

            if (rowsChanged) {
                rows.sort(compareRows);
                setRowCount(rows.length);
            }
        };

        const syncOffscreenHints = (activeRows: boolean[]) => {
            const wrapper = wrapperRef.current;
            if (!wrapper) return;

            // derive the viewport from the row count rather than the DOM: a new row is drawn a frame before React grows the canvas
            const viewportHeight = Math.min(heightForRows(rows.length), heightForRows(MAX_VISIBLE_ROWS));
            const firstVisible = Math.ceil((wrapper.scrollTop - ROWS_PADDING_Y) / ROW_HEIGHT);
            const lastVisible = Math.floor((wrapper.scrollTop + viewportHeight - ROWS_PADDING_Y) / ROW_HEIGHT) - 1;

            const summarize = (from: number, to: number): OffscreenActive => {
                let count = 0;
                let worst: AlertSeverity = "info";
                for (let i = Math.max(from, 0); i <= Math.min(to, rows.length - 1); i++) {
                    if (!activeRows[i]) continue;
                    count++;
                    if (ALERT_SEVERITY_ORDER[rows[i].severity] < ALERT_SEVERITY_ORDER[worst]) worst = rows[i].severity;
                }
                return count > 0 ? { count, worst } : null;
            };

            const above = summarize(0, firstVisible - 1);
            const below = summarize(lastVisible + 1, rows.length - 1);
            const key = JSON.stringify([above, below]);
            if (key === offscreenKey) return;
            offscreenKey = key;
            setActiveAbove(above);
            setActiveBelow(below);
        };

        const syncStatus = (referenceTime: number, activeRows: boolean[]) => {
            const status: AlertStatus = { fault: 0, warning: 0, info: 0 };
            rows.forEach((row, rowIndex) => {
                if (!activeRows[rowIndex]) return;
                status[row.severity]++;
            });

            const key = `${status.fault}|${status.warning}|${status.info}`;
            if (key === statusKey) return;
            statusKey = key;
            reportAlertStatusRef.current(status);
        };

        // live dots answer "what is wrong right now"; historical dots follow the cursor, else the right edge of the view
        const getReferenceTime = (latest: number, rightEdge: number) => {
            const hoverX = hoverXRef.current;
            return isLiveRef.current ? latest : hoverX !== null ? XToTimeRef.current(hoverX) : rightEdge;
        };

        // while scrolled offscreen nothing is drawn, but the toolbar chip still needs current status
        const updateStatusOnly = () => {
            const store = storeRef.current;
            const range = globalTimeRangeRef.current;
            if (!store || !range) return;

            const width = canvas.getBoundingClientRect().width;
            const leftEdge = XToTimeRef.current(CHART_PADDING.left);
            const rightEdge = XToTimeRef.current(width);
            syncRows(store, leftEdge, rightEdge, width - CHART_PADDING.left);

            const referenceTime = getReferenceTime(range.max, rightEdge);
            syncStatus(
                referenceTime,
                rows.map((row) => row.tracker.isActiveAt(referenceTime))
            );
        };

        const renderRows = () => {
            animationFrame.current = requestAnimationFrame(renderRows);

            const store = storeRef.current;
            const range = globalTimeRangeRef.current;
            if (!store || !range) return;

            const toTime = XToTimeRef.current;
            const toX = timeToXRef.current;

            const rect = canvas.getBoundingClientRect();
            const width = rect.width;
            const height = rect.height;
            const targetWidth = Math.max(1, Math.floor(width * dpr));
            const targetHeight = Math.max(1, Math.floor(height * dpr));
            if (canvas.width !== targetWidth || canvas.height !== targetHeight) {
                canvas.width = targetWidth;
                canvas.height = targetHeight;
            }
            ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
            ctx.clearRect(0, 0, width, height);

            const plotLeft = CHART_PADDING.left;
            const leftEdge = toTime(plotLeft);
            const rightEdge = toTime(width);
            syncRows(store, leftEdge, rightEdge, width - plotLeft);

            const hoverX = hoverXRef.current;
            const referenceTime = getReferenceTime(range.max, rightEdge);
            const edgeTime = toTime(width + EDGE_TOLERANCE_PX);

            const mouse = mousePos.current;
            const hoveredRowIndex = mouse ? Math.floor((mouse.y - ROWS_PADDING_Y) / ROW_HEIGHT) : -1;
            let tooltipLines: string[] | null = null;

            const hoveredRow = rows[hoveredRowIndex];
            highlightedAlertRef.current = hoveredRow ? { tracker: hoveredRow.tracker, color: ALERT_SEVERITY_COLOR[hoveredRow.severity], latestTime: range.max } : null;

            const nameBackdrop = ctx.createLinearGradient(NAME_COLUMN_WIDTH, 0, NAME_COLUMN_WIDTH + NAME_FADE_WIDTH, 0);
            nameBackdrop.addColorStop(0, `rgba(${NAME_BACKDROP_RGB}, ${NAME_BACKDROP_ALPHA})`);
            nameBackdrop.addColorStop(1, `rgba(${NAME_BACKDROP_RGB}, 0)`);

            const activeRows = rows.map((row, rowIndex) => {
                const top = ROWS_PADDING_Y + rowIndex * ROW_HEIGHT;
                const centerY = top + ROW_HEIGHT / 2;
                const color = ALERT_SEVERITY_COLOR[row.severity];
                const { intervals, open } = row.tracker;
                const isActive = row.tracker.isActiveAt(referenceTime);

                // bars first so the name backdrop can sit over them
                ctx.save();
                ctx.beginPath();
                ctx.rect(plotLeft, top, width - plotLeft, ROW_HEIGHT);
                ctx.clip();
                ctx.fillStyle = color;

                let countInView = 0;
                let continuesPastRightEdge = false;
                let hoveredInterval: { start: number; end: number; ongoing: boolean } | null = null;
                for (let i = row.tracker.firstEndingAfter(leftEdge); i < intervals.length; i++) {
                    const interval = intervals[i];
                    if (interval.start > rightEdge) break;

                    // an alert still active at its latest sample stays drawn up to the newest data
                    const ongoing = open && i === intervals.length - 1;
                    const end = ongoing ? Math.max(interval.end, range.max) : interval.end;
                    const startX = toX(interval.start);
                    const endX = Math.max(toX(end), startX + 2);

                    countInView++;
                    if (end > edgeTime) continuesPastRightEdge = true;
                    if (rowIndex === hoveredRowIndex && mouse && mouse.x >= NAME_COLUMN_WIDTH && mouse.x >= startX - BAR_HIT_SLOP_PX && mouse.x <= endX + BAR_HIT_SLOP_PX) {
                        hoveredInterval = { start: interval.start, end, ongoing };
                    }

                    ctx.beginPath();
                    ctx.roundRect(startX, centerY - BAR_HEIGHT / 2, endX - startX, BAR_HEIGHT, 3);
                    ctx.fill();
                }
                ctx.restore();

                ctx.fillStyle = `rgba(${NAME_BACKDROP_RGB}, ${NAME_BACKDROP_ALPHA})`;
                ctx.fillRect(0, top, NAME_COLUMN_WIDTH, ROW_HEIGHT);
                ctx.fillStyle = nameBackdrop;
                ctx.fillRect(NAME_COLUMN_WIDTH, top, NAME_FADE_WIDTH, ROW_HEIGHT);

                if (rowIndex === hoveredRowIndex) {
                    ctx.fillStyle = ROW_HOVER_COLOR;
                    ctx.fillRect(0, top, width, ROW_HEIGHT);
                } else if (rowIndex % 2 === 1) {
                    ctx.fillStyle = ROW_STRIPE_COLOR;
                    ctx.fillRect(0, top, width, ROW_HEIGHT);
                }

                ctx.beginPath();
                ctx.arc(NAME_DOT_X, centerY, 4, 0, Math.PI * 2);
                if (isActive) {
                    ctx.fillStyle = color;
                    ctx.fill();
                } else {
                    ctx.strokeStyle = INACTIVE_DOT_COLOR;
                    ctx.lineWidth = 1.5;
                    ctx.stroke();
                }

                ctx.font = NAME_FONT;
                ctx.textAlign = "left";
                ctx.textBaseline = "middle";
                let nameX = NAME_X;
                if (row.node) {
                    const nodeText = `${row.node} `;
                    ctx.fillStyle = NODE_TEXT_COLOR;
                    ctx.fillText(nodeText, nameX, centerY);
                    nameX += ctx.measureText(nodeText).width;
                }
                ctx.fillStyle = isActive ? ACTIVE_TEXT_COLOR : INACTIVE_TEXT_COLOR;
                ctx.fillText(fitText(ctx, row.shortName, NAME_MAX_WIDTH - (nameX - NAME_X)), nameX, centerY);

                if (countInView > 0) {
                    ctx.font = COUNT_FONT;
                    ctx.textAlign = "right";
                    ctx.fillStyle = NODE_TEXT_COLOR;
                    ctx.fillText(`${countInView}`, COUNT_RIGHT_X, centerY);
                }

                if (continuesPastRightEdge) {
                    // the bar runs on into time that is out of view
                    ctx.beginPath();
                    ctx.moveTo(width - EDGE_MARKER_WIDTH - 2, centerY - 5);
                    ctx.lineTo(width - 2, centerY);
                    ctx.lineTo(width - EDGE_MARKER_WIDTH - 2, centerY + 5);
                    ctx.closePath();
                    ctx.strokeStyle = "#ffffff";
                    ctx.lineWidth = 2;
                    ctx.stroke();
                    ctx.fillStyle = color;
                    ctx.fill();
                }

                if (rowIndex === hoveredRowIndex && mouse) {
                    const timeZone = timezoneRef.current;
                    if (mouse.x < NAME_COLUMN_WIDTH) {
                        tooltipLines = [row.name, `${isActive ? "Active" : "Inactive"} · ${countInView} ${countInView === 1 ? "occurrence" : "occurrences"} in view`];
                    } else if (hoveredInterval) {
                        const { start, end, ongoing } = hoveredInterval;
                        tooltipLines = [row.name, ongoing ? `Since ${formatTimestamp(start, timeZone)} · active for ${formatTimeSpan(end - start)}` : `${formatTimestamp(start, timeZone)} → ${formatTimestamp(end, timeZone)} · ${formatTimeSpan(end - start)}`];
                    }
                }

                return isActive;
            });

            ctx.save();
            ctx.beginPath();
            // vertical lines stay off the names so they remain readable
            ctx.rect(NAME_COLUMN_WIDTH, 0, width - NAME_COLUMN_WIDTH, height);
            ctx.clip();
            ctx.strokeStyle = MARKER_COLOR;
            ctx.lineWidth = 1.5;
            getVisibleTelemetryMarkers(leftEdge, rightEdge).forEach((marker) => {
                const x = toX(marker.timestampMs);
                ctx.beginPath();
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
                ctx.stroke();
            });

            if (hoverX !== null) {
                render_hover_line(ctx, width, height, toTime(hoverX), toX, false);
            }
            ctx.restore();

            setTooltip(tooltipLines, mouse?.clientX, mouse?.clientY);
            syncOffscreenHints(activeRows);
            syncStatus(referenceTime, activeRows);
        };

        let statusInterval: number | null = null;
        const observer = new IntersectionObserver((entries) => {
            const entry = entries[entries.length - 1];
            if (entry.isIntersecting) {
                if (statusInterval !== null) window.clearInterval(statusInterval);
                statusInterval = null;
                if (animationFrame.current === null) animationFrame.current = requestAnimationFrame(renderRows);
                return;
            }

            if (animationFrame.current !== null) cancelAnimationFrame(animationFrame.current);
            animationFrame.current = null;
            statusInterval ??= window.setInterval(updateStatusOnly, OFFSCREEN_STATUS_INTERVAL_MS);
        });
        observer.observe(canvas);

        return () => {
            reportAlertStatusRef.current(null);
            highlightedAlertRef.current = null;
            observer.disconnect();
            if (statusInterval !== null) window.clearInterval(statusInterval);
            if (animationFrame.current !== null) cancelAnimationFrame(animationFrame.current);
            animationFrame.current = null;
        };
    }, []);

    const handleMouseMove = (event: React.MouseEvent<HTMLCanvasElement>) => {
        const rect = event.currentTarget.getBoundingClientRect();
        const x = event.clientX - rect.left;
        mousePos.current = { x, y: event.clientY - rect.top, clientX: event.clientX, clientY: event.clientY };
        hoverXRef.current = x >= NAME_COLUMN_WIDTH ? x : null; // hovering names shouldn't move the charts' hover line
    };

    const handleMouseLeave = () => {
        mousePos.current = null;
        hoverXRef.current = null;
    };

    return (
        <section ref={sectionRef} aria-labelledby="alerts-heading" className="relative z-50 flex flex-col gap-1 border-b-2 border-gray-300 bg-gray-50">
            <div className="flex items-baseline justify-between gap-4 px-6 pt-3">
                <div className="flex items-baseline gap-2">
                    <h2 id="alerts-heading" className="text-xs font-semibold tracking-wide text-gray-600 uppercase">
                        Alerts
                    </h2>
                    {rowCount > 0 && <span className="text-xs text-gray-500">{rowCount}</span>}
                </div>
                <div className="flex items-baseline gap-3">
                    <OffscreenHint direction="above" active={activeAbove} />
                    <OffscreenHint direction="below" active={activeBelow} />
                </div>
            </div>
            <div ref={wrapperRef} className="relative overflow-y-auto" tabIndex={0} aria-label="Alert rows" style={{ maxHeight: heightForRows(MAX_VISIBLE_ROWS) }}>
                {rowCount === 0 && (
                    <p className="pointer-events-none absolute inset-y-0 flex items-center text-xs text-gray-500" style={{ left: NAME_X }}>
                        {isLive ? "No alerts have fired yet." : "No alerts in the loaded data."}
                    </p>
                )}
                <canvas className="block w-full" ref={canvasRef} style={{ height: heightForRows(Math.max(rowCount, 1)) }} onMouseMove={handleMouseMove} onMouseLeave={handleMouseLeave} />
            </div>
            <div ref={tooltipRef} hidden className="pointer-events-none absolute top-0 left-0 max-w-md rounded bg-gray-900/90 px-2 py-1 text-xs whitespace-pre text-white shadow" />
        </section>
    );
}

export default AlertTimeline;
