import { RefObject } from "react";
import { ENUM_COLORS, NA_COLOR } from "@/components/widgets/signalColors";

// first index where timestamp >= targetTime
function binarySearchForFirstVisibleIndex(
    timestamps: number[],
    targetTime: number
): number {
    // Lower-bound: first index where timestamp >= targetTime.
    // If none, returns timestamps.length.
    let left = 0;
    let right = timestamps.length;
    while (left < right) {
        const mid = left + ((right - left) >> 1);
        if (timestamps[mid] >= targetTime) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

// last index where timestamp <= targetTime
function binarySearchForLastVisibleIndex(
    timestamps: number[],
    targetTime: number
): number {
    let left = 0;
    let right = timestamps.length - 1;
    let result = timestamps.length - 1;

    while (left <= right) {
        const mid = Math.floor((left + right) / 2);
        if (timestamps[mid] <= targetTime) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

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

export interface ChunkStats {
    min: number[];
    max: number[];
};

export interface SeriesMeta {
    label: string;
    color?: string;
    min?: number;
    max?: number;
};

export interface PreparedChartData {
    timestamps: number[];
    seriesData: Array<(number | string | null)[]>;
    chunkSize: number;
    chunkStats: ChunkStats[];
    enumSeriesIndices: number[]; // indices of series that are enums
    numericalSeriesIndices: number[]; // indices of series that are numerical
    uniqueEnumValues: Record<number, string[]>; // map of series index to unique enum values
};

export interface ChartLayout {
    minTime: number;
    timeRange: number;
    chartWidth: number;
    paddingLeft: number;
}

export interface DownsampleOptions {
    minVisiblePoints: number;
    minPointsPerPixel: number;
}

// THIS IS FOR DEBUG! we will remove this when downsampling is fully implemented and tested, to avoid confusion about which numbers are real vs estimated.
// but for now we want to expose this info for testing and validation purpose s i think
export interface DownsampleInfo {
    seriesLabel?: string;
    visiblePoints: number;
    renderedPoints: number;
    pointsPerPixel: number;
    downsampled: boolean;
}

const timeFormatter = new Intl.DateTimeFormat(undefined, {
    hour: "numeric",
    minute: "2-digit",
    second: "2-digit",
    hour12: true,
});

const dateFormatter = new Intl.DateTimeFormat(undefined, {
    year: "numeric",
    month: "short",
    day: "2-digit",
});

export default function render(context: CanvasRenderingContext2D, width: number, height: number, preparedData: PreparedChartData,
    series: SeriesMeta[], timeTickCount: number, externalHoverTimestamp: number | null,
    hoverPixelRef: RefObject<{ x: number; y: number; } | null>, tooltipBufferRef: RefObject<string[]>,
    layoutRef: RefObject<ChartLayout | null>, visibleTimeRange: { min: number; max: number },
    scalingMode: "absolute" | "relative" = "absolute",
    downsampleOptions: DownsampleOptions = { minVisiblePoints: 50_000, minPointsPerPixel: 8 },
    downsampleInfoRef?: RefObject<DownsampleInfo | null>
) {
    context.clearRect(0, 0, width, height);

    const {
        timestamps, seriesData, chunkSize, chunkStats, enumSeriesIndices, numericalSeriesIndices, uniqueEnumValues,
    } = preparedData;

    //if (!timestamps || timestamps.length === 0) return;

    const clampedTickCount = Math.max(1, Math.floor(timeTickCount));
    const padding = { top: 20, right: 20, bottom: 56, left: 60 };

    const ENUM_STRIP_HEIGHT = 20;
    const ENUM_STRIP_GAP = 10;
    const LEGEND_HEIGHT = enumSeriesIndices.length > 0 ? 30 : 0;
    const enumSectionHeight = enumSeriesIndices.length > 0
        ? enumSeriesIndices.length * (ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP) +
        LEGEND_HEIGHT +
        10
        : 0;

    const numericalTop = padding.top + enumSectionHeight;
    const chartWidth = width - padding.left - padding.right;
    const chartHeight = Math.max(0, height - numericalTop - padding.bottom);

    const visibleStartTime = visibleTimeRange.min;
    const visibleEndTime = visibleTimeRange.max;

    const startIndex = binarySearchForFirstVisibleIndex(
        timestamps,
        visibleStartTime
    );

    const endIndex = binarySearchForLastVisibleIndex(
        timestamps,
        visibleEndTime
    );

    const visiblePointCount = Math.max(0, endIndex - startIndex + 1);

    // data ranges - only use visible range
    const minTime = visibleStartTime;
    const maxTime = visibleEndTime;
    const rawTimeRange = maxTime - minTime;
    const timeRange = rawTimeRange <= 0 ? 1 : rawTimeRange;

    const timeToX = (time: number) => {
        return padding.left + ((time - minTime) / timeRange) * chartWidth;
    };

    layoutRef.current = {
        minTime,
        timeRange,
        chartWidth,
        paddingLeft: padding.left,
    };

    // --- RENDER ENUMS ---
    if (enumSeriesIndices.length > 0) {
        // legend
        context.textAlign = "left";
        context.textBaseline = "top";
        context.font = "10px sans-serif";

        let legendX = padding.left;
        const legendY = padding.top;

        const allEnumValues = new Set<string>();
        enumSeriesIndices.forEach((idx) => {
            uniqueEnumValues[idx]?.forEach((v) => allEnumValues.add(v));
        });
        const sortedAllEnums = Array.from(allEnumValues).sort();

        sortedAllEnums.forEach((val, i) => {
            const color = val === "N/A" ? NA_COLOR : ENUM_COLORS[i % ENUM_COLORS.length];

            context.fillStyle = color;
            context.fillRect(legendX, legendY, 10, 10);

            context.fillStyle = "#ffffff"; // Assuming dark background based on numerical chart
            context.fillText(val, legendX + 14, legendY);

            const textWidth = context.measureText(val).width;
            legendX += 14 + textWidth + 15;
        });

        let currentStripY = padding.top + LEGEND_HEIGHT;

        enumSeriesIndices.forEach((seriesIndex) => {
            const dataPoints = seriesData[seriesIndex];
            context.fillStyle = series[seriesIndex]?.color || "#ffffff";
            context.textAlign = "right";
            context.textBaseline = "middle";
            context.fillText(
                series[seriesIndex].label,
                padding.left - 10,
                currentStripY + ENUM_STRIP_HEIGHT / 2
            );

            const enumStartIndex = binarySearchForFirstEnumIndex(
                timestamps,
                visibleStartTime
            );

            for (let i = enumStartIndex; i <= endIndex; i++) {
                const time = timestamps[i];
                const value = dataPoints[i] as string | null;

                if (value === null) continue;

                let endTime: number;
                if (i === endIndex) {
                    endTime = visibleEndTime;
                } else {
                    endTime = timestamps[i + 1];
                }

                const startX = Math.max(padding.left, timeToX(time));
                const endX = Math.min(width - padding.right, timeToX(endTime));
                const barWidth = endX - startX;

                if (barWidth < 0.5) continue;

                const valIndex = sortedAllEnums.indexOf(value);
                const color = value === "N/A"
                    ? NA_COLOR
                    : ENUM_COLORS[valIndex % ENUM_COLORS.length];

                context.fillStyle = color;
                context.fillRect(
                    startX,
                    currentStripY,
                    barWidth,
                    ENUM_STRIP_HEIGHT
                );
            }

            currentStripY += ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP;
        });
    }

    // --- RENDER NUMERICAL ---
    const hasNumerical = numericalSeriesIndices.length > 0;

    // We will store min/max per series here for relative mode, or global for absolute
    const seriesRanges: Record<number, { min: number; max: number }> = {};
    let globalMin = Infinity;
    let globalMax = -Infinity;

    if (hasNumerical) {
        const computeVisibleRange = ( dataPoints: (number | string | null)[], stats?: ChunkStats) => {
            if (startIndex > endIndex) return { min: Infinity, max: -Infinity };

            const windowSize = endIndex - startIndex + 1;
            const shouldUseChunks = chunkSize > 0 && stats && windowSize > chunkSize * 2;

            let localMin = Infinity;
            let localMax = -Infinity;

            if (!shouldUseChunks) {
                for (let i = startIndex; i <= endIndex; i++) {
                    const value = dataPoints[i];
                    if (value === null ||
                        value === undefined ||
                        typeof value === "string")
                        continue;
                    if (value < localMin) localMin = value;
                    if (value > localMax) localMax = value;
                }
                return { min: localMin, max: localMax };
            }

            const startChunk = Math.floor(startIndex / chunkSize);
            const endChunk = Math.floor(endIndex / chunkSize);

            const scanRange = (from: number, to: number) => {
                for (let i = from; i <= to; i++) {
                    const value = dataPoints[i];
                    if (value === null ||
                        value === undefined ||
                        typeof value === "string")
                        continue;
                    if (value < localMin) localMin = value;
                    if (value > localMax) localMax = value;
                }
            };

            const startChunkEnd = Math.min(
                (startChunk + 1) * chunkSize - 1,
                endIndex
            );
            scanRange(startIndex, startChunkEnd);

            if (endChunk > startChunk + 1) {
                for (let chunk = startChunk + 1; chunk <= endChunk - 1; chunk++) {
                    const chunkMin = stats.min[chunk];
                    const chunkMax = stats.max[chunk];
                    if (Number.isFinite(chunkMin) && chunkMin < localMin) {
                        localMin = chunkMin;
                    }
                    if (Number.isFinite(chunkMax) && chunkMax > localMax) {
                        localMax = chunkMax;
                    }
                }
            }

            const tailStart = Math.max(endChunk * chunkSize, startChunkEnd + 1);
            if (tailStart <= endIndex) {
                scanRange(tailStart, endIndex);
            }

            return { min: localMin, max: localMax };
        };

        // for downsampling
        const computeMinMaxInIndexRange = ( dataPoints: (number | string | null)[], fromIndex: number, toIndex: number, stats?: ChunkStats) => {
            if (fromIndex > toIndex) return { min: Infinity, max: -Infinity };

            let localMin = Infinity;
            let localMax = -Infinity;

            const scanRange = (from: number, to: number) => {
                for (let i = from; i <= to; i++) {
                    const value = dataPoints[i];
                    if (value === null || value === undefined || typeof value === "string") continue;
                    if (value < localMin) localMin = value;
                    if (value > localMax) localMax = value;
                }
            };

            const windowSize = toIndex - fromIndex + 1;
            const shouldUseChunks = chunkSize > 0 && stats && windowSize > chunkSize * 2;
            if (!shouldUseChunks) {
                scanRange(fromIndex, toIndex);
                return { min: localMin, max: localMax };
            }

            const startChunk = Math.floor(fromIndex / chunkSize);
            const endChunk = Math.floor(toIndex / chunkSize);

            const startChunkEnd = Math.min((startChunk + 1) * chunkSize - 1, toIndex);
            scanRange(fromIndex, startChunkEnd);

            if (endChunk > startChunk + 1) {
                for (let chunk = startChunk + 1; chunk <= endChunk - 1; chunk++) {
                    const chunkMin = stats.min[chunk];
                    const chunkMax = stats.max[chunk];
                    if (Number.isFinite(chunkMin) && chunkMin < localMin) localMin = chunkMin;
                    if (Number.isFinite(chunkMax) && chunkMax > localMax) localMax = chunkMax;
                }
            }

            const tailStart = Math.max(endChunk * chunkSize, startChunkEnd + 1);
            if (tailStart <= toIndex) {
                scanRange(tailStart, toIndex);
            }

            return { min: localMin, max: localMax };
        };

        // 1. Calculate ranges for all visible numerical series
        numericalSeriesIndices.forEach((seriesIndex) => {
            const meta = series[seriesIndex];

            let sMin = Infinity;
            let sMax = -Infinity;

            // Use fixed bounds if available
            if (meta && typeof meta.min === 'number' && typeof meta.max === 'number') {
                sMin = meta.min;
                sMax = meta.max;
            } else {
                // Otherwise calculate from visible data
                const dataPoints = seriesData[seriesIndex];
                const stats = chunkStats[seriesIndex];
                const { min, max } = computeVisibleRange(dataPoints, stats);
                if (Number.isFinite(min) && Number.isFinite(max)) {
                    sMin = min;
                    sMax = max;
                }
            }

            // Fallback if still invalid (e.g. no visible data and no fixed bounds)
            // We'll handle this by checking Number.isFinite later

            seriesRanges[seriesIndex] = { min: sMin, max: sMax };

            if (Number.isFinite(sMin) && Number.isFinite(sMax)) {
                globalMin = Math.min(globalMin, sMin);
                globalMax = Math.max(globalMax, sMax);
            }
        });

        // If absolute mode, we need to enforce global min/max for everyone
        if (scalingMode === "absolute") {
            if (!Number.isFinite(globalMin) || !Number.isFinite(globalMax)) {
                // No valid data at all
                globalMin = 0;
                globalMax = 1;
            } else if (globalMin === globalMax) {
                globalMin -= 1;
                globalMax += 1;
            }

            // Override all individual ranges with global
            numericalSeriesIndices.forEach(idx => {
                seriesRanges[idx] = { min: globalMin, max: globalMax };
            });
        } else {
            // Relative mode: Handle edge case per series
            numericalSeriesIndices.forEach(idx => {
                const r = seriesRanges[idx];
                if (!Number.isFinite(r.min) || !Number.isFinite(r.max)) {
                    // Default range if no data
                    seriesRanges[idx] = { min: 0, max: 1 };
                } else if (r.min === r.max) {
                    seriesRanges[idx] = { min: r.min - 1, max: r.max + 1 };
                }
            });
        }

        const valueToY = (value: number, rangeMin: number, rangeMax: number) => {
            return (
                numericalTop + chartHeight - ((value - rangeMin) / (rangeMax - rangeMin)) * chartHeight
            );
        };

        context.strokeStyle = "#333";
        context.lineWidth = 1;
        context.beginPath();

        // y-axis line (left)
        context.moveTo(padding.left, numericalTop);
        context.lineTo(padding.left, numericalTop + chartHeight);

        // x-axis line (bottom)
        context.lineTo(width - padding.right, numericalTop + chartHeight);
        context.stroke();

        // grid lines - use the first series range (or global in absolute) for reference grid
        // In relative mode, grid lines might be confusing if we map them to values, 
        // so we just draw horizontal lines at percentage intervals.
        context.strokeStyle = "#e0e0e0";
        context.lineWidth = 0.5;
        const numGridLines = 5;
        for (let i = 0; i <= numGridLines; i++) {
            const y = numericalTop + (chartHeight / numGridLines) * i;
            context.beginPath();
            context.moveTo(padding.left, y);
            context.lineTo(width - padding.right, y);
            context.stroke();
        }

        // axis labels
        context.font = "12px sans-serif";
        context.textAlign = "right";
        context.textBaseline = "middle";

        if (scalingMode === "absolute") {
            context.fillStyle = "#000000";
            // Standard single axis
            for (let i = 0; i <= numGridLines; i++) {
                const value = globalMax - ((globalMax - globalMin) / numGridLines) * i;
                const y = numericalTop + (chartHeight / numGridLines) * i;
                context.fillText(value.toFixed(2), padding.left - 5, y);
            }
        } else {
            // Relative mode: draw axis labels for each series
            // We'll stack them horizontally to the left of the axis
            let xOffset = padding.left - 5;

            numericalSeriesIndices.forEach((seriesIndex) => {
                const range = seriesRanges[seriesIndex];
                const color = series[seriesIndex]?.color || "#000";
                context.fillStyle = color;

                // Draw min and max only to avoid clutter? Or min/mid/max? 
                // Let's draw min, mid, max.
                // We shift xOffset left for each series to create "columns"

                // Top (Max)
                context.fillText(range.max.toFixed(1), xOffset, numericalTop);

                // Bottom (Min)
                context.fillText(range.min.toFixed(1), xOffset, numericalTop + chartHeight);

                // Middle? Maybe too crowded if many signals. Let's stick to min/max for relative mode
                // unless it's just 1 or 2 signals.
                if (numericalSeriesIndices.length <= 2) {
                    const mid = (range.max + range.min) / 2;
                    context.fillText(mid.toFixed(1), xOffset, numericalTop + chartHeight / 2);
                }

                // Move left for next series
                // Estimate width of text?
                const w = Math.max(
                    context.measureText(range.max.toFixed(1)).width,
                    context.measureText(range.min.toFixed(1)).width
                );

                xOffset -= (w + 10);
            });
        }

        context.save();
        context.beginPath();
        context.rect(padding.left, numericalTop, chartWidth, chartHeight);
        context.clip();

        const drawDownsampledEnvelope = ( dataPoints: (number | string | null)[], stats: ChunkStats | undefined, rangeMin: number, rangeMax: number, strokeStyle: string) => {
            const drawStartIndex = Math.max(0, startIndex - 1);
            const drawEndIndex = Math.min((timestamps.length ?? 0) - 1, endIndex + 1);
            if (drawStartIndex > drawEndIndex) return;

            const pixelWidth = Math.max(1, Math.floor(chartWidth));
            const pointCount = drawEndIndex - drawStartIndex + 1;
            const pointsPerPixel = pointCount / pixelWidth;
            const shouldDownsample = pointsPerPixel > downsampleOptions.minPointsPerPixel && pointCount > downsampleOptions.minVisiblePoints;

            if (!shouldDownsample) return { used: false, renderedPoints: pointCount, pointsPerPixel };

            const drawSegment = (seg: Array<{ x: number; yMin: number; yMax: number }>) => {
                if (seg.length < 2) return;

                // filled band
                context.save();
                context.globalAlpha = 0.18;
                context.fillStyle = strokeStyle;
                context.beginPath();
                context.moveTo(seg[0].x, seg[0].yMax);
                for (let i = 1; i < seg.length; i++) {
                    context.lineTo(seg[i].x, seg[i].yMax);
                }
                for (let i = seg.length - 1; i >= 0; i--) {
                    context.lineTo(seg[i].x, seg[i].yMin);
                }
                context.closePath();
                context.fill();
                context.restore();

                // band outlines
                context.strokeStyle = strokeStyle;
                context.lineWidth = 1.25;
                context.beginPath();
                context.moveTo(seg[0].x, seg[0].yMax);
                for (let i = 1; i < seg.length; i++) {
                    context.lineTo(seg[i].x, seg[i].yMax);
                }
                context.stroke();

                context.beginPath();
                context.moveTo(seg[0].x, seg[0].yMin);
                for (let i = 1; i < seg.length; i++) {
                    context.lineTo(seg[i].x, seg[i].yMin);
                }
                context.stroke();
            };

            // thanks claude for the following code; i am ngl i don't know too much about how the bucketing acutally works but i think
            // it does! yay! - jaden

            const seg: Array<{ x: number; yMin: number; yMax: number }> = [];
            let renderedBuckets = 0;

            // Use stable, time-aligned buckets so scrolling doesn't shift bucket boundaries every frame
            // Bucket width is roughly one pixel-worth of time but anchored to an absolute time grid
            const bucketDt = timeRange / pixelWidth;
            if (!(bucketDt > 0) || !Number.isFinite(bucketDt)) {
                return { used: false, renderedPoints: pointCount, pointsPerPixel };
            }

            const bucketIndexStart = Math.floor(minTime / bucketDt);
            const bucketIndexEnd = Math.ceil(maxTime / bucketDt) - 1;
            const maxBuckets = Math.max(1, pixelWidth + 4);
            const bucketCount = Math.min(maxBuckets, Math.max(0, bucketIndexEnd - bucketIndexStart + 1));

            for (let b = 0; b < bucketCount; b++) {
                const idx = bucketIndexStart + b;
                const tLeft = idx * bucketDt;
                const tRight = tLeft + bucketDt;

                let i0 = binarySearchForFirstVisibleIndex(timestamps, tLeft);
                let i1 = binarySearchForFirstVisibleIndex(timestamps, tRight) - 1;

                i0 = Math.max(i0, drawStartIndex);
                i1 = Math.min(i1, drawEndIndex);

                if (i0 > i1) {
                    // gap: flush current segment
                    drawSegment(seg);
                    seg.length = 0;
                    continue;
                }

                const { min, max } = computeMinMaxInIndexRange(dataPoints, i0, i1, stats);
                if (!Number.isFinite(min) || !Number.isFinite(max)) {
                    drawSegment(seg);
                    seg.length = 0;
                    continue;
                }

                // Place the bucket at its mid-time. This keeps x positions stable while scrolling.
                const x = timeToX((tLeft + tRight) / 2);
                if (x < padding.left || x > width - padding.right) {
                    continue;
                }
                const yMin = valueToY(min, rangeMin, rangeMax);
                const yMax = valueToY(max, rangeMin, rangeMax);

                // Ensure yMin is the lower line in screen coords (larger value is lower on canvas)
                const low = Math.max(yMin, yMax);
                const high = Math.min(yMin, yMax);
                seg.push({ x, yMin: low, yMax: high });
                renderedBuckets += 1;
            }

            drawSegment(seg);
            return { used: true, renderedPoints: renderedBuckets, pointsPerPixel };
        };

        // draw data series
        numericalSeriesIndices.forEach((seriesIndex) => {
            const dataPoints = seriesData[seriesIndex];
            const meta = series[seriesIndex];
            const range = seriesRanges[seriesIndex];

            const downsampleResult = drawDownsampledEnvelope(
                dataPoints,
                chunkStats[seriesIndex],
                range.min,
                range.max,
                meta?.color || "#000"
            );

            if (downsampleInfoRef) {
                downsampleInfoRef.current = {
                    seriesLabel: meta?.label,
                    visiblePoints: visiblePointCount,
                    renderedPoints: downsampleResult?.used
                        ? downsampleResult.renderedPoints
                        : visiblePointCount,
                    pointsPerPixel: downsampleResult?.pointsPerPixel ?? 0,
                    downsampled: Boolean(downsampleResult?.used),
                };
            }

            if (downsampleResult?.used) return;

            const drawStartIndex = Math.max(0, startIndex - 1);
            const drawEndIndex = Math.min(timestamps.length - 1, endIndex + 1);

            let loopStart = drawStartIndex;

            // if the last point before is not on screen (i.e. start of data range has no data),
            // then the line color is yellow for that segment.
            if (drawStartIndex < startIndex && drawStartIndex + 1 <= drawEndIndex) {
                const t0 = timestamps[drawStartIndex];
                const v0 = dataPoints[drawStartIndex];
                const t1 = timestamps[drawStartIndex + 1];
                const v1 = dataPoints[drawStartIndex + 1];

                if (typeof v0 === "number" && typeof v1 === "number") {
                    context.beginPath();
                    context.strokeStyle = "#EAB308"; // Yellow
                    context.lineWidth = 2;
                    context.moveTo(timeToX(t0), valueToY(v0, range.min, range.max));
                    context.lineTo(timeToX(t1), valueToY(v1, range.min, range.max));
                    context.stroke();

                    loopStart = drawStartIndex + 1;
                }
            }

            context.strokeStyle = meta?.color || "#000";
            context.lineWidth = 2;
            context.beginPath();

            let pathStarted = false;

            // only iterate through visible data points and last point before
            for (let i = loopStart; i <= drawEndIndex; i++) {
                const time = timestamps[i];
                const value = dataPoints[i];

                if (value === null || typeof value !== "number") {
                    pathStarted = false;
                    continue;
                }

                const x = timeToX(time);
                const y = valueToY(value, range.min, range.max);

                if (!pathStarted) {
                    context.moveTo(x, y);
                    pathStarted = true;
                } else {
                    context.lineTo(x, y);
                }
            }

            context.stroke();
        });

        context.restore();
    } // end hasNumerical


    // x-axis tick marks & labels (Shared for both)
    const numTimeTicks = clampedTickCount;
    context.strokeStyle = "#999";
    context.lineWidth = 1;
    context.textAlign = "center";
    context.textBaseline = "top";
    context.fillStyle = "#000000";

    const niceNumber = (range: number, round: boolean) => {
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
    };

    const tickSpacing = niceNumber(timeRange / numTimeTicks, true);
    const firstTick = Math.floor(minTime / tickSpacing) * tickSpacing;
    const lastTick = Math.ceil(maxTime / tickSpacing) * tickSpacing;

    context.beginPath();
    for (let tick = firstTick; tick <= lastTick; tick += tickSpacing) {
        const x = timeToX(tick);
        if (x >= padding.left && x <= width - padding.right) {
            context.moveTo(x, height - padding.bottom);
            context.lineTo(x, height - padding.bottom + 6);
        }
    }
    context.stroke();

    for (let tick = firstTick; tick <= lastTick; tick += tickSpacing) {
        const x = timeToX(tick);

        if (x < padding.left - 10 || x > width - padding.right + 10) continue;

        const dateObj = new Date(tick);
        const msLabel = dateObj.getMilliseconds().toString().padStart(3, "0");
        const timeLabel = `${timeFormatter.format(dateObj)}.${msLabel}`;
        const dateLabel = dateFormatter.format(dateObj);

        context.fillText(timeLabel, x, height - padding.bottom + 8);
        context.fillText(dateLabel, x, height - padding.bottom + 24);
    }

    // hover interaction (vertical line, points, and tooltip)
    let activeHoverX: number | null = null;
    let activeHoverTimestamp: number | null = null;

    const hover = hoverPixelRef.current;

    if (hover) {
        const withinX = hover.x >= padding.left && hover.x <= width - padding.right;
        const withinY = hover.y >= padding.top && hover.y <= height - padding.bottom;

        if (withinX && withinY) {
            activeHoverX = hover.x;

            const calculatedTime = minTime + ((hover.x - padding.left) / chartWidth) * timeRange;
            activeHoverTimestamp = calculatedTime;
        }
    } else if (externalHoverTimestamp !== null &&
        externalHoverTimestamp !== undefined) {
        if (externalHoverTimestamp >= minTime &&
            externalHoverTimestamp <= maxTime) {
            activeHoverTimestamp = externalHoverTimestamp;
            activeHoverX = timeToX(externalHoverTimestamp);

            // activeHoverY = padding.top + 50;
        }
    }

    // render lines 
    if (activeHoverX !== null && activeHoverTimestamp !== null) {
        const hoverTime = activeHoverTimestamp;

        const clampIndex = (idx: number) => Math.min(Math.max(idx, startIndex), endIndex);

        let low = startIndex;
        let high = endIndex;
        while (low <= high) {
            const mid = Math.floor((low + high) / 2);
            if (timestamps[mid] < hoverTime) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        const rightCandidate = clampIndex(low);
        const leftCandidate = clampIndex(low - 1);

        let nearestIndex = rightCandidate;
        if (Math.abs(timestamps[leftCandidate] - hoverTime) <
            Math.abs(timestamps[nearestIndex] - hoverTime)) {
            nearestIndex = leftCandidate;
        }

        const snappedTimestamp = timestamps[nearestIndex];
        const snappedX = timeToX(snappedTimestamp);

        // Use snapped X for drawing
        const drawX = snappedX;

        context.save();
        context.setLineDash([4, 4]);
        context.strokeStyle = "rgba(255,255,255,0.6)";
        context.lineWidth = 1;
        context.beginPath();
        context.moveTo(drawX, padding.top);
        context.lineTo(drawX, height - padding.bottom);
        context.stroke();
        context.setLineDash([]);

        const tooltipLines = tooltipBufferRef.current;
        tooltipLines.length = 0;
        const hoverDate = new Date(snappedTimestamp);
        const ms = hoverDate.getMilliseconds().toString().padStart(3, "0");
        tooltipLines.push(
            `${dateFormatter.format(hoverDate)} ${timeFormatter.format(
                hoverDate
            )}.${ms}`
        );

        // collect unified enum values for legend color mapping
        /*const allEnumValues = new Set<string>();
        enumSeriesIndices.forEach((idx) => {
            uniqueEnumValues[idx]?.forEach((v) => allEnumValues.add(v));
        });
        const sortedAllEnums = Array.from(allEnumValues).sort();*/

        let firstPointY: number | null = null;

        // TODO: optimize search by only looking at visible indices
        seriesData.forEach((points, idx) => {
            const value = points[nearestIndex];
            if (value === null || value === undefined) return;

            if (typeof value === "number" && hasNumerical && numericalSeriesIndices.includes(idx)) {
                let y = 0;
                const range = seriesRanges[idx];
                if (range &&
                    Number.isFinite(range.min) &&
                    Number.isFinite(range.max)) {
                    y = numericalTop + chartHeight - ((value - range.min) / (range.max - range.min)) * chartHeight;

                    if (firstPointY === null) firstPointY = y;

                    const color = series[idx]?.color || "#4f46e5";
                    context.beginPath();
                    context.fillStyle = color;
                    context.strokeStyle = "#ffffff";
                    context.lineWidth = 1.5;
                    context.arc(drawX, y, 4, 0, Math.PI * 2);
                    context.fill();
                    context.stroke();
                }
            }

            let displayValue = "";
            if (typeof value === "number") {
                displayValue = value.toFixed(2);
            } else {
                displayValue = String(value);
            }

            tooltipLines.push(
                `${series[idx]?.label ?? `Series ${idx + 1}`}: ${displayValue}`
            );
        });

        if (tooltipLines.length > 0) {
            const font = "12px sans-serif";
            context.font = font;
            const lineHeight = 16;
            const horizontalPadding = 10;
            const verticalPadding = 8;
            let tooltipWidth = 0;
            tooltipLines.forEach((line) => {
                tooltipWidth = Math.max(
                    tooltipWidth,
                    context.measureText(line).width
                );
            });
            tooltipWidth += horizontalPadding * 2;
            const tooltipHeight = tooltipLines.length * lineHeight + verticalPadding;

            let tooltipX = drawX + 10;
            if (tooltipX + tooltipWidth > width - padding.right) {
                tooltipX = drawX - 10 - tooltipWidth;
            }

            // determine tooltip Y
            let tooltipY = padding.top + 20;

            const minY = padding.top;
            const maxY = height - padding.bottom - tooltipHeight;
            tooltipY = Math.min(Math.max(tooltipY, minY), maxY);

            context.fillStyle = "rgba(17, 24, 39, 0.85)";
            context.fillRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);

            context.strokeStyle = "rgba(255, 255, 255, 0.25)";
            context.lineWidth = 1;
            context.strokeRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);

            context.fillStyle = "#ffffff";
            context.textAlign = "left";
            context.textBaseline = "top";

            tooltipLines.forEach((line, idx) => {
                context.fillText(
                    line,
                    tooltipX + horizontalPadding,
                    tooltipY + verticalPadding / 2 + idx * lineHeight
                );
            });
        }

        context.restore();
    }
}
