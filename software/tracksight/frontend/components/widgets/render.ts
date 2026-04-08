// react
import { RefObject } from "react";
// constants
import { ENUM_COLORS } from "@/lib/constants";
// types
import { ChartLayout, LODAwareNumericalSeries, LODAwareSeries } from "./CanvasChartTypes";
// utils
import { bisect } from "@/lib/bisect";
import { EnumTimelineWidgetData, NumericalGraphWidgetData, WidgetData } from "@/lib/types/Widget";

// TODO reduce to bisect right
// first enum index where the enum's end time (timestamps[i+1]) >= targetTime
function binarySearchForFirstEnumIndex(timestamps: number[], targetTime: number): number {
    if (timestamps.length === 0) return 0;

    let left = 0;
    let right = timestamps.length - 1;
    let result = timestamps.length - 1;

    while (left <= right) {
        const mid = Math.floor((left + right) / 2);

        let overlaps = false;

        if (mid < timestamps.length - 1) {
            overlaps = timestamps[mid + 1] >= targetTime;
        } else {
            overlaps = true;
        }

        if (overlaps) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return result;
}

// constants
const TIME_FORMATTER = new Intl.DateTimeFormat(undefined, {
    hour: "numeric",
    minute: "2-digit",
    second: "2-digit",
    hour12: true,
});

const DATE_FORMATTER = new Intl.DateTimeFormat(undefined, {
    year: "numeric",
    month: "short",
    day: "2-digit",
});
const CHART_PADDING = { top: 15, right: 0, bottom: 40, left: 60 };

function selectLOD(series: LODAwareSeries, visibleTimeRange: number, chartWidth: number): number {
    const totalLODCount = series.lods.length;
    const timePerPixel = visibleTimeRange / chartWidth;

    for (let i = 0; i < totalLODCount; i++) {
        if (series.lods[i].sampleIntervalMs <= timePerPixel) continue;

        return Math.max(0, i - 1);
    }

    return totalLODCount - 1;
}

function niceNumber(range: number, round: boolean) {
    const exponent = Math.floor(Math.log10(range));
    const fraction = range / Math.pow(10, exponent);
    let niceFraction;

    if (round) {
        if (fraction < 1.5) niceFraction = 1;
        else if (fraction < 3) niceFraction = 2;
        else if (fraction < 7) niceFraction = 5;
        else niceFraction = 10;
    } else {
        if (fraction <= 1) niceFraction = 1;
        else if (fraction <= 2) niceFraction = 2;
        else if (fraction <= 5) niceFraction = 5;
        else niceFraction = 10;
    }
    return niceFraction * Math.pow(10, exponent);
}

function render_enum(
    context: CanvasRenderingContext2D,
    width: number, // container context
    widgetConfig: EnumTimelineWidgetData,
    // rendering
    visibleStartTime: number,
    visibleEndTime: number,
    latestDateTime: number,
    timeToX: (time: number) => number, // time range
    LEGEND_HEIGHT: number,
    ENUM_STRIP_HEIGHT: number,
    ENUM_STRIP_GAP: number, // layout
    hoverTime: number | null,
    hoveredSignal: RefObject<string | null> | undefined
): Array<{ name: string; value: string }> | null {
    const signalConfigByName = new Map(widgetConfig.signals.map((signal) => [signal.name, signal]));

    // DRAWING LEGEND
    // context.textAlign = "left";
    // context.textBaseline = "top";
    // context.font = "10px sans-serif";
    // let legendX = CHART_PADDING.left;
    // const legendY = CHART_PADDING.top;
    // const allEnumValues = new Set<string>();

    let currentStripY = CHART_PADDING.top + LEGEND_HEIGHT;
    for (let series_idx = 0; series_idx < widgetConfig.signals.length; series_idx++) {
        const series = widgetConfig.data[series_idx];
        const selectedLOD = series.lods[selectLOD(series, visibleEndTime - visibleStartTime, width)];
        const { data: seriesData, timestamps: seriesTimestamps } = selectedLOD;
        const signalConfig = signalConfigByName.get(series.label);
        const isHovered = hoveredSignal?.current === series.label;

        context.fillStyle = "#000000";
        context.textAlign = "right";
        context.textBaseline = "middle";
        context.fillText(series.label, CHART_PADDING.left - 10, currentStripY + ENUM_STRIP_HEIGHT / 2);

        for (let data_idx = binarySearchForFirstEnumIndex(seriesTimestamps, visibleStartTime); data_idx <= binarySearchForFirstEnumIndex(seriesTimestamps, visibleEndTime); data_idx++) {
            const dataStartTime = seriesTimestamps[data_idx];
            const dataEndTime = Math.min(
                data_idx + 1 < seriesTimestamps.length ? seriesTimestamps[data_idx + 1] : latestDateTime, // if it's the last point, only render up to the latest datetime
                visibleEndTime
            );
            const value = seriesData[data_idx];

            const startX = Math.max(CHART_PADDING.left, timeToX(dataStartTime));
            const endX = Math.min(width - CHART_PADDING.right, timeToX(dataEndTime));
            const barWidth = endX - startX;
            if (barWidth < 0.5) continue;

            // TODO work out when to use NA_COLOR
            // const color = value === null
            //     ? NA_COLOR
            //     : ENUM_COLORS[value % ENUM_COLORS.length];
            const palette = widgetConfig.options.colorPalette[widgetConfig.signals[series_idx]?.name ?? ""] ?? {};
            const paletteColor = palette.enumValueColors[value];
            if (paletteColor) {
                context.fillStyle = paletteColor.hex();
            } else {
                // fallback just in case
                context.fillStyle = ENUM_COLORS[value % ENUM_COLORS.length];
            }

            context.fillRect(startX, currentStripY, barWidth, ENUM_STRIP_HEIGHT);
        }

        if (isHovered) {
            context.strokeStyle = "#000000";
            context.lineWidth = 3;
            context.strokeRect(CHART_PADDING.left, currentStripY, width - CHART_PADDING.left - CHART_PADDING.right, ENUM_STRIP_HEIGHT);
        }

        // move to the next strip
        currentStripY += ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP;
    }

    // move to the next strip
    currentStripY += ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP;
    return [];
}

function render_numerical(context: CanvasRenderingContext2D, width: number, chartWidth: number, chartHeight: number, numericalTop: number, widgetConfig: NumericalGraphWidgetData, visibleStartTime: number, visibleEndTime: number, timeToX: (time: number) => number, hoverTime: number | null, hoveredSignal: RefObject<string | null> | undefined): Array<{ name: string; value: string }> | null {
    const series = widgetConfig.data;

    if (series.length === 0) {
        return [];
    }

    const selectedLODs = series.map((s) => s.lods[selectLOD(s, visibleEndTime - visibleStartTime, chartWidth)]);

    const series_bounds = selectedLODs.map((lod) => {
        const left = bisect(lod.timestamps, visibleStartTime);
        const right = bisect(lod.timestamps, visibleEndTime);
        return {
            left,
            right,
            min: lod.data.get_min(left, right) - 0.2,
            max: lod.data.get_max(left, right) + 0.2,
        };
    });

    let [all_series_min, all_series_max] = series_bounds.reduce(([min, max], meta) => [Math.min(min, meta.min), Math.max(max, meta.max)], [Infinity, -Infinity]);

    // edge case where all values are the same
    if (all_series_min === all_series_max) {
        all_series_min -= 1;
        all_series_max += 1;
    }

    context.strokeStyle = "#333";
    context.lineWidth = 1;
    context.beginPath();

    // y-axis
    context.moveTo(CHART_PADDING.left, numericalTop);
    context.lineTo(CHART_PADDING.left, numericalTop + chartHeight);

    // x-axis
    context.lineTo(width - CHART_PADDING.right, numericalTop + chartHeight);
    context.stroke();

    // grid lines
    context.strokeStyle = "#e0e0e0";
    context.lineWidth = 0.5;
    const numGridLines = 5;
    for (let i = 0; i <= numGridLines; i++) {
        const y = numericalTop + (chartHeight / numGridLines) * i;
        context.beginPath();
        context.moveTo(CHART_PADDING.left, y);
        context.lineTo(width - CHART_PADDING.right, y);
        context.stroke();
    }

    // y-axis labels
    context.fillStyle = "#000000";
    context.font = "12px sans-serif";
    context.textAlign = "right";
    context.textBaseline = "middle";
    for (let i = 0; i <= numGridLines; i++) {
        const value = all_series_max - ((all_series_max - all_series_min) / numGridLines) * i;
        const y = numericalTop + (chartHeight / numGridLines) * i;
        context.fillText(value.toFixed(2), CHART_PADDING.left - 5, y);
    }

    context.save();
    context.beginPath();
    context.rect(CHART_PADDING.left, numericalTop, chartWidth, chartHeight);
    context.clip();

    for (let seriesIndex = 0; seriesIndex < series.length; seriesIndex++) {
        const lodSeries = series[seriesIndex];
        const lod = selectedLODs[seriesIndex];
        const { left, right } = series_bounds[seriesIndex];

        let strokeCol = widgetConfig.options.colorPalette[widgetConfig.signals[seriesIndex]?.name ?? ""]?.hex() ?? "#333";
        let lineWidth = 2;

        if (hoveredSignal?.current && hoveredSignal.current !== lodSeries.label) {
            strokeCol = "#808080";
        } else if (hoveredSignal?.current && hoveredSignal.current === lodSeries.label) {
            lineWidth = 3;
        }

        context.strokeStyle = strokeCol;
        context.lineWidth = lineWidth;
        context.beginPath();

        let initialMove = false;

        for (let i = left - 1; i <= right + 1; i++) {
            const time = lod.timestamps[i];
            const value = lod.data.get(i);
            const x = timeToX(time);
            const y = numericalTop + chartHeight - ((value - all_series_min) / (all_series_max - all_series_min)) * chartHeight;

            if (!initialMove) {
                context.moveTo(x, y);
                initialMove = true;
            } else {
                context.lineTo(x, y);
            }
        }
        context.stroke();
    }

    if (hoverTime !== null) {
        const result: Array<{ name: string; value: string }> = [];

        for (let seriesIndex = 0; seriesIndex < series.length; seriesIndex++) {
            const lodSeries = series[seriesIndex];
            const lod = selectedLODs[seriesIndex];
            if (lod.timestamps.length === 0) continue;

            const idx = bisect(lod.timestamps, hoverTime);
            let closestIdx = idx;

            if (idx >= lod.timestamps.length) {
                closestIdx = lod.timestamps.length - 1;
            } else if (idx > 0) {
                const diffLeft = Math.abs(lod.timestamps[idx - 1] - hoverTime);
                const diffRight = Math.abs(lod.timestamps[idx] - hoverTime);
                closestIdx = diffLeft < diffRight ? idx - 1 : idx;
            }

            const value = lod.data.get(closestIdx);
            const drawX = timeToX(lod.timestamps[closestIdx]);
            const y = numericalTop + chartHeight - ((value - all_series_min) / (all_series_max - all_series_min)) * chartHeight;

            context.beginPath();
            context.fillStyle = widgetConfig.options.colorPalette[widgetConfig.signals[seriesIndex]?.name ?? ""]?.hex() ?? "#333";
            context.strokeStyle = "#ffffff";
            context.lineWidth = 1.5;
            context.arc(drawX, y, 4, 0, Math.PI * 2);
            context.fill();
            context.stroke();

            result.push({
                name: lodSeries.label,
                value: value.toFixed(2),
            });
        }

        context.restore();
        return result;
    }

    context.restore();
    return [];
}

function render_tooltip(
    context: CanvasRenderingContext2D,
    width: number,
    height: number, // layout
    hoverTime: number,
    hover_value: Array<{ name: string; value: string }>,
    timeToX: (t: number) => number
) {
    if (timeToX(hoverTime) < CHART_PADDING.left || timeToX(hoverTime) > width - CHART_PADDING.right) {
        return;
    }

    const hoverDate = new Date(hoverTime);
    const ms = hoverDate.getMilliseconds().toString().padStart(3, "0");
    const time_string = `${DATE_FORMATTER.format(hoverDate)} ${TIME_FORMATTER.format(hoverDate)}.${ms}`;
    const tooltip_lines = [time_string, ...hover_value.map(({ name, value }) => `${name}: ${value}`)];

    context.setLineDash([4, 4]);
    context.strokeStyle = "rgba(0,0,0,0.6)";
    context.lineWidth = 1;
    context.beginPath();
    context.moveTo(timeToX(hoverTime), CHART_PADDING.top);
    context.lineTo(timeToX(hoverTime), height - CHART_PADDING.bottom);
    context.stroke();
    context.setLineDash([]);

    const font = "12px sans-serif";
    context.font = font;
    const lineHeight = 16;
    const horizontalPadding = 10;
    const verticalPadding = 8;
    let tooltipWidth = 0;
    tooltip_lines.forEach((line) => {
        tooltipWidth = Math.max(tooltipWidth, context.measureText(line).width);
    });
    tooltipWidth += horizontalPadding * 2;
    const tooltipHeight = tooltip_lines.length * lineHeight + verticalPadding;

    let tooltipX = timeToX(hoverTime) + 10;
    if (tooltipX + tooltipWidth > width - CHART_PADDING.right) {
        tooltipX = timeToX(hoverTime) - 10 - tooltipWidth;
    }

    // determine tooltip Y
    let tooltipY = CHART_PADDING.top + 20;

    const minY = CHART_PADDING.top;
    const maxY = height - CHART_PADDING.bottom - tooltipHeight;
    tooltipY = Math.min(Math.max(tooltipY, minY), maxY);

    context.fillStyle = "rgba(17, 24, 39, 0.85)";
    context.fillRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);

    context.strokeStyle = "rgba(255, 255, 255, 0.25)";
    context.lineWidth = 1;
    context.strokeRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);

    context.fillStyle = "#ffffff";
    context.textAlign = "left";
    context.textBaseline = "top";

    tooltip_lines.forEach((line, idx) => {
        context.fillText(line, tooltipX + horizontalPadding, tooltipY + verticalPadding / 2 + idx * lineHeight);
    });
}

export default function render(context: CanvasRenderingContext2D, width: number, height: number, layoutRef: RefObject<ChartLayout | null>, chartData: WidgetData, timeTickCount: number, hoverTime: number | null, hoveredSignal: RefObject<string | null> | undefined, { min: visibleStartTime, max: visibleEndTime }: { min: number; max: number }) {
    context.clearRect(0, 0, width, height);

    const numericalTop = CHART_PADDING.top;
    const chartHeight = Math.max(0, height - numericalTop - CHART_PADDING.bottom);
    const chartWidth = width - CHART_PADDING.left - CHART_PADDING.right;

    const timeRange = Math.max(1, visibleEndTime - visibleStartTime);

    const timeToX = (time: number) => {
        return CHART_PADDING.left + ((time - visibleStartTime) / timeRange) * chartWidth;
    };

    layoutRef.current = {
        minTime: visibleStartTime,
        timeRange,
        chartWidth,
        paddingLeft: CHART_PADDING.left,
    };

    let hover_value: Array<{ name: string; value: string }> | null = null;

    if (chartData.type === "enumTimeline") {
        const LEGEND_HEIGHT = 30;
        const ENUM_STRIP_HEIGHT = 40;
        const ENUM_STRIP_GAP = 40;
        hover_value = render_enum(
            context,
            width,
            chartData,
            visibleStartTime,
            visibleEndTime,
            0, // TODO
            timeToX,
            LEGEND_HEIGHT,
            ENUM_STRIP_HEIGHT,
            ENUM_STRIP_GAP,
            hoverTime,
            hoveredSignal
        );
    } else if (chartData.type === "numericalGraph") {
        hover_value = render_numerical(context, width, chartWidth, chartHeight, numericalTop, chartData, visibleStartTime, visibleEndTime, timeToX, hoverTime, hoveredSignal);
    }

    // x-axis tick marks & labels
    const numTimeTicks = Math.max(1, Math.floor(timeTickCount));
    context.strokeStyle = "#999";
    context.lineWidth = 1;
    context.textAlign = "center";
    context.textBaseline = "top";
    context.fillStyle = "#000000";

    const tickSpacing = niceNumber(timeRange / numTimeTicks, true);
    const firstTick = Math.floor(visibleStartTime / tickSpacing) * tickSpacing;
    const lastTick = Math.ceil(visibleEndTime / tickSpacing) * tickSpacing;

    context.beginPath();
    for (let tick = firstTick; tick <= lastTick; tick += tickSpacing) {
        const x = timeToX(tick);
        if (x >= CHART_PADDING.left && x <= width - CHART_PADDING.right) {
            context.moveTo(x, height - CHART_PADDING.bottom);
            context.lineTo(x, height - CHART_PADDING.bottom + 6);
        }
    }
    context.stroke();

    for (let tick = firstTick; tick <= lastTick; tick += tickSpacing) {
        const x = timeToX(tick);
        if (x < CHART_PADDING.left - 10 || x > width - CHART_PADDING.right + 10) continue;

        const dateObj = new Date(tick);
        const msLabel = dateObj.getMilliseconds().toString().padStart(3, "0");
        const timeLabel = `${TIME_FORMATTER.format(dateObj)}.${msLabel}`;
        const dateLabel = DATE_FORMATTER.format(dateObj);

        context.fillText(timeLabel, x, height - CHART_PADDING.bottom + 8);
        context.fillText(dateLabel, x, height - CHART_PADDING.bottom + 24);
    }

    if (hoverTime !== null) {
        render_tooltip(context, width, height, hoverTime, hover_value!, timeToX);
    }
}

export function render_empty(context: CanvasRenderingContext2D, width: number, height: number) {
    context.clearRect(0, 0, width, height);

    context.fillStyle = "#666";
    context.font = "14px sans-serif";
    context.textAlign = "center";
    context.textBaseline = "middle";
    context.fillText("No data collected yet.", width / 2, height / 2);
    context.restore();
}
