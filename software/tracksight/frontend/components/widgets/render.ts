import { RefObject } from "react";
import { ENUM_COLORS } from "@/components/widgets/signalColors";
import { SignalType } from "@/lib/types/Signal";
import { NumericalSeries, ChartLayout, ChartData, EnumSeries } from "./chart_types";
import { bisect } from "@/lib/bisect";

// TODO reduce to bisect right
// first enum index where the enum's end time (timestamps[i+1]) >= targetTime
function binarySearchForFirstEnumIndex(
    timestamps: number[],
    targetTime: number
): number {
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
const CHART_PADDING = { top: 15, right: 20, bottom: 40, left: 60 };
const ENUM_STRIP_HEIGHT = 20;
const ENUM_STRIP_GAP = 10;

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
    context: CanvasRenderingContext2D, width: number, // container context
    all_series: Array<EnumSeries>,
    // rendering
    visibleStartTime: number, visibleEndTime: number, latestDateTime: number, timeToX: (time: number) => number, // time range
    LEGEND_HEIGHT: number, ENUM_STRIP_HEIGHT: number, ENUM_STRIP_GAP: number, // layout
    hover: { x: number; y: number; } | null
): Array<{ name: string, value: string }> | null {
    // DRAWING LEGEND
    // context.textAlign = "left";
    // context.textBaseline = "top";
    // context.font = "10px sans-serif";
    // let legendX = CHART_PADDING.left;
    // const legendY = CHART_PADDING.top;
    // const allEnumValues = new Set<string>();
    // enumSeriesIndices.forEach((idx) => {
    //     uniqueEnumValues[idx]?.forEach((v) => allEnumValues.add(v));
    // });
    // const sortedAllEnums = Array.from(allEnumValues).sort();
    // sortedAllEnums.forEach((val, i) => {
    //     const color = val === "N/A" ? NA_COLOR : ENUM_COLORS[i % ENUM_COLORS.length];

    //     context.fillStyle = color;
    //     context.fillRect(legendX, legendY, 10, 10);

    //     context.fillStyle = "#ffffff"; // Assuming dark background based on numerical chart
    //     context.fillText(val, legendX + 14, legendY);

    //     const textWidth = context.measureText(val).width;
    //     legendX += 14 + textWidth + 15;
    // });

    let currentStripY = CHART_PADDING.top + LEGEND_HEIGHT;
    for (let series_idx = 0; series_idx < all_series.length; series_idx++) {
        const series = all_series[series_idx];
        const { data: seriesData, timestamps: seriesTimestamps } = series;

        context.fillStyle = "#000000";
        context.textAlign = "right";
        context.textBaseline = "middle";
        context.fillText(
            series.label,
            CHART_PADDING.left - 10,
            currentStripY + ENUM_STRIP_HEIGHT / 2
        );

        for (
            let data_idx = binarySearchForFirstEnumIndex(seriesTimestamps, visibleStartTime);
            data_idx <= binarySearchForFirstEnumIndex(seriesTimestamps, visibleEndTime);
            data_idx++
        ) {
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
            context.fillStyle = ENUM_COLORS[value % ENUM_COLORS.length];
            context.fillRect(
                startX,
                currentStripY,
                barWidth,
                ENUM_STRIP_HEIGHT
            );
        }

        // move to the next strip
        currentStripY += ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP;
    }

    if (hover !== null) {
        // TODO calculate hover value for enums
    }

    return [];
}

function render_numerical(
    context: CanvasRenderingContext2D, width: number, chartWidth: number, chartHeight: number,
    numericalTop: number,
    series: NumericalSeries[], // series data
    visibleStartTime: number, visibleEndTime: number, timeToX: (time: number) => number, // time range
    hover: { x: number; y: number; } | null
): Array<{ name: string, value: string }> | null {
    // TODO edge case where series.length === 0
    const series_bounds = series.map(s => {
        const left = bisect(s.timestamps, visibleStartTime)
        const right = bisect(s.timestamps, visibleEndTime)
        return ({
            left,
            right,
            min: s.data.get_min(left, right),
            max: s.data.get_max(left, right)
        })
    });

    let [all_series_min, all_series_max] = series_bounds.reduce(([min, max], meta) =>
        [Math.min(min, meta.min), Math.max(max, meta.max)], [Infinity, -Infinity]);

    // edge case where all values are the same
    if (all_series_min === all_series_max) {
        all_series_min -= 1;
        all_series_max += 1;
    }

    // console.log("Numerical value range:", minValue, maxValue);

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

    // axis labels
    context.fillStyle = "#000000";
    context.font = "12px sans-serif";
    context.textAlign = "right";
    context.textBaseline = "middle";

    // y-axis labels
    for (let i = 0; i <= numGridLines; i++) {
        const value = all_series_max - ((all_series_max - all_series_min) / numGridLines) * i;
        const y = numericalTop + (chartHeight / numGridLines) * i;
        context.fillText(value.toFixed(2), CHART_PADDING.left - 5, y);
    }

    context.save();
    context.beginPath();
    context.rect(CHART_PADDING.left, numericalTop, chartWidth, chartHeight);
    context.clip();

    // draw data series
    // numericalSeriesIndices.forEach((seriesIndex) => {
    for (let seriesIndex = 0; seriesIndex < series.length; seriesIndex++) {
        const s = series[seriesIndex];
        const { left, right } = series_bounds[seriesIndex];

        context.strokeStyle = s.color || "#000";
        context.lineWidth = 2;
        context.beginPath();

        // only iterate through visible data points and last point before
        for (let i = left - 1; i <= right + 1; i++) {
            const time = s.timestamps[i];
            const value = s.data.get(i);
            const x = timeToX(time);
            const y = numericalTop + chartHeight - ((value - all_series_min) / (all_series_max - all_series_min)) * chartHeight
            if (false) { // TODO if gap between current point and last point is greater than threshold
                context.moveTo(x, y);
            } else {
                context.lineTo(x, y);
            }
        }
        context.stroke();
    }

    if (hover !== null) {
        // TODO calculate closes point if hovering
        // TODO render the numerical circle
        // const y = numericalTop + chartHeight - ((value - all_series_min) / (all_series_max - all_series_min)) * chartHeight;
        // // draw the circle at the hovered point
        // context.beginPath();
        // context.fillStyle = s.color || "#4f46e5";
        // context.strokeStyle = "#ffffff";
        // context.lineWidth = 1.5;
        // context.arc(drawX, y, 4, 0, Math.PI * 2);
        // context.fill();
        // context.stroke();
    }


    context.restore();

    return [];
}

function render_tooltip(
    context: CanvasRenderingContext2D, width: number, height: number, // layout
    hover: { x: number; y: number; },
    hover_value: Array<{ name: string, value: string }>,
    XToTime: (x: number) => number
) {
    // bounds check
    const withinX = hover.x >= CHART_PADDING.left && hover.x <= width - CHART_PADDING.right;
    const withinY = hover.y >= CHART_PADDING.top && hover.y <= height - CHART_PADDING.bottom;
    if (!(withinX && withinY)) {
        return
    }

    const snappedTimestamp = XToTime(hover.x);
    const hoverDate = new Date(snappedTimestamp);
    const ms = hoverDate.getMilliseconds().toString().padStart(3, "0");
    const time_string = `${DATE_FORMATTER.format(hoverDate)} ${TIME_FORMATTER.format(hoverDate)}.${ms}`
    const tooltip_lines = hover_value.map(({ name, value }) => `${name}: ${value}`);


    context.setLineDash([4, 4]);
    context.strokeStyle = "rgba(255,255,255,0.6)";
    context.lineWidth = 1;
    context.beginPath();
    context.moveTo(hover.x, CHART_PADDING.top);
    context.lineTo(hover.x, height - CHART_PADDING.bottom);
    context.stroke();
    context.setLineDash([]);

    const font = "12px sans-serif";
    context.font = font;
    const lineHeight = 16;
    const horizontalPadding = 10;
    const verticalPadding = 8;
    let tooltipWidth = 0;
    tooltip_lines.forEach((line) => {
        tooltipWidth = Math.max(
            tooltipWidth,
            context.measureText(line).width
        );
    });
    tooltipWidth += horizontalPadding * 2;
    const tooltipHeight = tooltip_lines.length * lineHeight + verticalPadding;

    let tooltipX = hover.x + 10;
    if (tooltipX + tooltipWidth > width - CHART_PADDING.right) {
        tooltipX = hover.x - 10 - tooltipWidth;
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
        context.fillText(
            line,
            tooltipX + horizontalPadding,
            tooltipY + verticalPadding / 2 + idx * lineHeight
        );
    });
}

export default function render(
    context: CanvasRenderingContext2D, width: number, height: number, // dimensions of chart
    layoutRef: RefObject<ChartLayout | null>, // seems kinda bodgey
    chartData: ChartData,
    timeTickCount: number,
    hoverPixelRef: RefObject<{ x: number; y: number; } | null>,
    { min: visibleStartTime, max: visibleEndTime }: { min: number; max: number },
) {
    context.clearRect(0, 0, width, height);

    // calculate layout
    const numericalTop = CHART_PADDING.top;
    const chartHeight = Math.max(0, height - numericalTop - CHART_PADDING.bottom);
    const chartWidth = width - CHART_PADDING.left - CHART_PADDING.right;

    // data ranges - only use visible range
    const timeRange = Math.max(1, visibleEndTime - visibleStartTime);

    /**
     * converts a time value to an x coordinate within the chart area
     * @param time
     * @returns 
     */
    const timeToX = (time: number) => {
        return CHART_PADDING.left + ((time - visibleStartTime) / timeRange) * chartWidth;
    };
    const XToTime = (x: number) => {
        return visibleStartTime + ((x - CHART_PADDING.left) / chartWidth) * timeRange;
    }

    layoutRef.current = {
        minTime: visibleStartTime,
        timeRange,
        chartWidth,
        paddingLeft: CHART_PADDING.left,
    };

    // --- RENDER ENUMS ---
    let hover_value: Array<{ name: string, value: string }> | null = null;
    if (chartData.type === SignalType.ENUM) {
        const LEGEND_HEIGHT = 30;
        const ENUM_STRIP_HEIGHT = 40;
        const ENUM_STRIP_GAP = 40;
        hover_value = render_enum(context, width, chartData.all_series, visibleStartTime, visibleEndTime, 0, // TODO
            timeToX, LEGEND_HEIGHT, ENUM_STRIP_HEIGHT, ENUM_STRIP_GAP, hoverPixelRef.current);
    }
    // --- RENDER NUMERICAL ---
    else if (chartData.type === SignalType.NUMERICAL) {
        hover_value = render_numerical(
            context, width, chartWidth, chartHeight, numericalTop,
            chartData.all_series, visibleStartTime, visibleEndTime, timeToX, hoverPixelRef.current
        );
    } // end hasNumerical

    // x-axis tick marks & labels (Shared for both)
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

    if (hoverPixelRef.current !== null) {
        const hover = hoverPixelRef.current;
        render_tooltip(
            context, width, height,
            hover, hover_value!, XToTime
        );
    }

    context.restore();
}
