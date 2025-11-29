"use client";

import { useRef, useEffect, useMemo } from "react";

type SeriesMeta = {
  label: string;
  color?: string;
};

// data format is an array where:
// -first element is an array of x-axis timestamps
// - subsequent elements are arrays of y-axis values for each series
interface AlignedData {
  timestamps: number[];
  series: Array<(number | string | null)[]>;
};

type ChunkStats = {
  min: number[];
  max: number[];
};

type PreparedChartData = {
  timestamps: number[];
  seriesData: Array<(number | string | null)[]>;
  chunkSize: number;
  chunkStats: ChunkStats[];
  enumSeriesIndices: number[]; // indices of series that are enums
  numericalSeriesIndices: number[]; // indices of series that are numerical
  uniqueEnumValues: Record<number, string[]>; // map of series index to unique enum values
};

// first index where timestamp >= targetTime
function binarySearchForFirstVisibleIndex(
  timestamps: number[],
  targetTime: number
): number {
  let left = 0;
  let right = timestamps.length - 1;
  let result = 0;

  while (left <= right) {
    const mid = Math.floor((left + right) / 2);
    if (timestamps[mid] >= targetTime) {
      result = mid;
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  return result;
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

export default function CanvasChart({
  data, series, height, panOffset = 0, scrollProgress = 1, zoomLevel = 100, frozenTimeWindow = null,
  downsampleThreshold = 100000, timeTickCount = 6, hoverTimestamp: externalHoverTimestamp,
  onHoverTimestampChange, domainStart, domainEnd,
}: {
  data: AlignedData;
  series: SeriesMeta[];
  height: number;
  panOffset?: number;
  scrollProgress?: number;
  zoomLevel?: number;
  frozenTimeWindow?: { startTime: number; endTime: number } | null;
  downsampleThreshold?: number;
  timeTickCount?: number;
  hoverTimestamp?: number | null;
  onHoverTimestampChange?: (timestamp: number | null) => void;
  domainStart?: number;
  domainEnd?: number;
}) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const animationFrameId = useRef<number | null>(null);
  const hoverPixelRef = useRef<{ x: number; y: number } | null>(null);
  const tooltipBufferRef = useRef<string[]>([]);
  const timeFormatterRef = useRef<Intl.DateTimeFormat | null>(null);
  const dateFormatterRef = useRef<Intl.DateTimeFormat | null>(null);
  const layoutRef = useRef<{
    minTime: number;
    timeRange: number;
    chartWidth: number;
    paddingLeft: number;
  } | null>(null);

  const preparedData = useMemo<PreparedChartData>(() => {
    if (!data || data.length === 0) {
      return {
        timestamps: [],
        seriesData: [],
        chunkSize: 0,
        chunkStats: [],
        enumSeriesIndices: [],
        numericalSeriesIndices: [],
        uniqueEnumValues: {},
      };
    }

    const [originalTimestamps, ...originalSeries] = data;
    const timestamps = originalTimestamps ?? [];

    if (!timestamps || timestamps.length === 0) {
      return {
        timestamps: [],
        seriesData: [],
        chunkSize: 0,
        chunkStats: [],
        enumSeriesIndices: [],
        numericalSeriesIndices: [],
        uniqueEnumValues: {},
      };
    }

    let workingTimestamps: number[] = timestamps;
    let workingSeries: Array<(number | string | null)[]> = originalSeries;

    const enumSeriesIndices: number[] = [];
    const numericalSeriesIndices: number[] = [];
    const uniqueEnumValues: Record<number, Set<string>> = {};

    workingSeries.forEach((s, idx) => {
      let isEnum = false;
      for (const val of s) {
        if (val !== null && val !== undefined) {
          if (typeof val === "string") {
            isEnum = true;
          }
          break;
        }
      }

      if (isEnum) {
        enumSeriesIndices.push(idx);
        uniqueEnumValues[idx] = new Set<string>();
        s.forEach((val) => {
          if (typeof val === "string") {
            uniqueEnumValues[idx].add(val);
          }
        });
      } else {
        numericalSeriesIndices.push(idx);
      }
    });

    if (downsampleThreshold && workingTimestamps.length > downsampleThreshold) {
      const newTimestamps: number[] = [];
      const newSeriesData = workingSeries.map(
        () => [] as (number | string | null)[]
      );
      const step = workingTimestamps.length / downsampleThreshold;

      for (let i = 0; i < downsampleThreshold; i++) {
        const index = Math.floor(i * step);
        newTimestamps.push(workingTimestamps[index]);
        workingSeries.forEach((seriesPoints, seriesIndex) => {
          newSeriesData[seriesIndex].push(seriesPoints[index]);
        });
      }

      if (
        newTimestamps.length > 0 &&
        workingTimestamps.length > 0 &&
        newTimestamps[newTimestamps.length - 1] !==
        workingTimestamps[workingTimestamps.length - 1]
      ) {
        newTimestamps.push(workingTimestamps[workingTimestamps.length - 1]);
        workingSeries.forEach((seriesPoints, seriesIndex) => {
          const source = seriesPoints[seriesPoints.length - 1];
          newSeriesData[seriesIndex].push(source);
        });
      }

      workingTimestamps = newTimestamps;
      workingSeries = newSeriesData;
    }

    const chunkSize =
      workingTimestamps.length > 0
        ? Math.max(32, Math.ceil(workingTimestamps.length / 512))
        : 0;
    const chunkStats: ChunkStats[] = workingSeries.map(() => ({
      min: [],
      max: [],
    }));

    if (chunkSize > 0) {
      const chunkCount = Math.ceil(workingTimestamps.length / chunkSize);
      workingSeries.forEach((seriesPoints, seriesIndex) => {
        if (enumSeriesIndices.includes(seriesIndex)) return;

        const stats = chunkStats[seriesIndex];
        for (let chunkIdx = 0; chunkIdx < chunkCount; chunkIdx++) {
          const start = chunkIdx * chunkSize;
          const end = Math.min(start + chunkSize, workingTimestamps.length);
          let chunkMin = Infinity;
          let chunkMax = -Infinity;

          for (let i = start; i < end; i++) {
            const value = seriesPoints[i];
            if (
              value === null ||
              value === undefined ||
              typeof value === "string"
            )
              continue;
            if (value < chunkMin) chunkMin = value;
            if (value > chunkMax) chunkMax = value;
          }

          stats.min[chunkIdx] = chunkMin;
          stats.max[chunkIdx] = chunkMax;
        }
      });
    }

    const uniqueEnumValuesRecord: Record<number, string[]> = {};
    Object.keys(uniqueEnumValues).forEach((k) => {
      const key = Number(k);
      uniqueEnumValuesRecord[key] = Array.from(uniqueEnumValues[key]).sort();
    });

    return {
      timestamps: workingTimestamps,
      seriesData: workingSeries,
      chunkSize,
      chunkStats,
      enumSeriesIndices,
      numericalSeriesIndices,
      uniqueEnumValues: uniqueEnumValuesRecord,
    };
  }, [data, downsampleThreshold]);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const context = canvas.getContext("2d");
    if (!context) return;

    if (!timeFormatterRef.current) {
      timeFormatterRef.current = new Intl.DateTimeFormat(undefined, {
        hour: "numeric",
        minute: "2-digit",
        second: "2-digit",
        hour12: true,
      });
    }

    if (!dateFormatterRef.current) {
      dateFormatterRef.current = new Intl.DateTimeFormat(undefined, {
        year: "numeric",
        month: "short",
        day: "2-digit",
      });
    }

    const timeFormatter = timeFormatterRef.current;
    const dateFormatter = dateFormatterRef.current;

    const render = () => {
      context.clearRect(0, 0, width, height);

      const {
        timestamps,
        seriesData,
        chunkSize,
        chunkStats,
        enumSeriesIndices,
        numericalSeriesIndices,
        uniqueEnumValues,
      } = preparedData;
      if (!timestamps || timestamps.length === 0) return;

      const clampedTickCount = Math.max(1, Math.floor(timeTickCount));
      const padding = { top: 20, right: 20, bottom: 56, left: 60 };

      const ENUM_STRIP_HEIGHT = 20;
      const ENUM_STRIP_GAP = 10;
      const LEGEND_HEIGHT = enumSeriesIndices.length > 0 ? 30 : 0;
      const enumSectionHeight =
        enumSeriesIndices.length > 0
          ? enumSeriesIndices.length * (ENUM_STRIP_HEIGHT + ENUM_STRIP_GAP) +
          LEGEND_HEIGHT +
          10
          : 0;

      const numericalTop = padding.top + enumSectionHeight;
      const chartWidth = width - padding.left - padding.right;
      const chartHeight = Math.max(0, height - numericalTop - padding.bottom);

      const latestTime =
        domainEnd !== undefined ? domainEnd : timestamps[timestamps.length - 1];
      const earliestTime =
        domainStart !== undefined ? domainStart : timestamps[0];
      const totalTimeRange = latestTime - earliestTime;

      let visibleStartTime: number;
      let visibleEndTime: number;

      if (frozenTimeWindow) {
        // use the frozen time window when manually panning
        visibleStartTime = frozenTimeWindow.startTime;
        visibleEndTime = frozenTimeWindow.endTime;
      } else {
        // otherwise, calculate based on zoom and panOffset and auto follow
        // zoomLevel: 100 = show all data, 200 = show half the data (2x zoom), 50 = show double (0.5x zoom) etc.
        const zoomFactor = 100 / zoomLevel;
        const visibleTimeRange = totalTimeRange * zoomFactor;
        const normalizedScroll =
          typeof scrollProgress === "number" && Number.isFinite(scrollProgress)
            ? Math.min(Math.max(scrollProgress, 0), 1)
            : null;

        if (normalizedScroll !== null) {
          const maxStartOffset = Math.max(totalTimeRange - visibleTimeRange, 0);
          const startOffset = maxStartOffset * normalizedScroll;
          visibleStartTime = earliestTime + startOffset;
          visibleEndTime = visibleStartTime + visibleTimeRange;
        } else {
          // panOffset is in pixels from the right edge (latest data)
          const timePerPixel = visibleTimeRange / chartWidth;
          const timeOffset = panOffset * timePerPixel;

          visibleEndTime = latestTime - timeOffset;
          visibleStartTime = visibleEndTime - visibleTimeRange;
        }
      }

      if (!frozenTimeWindow) {
        if (visibleStartTime < earliestTime) {
          const delta = earliestTime - visibleStartTime;
          visibleStartTime = earliestTime;
          visibleEndTime += delta;
        }
        if (visibleEndTime > latestTime) {
          const delta = visibleEndTime - latestTime;
          visibleEndTime = latestTime;
          visibleStartTime -= delta;
        }
        if (visibleStartTime < earliestTime) {
          visibleStartTime = earliestTime;
        }
      }

      const startIndex = binarySearchForFirstVisibleIndex(
        timestamps,
        visibleStartTime
      );

      const endIndex = binarySearchForLastVisibleIndex(
        timestamps,
        visibleEndTime
      );

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
          const color =
            val === "N/A" ? NA_COLOR : ENUM_COLORS[i % ENUM_COLORS.length];

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
            const color =
              value === "N/A"
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
      let minValue = Infinity;
      let maxValue = -Infinity;

      if (hasNumerical) {
        const computeVisibleRange = (
          dataPoints: (number | string | null)[],
          stats?: ChunkStats
        ) => {
          if (startIndex > endIndex) return { min: Infinity, max: -Infinity };

          const windowSize = endIndex - startIndex + 1;
          const shouldUseChunks =
            chunkSize > 0 && stats && windowSize > chunkSize * 2;

          let localMin = Infinity;
          let localMax = -Infinity;

          if (!shouldUseChunks) {
            for (let i = startIndex; i <= endIndex; i++) {
              const value = dataPoints[i];
              if (
                value === null ||
                value === undefined ||
                typeof value === "string"
              )
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
              if (
                value === null ||
                value === undefined ||
                typeof value === "string"
              )
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

        numericalSeriesIndices.forEach((seriesIndex) => {
          const dataPoints = seriesData[seriesIndex];
          const stats = chunkStats[seriesIndex];
          const { min, max } = computeVisibleRange(dataPoints, stats);
          if (!Number.isFinite(min) || !Number.isFinite(max)) {
            return;
          }
          minValue = Math.min(minValue, min);
          maxValue = Math.max(maxValue, max);
        });

        // edge case where all values are the same
        if (minValue === maxValue) {
          minValue -= 1;
          maxValue += 1;
        }

        const valueToY = (value: number) => {
          return (
            numericalTop +
            chartHeight -
            ((value - minValue) / (maxValue - minValue)) * chartHeight
          );
        };

        context.strokeStyle = "#333";
        context.lineWidth = 1;
        context.beginPath();

        // y-axis
        context.moveTo(padding.left, numericalTop);
        context.lineTo(padding.left, numericalTop + chartHeight);

        // x-axis
        context.lineTo(width - padding.right, numericalTop + chartHeight);
        context.stroke();

        // grid lines
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
        context.fillStyle = "#000000";
        context.font = "12px sans-serif";
        context.textAlign = "right";
        context.textBaseline = "middle";

        // y-axis labels
        for (let i = 0; i <= numGridLines; i++) {
          const value = maxValue - ((maxValue - minValue) / numGridLines) * i;
          const y = numericalTop + (chartHeight / numGridLines) * i;
          context.fillText(value.toFixed(2), padding.left - 5, y);
        }

        // draw data series
        numericalSeriesIndices.forEach((seriesIndex) => {
          const dataPoints = seriesData[seriesIndex];
          const meta = series[seriesIndex];
          context.strokeStyle = meta?.color || "#000";
          context.lineWidth = 2;
          context.beginPath();

          let pathStarted = false;

          // only iterate through visible data points
          for (let i = startIndex; i <= endIndex; i++) {
            const time = timestamps[i];
            const value = dataPoints[i];

            if (value === null || typeof value !== "number") {
              pathStarted = false;
              continue;
            }

            const x = timeToX(time);
            const y = valueToY(value);

            if (!pathStarted) {
              context.moveTo(x, y);
              pathStarted = true;
            } else {
              context.lineTo(x, y);
            }
          }

          context.stroke();
        });
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
      let activeHoverY: number | null = null;
      let activeHoverTimestamp: number | null = null;

      const hover = hoverPixelRef.current;

      if (hover) {
        const withinX =
          hover.x >= padding.left && hover.x <= width - padding.right;
        const withinY =
          hover.y >= padding.top && hover.y <= height - padding.bottom;

        if (withinX && withinY) {
          activeHoverX = hover.x;
          activeHoverY = hover.y;

          const calculatedTime =
            minTime + ((hover.x - padding.left) / chartWidth) * timeRange;
          activeHoverTimestamp = calculatedTime;
        }
      } else if (
        externalHoverTimestamp !== null &&
        externalHoverTimestamp !== undefined
      ) {
        if (
          externalHoverTimestamp >= minTime &&
          externalHoverTimestamp <= maxTime
        ) {
          activeHoverTimestamp = externalHoverTimestamp;
          activeHoverX = timeToX(externalHoverTimestamp);
          activeHoverY = padding.top + 50;
        }
      }

      if (activeHoverX !== null && activeHoverTimestamp !== null) {
        const hoverX = activeHoverX;
        const hoverTime = activeHoverTimestamp;

        const clampIndex = (idx: number) =>
          Math.min(Math.max(idx, startIndex), endIndex);

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
        if (
          Math.abs(timestamps[leftCandidate] - hoverTime) <
          Math.abs(timestamps[nearestIndex] - hoverTime)
        ) {
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
        const allEnumValues = new Set<string>();
        enumSeriesIndices.forEach((idx) => {
          uniqueEnumValues[idx]?.forEach((v) => allEnumValues.add(v));
        });
        const sortedAllEnums = Array.from(allEnumValues).sort();

        let firstPointY: number | null = null;

        seriesData.forEach((points, idx) => {
          const value = points[nearestIndex];
          if (value === null || value === undefined) return;

          if (
            typeof value === "number" &&
            hasNumerical &&
            numericalSeriesIndices.includes(idx)
          ) {
            let y = 0;
            if (
              numericalSeriesIndices.includes(idx) &&
              Number.isFinite(minValue) &&
              Number.isFinite(maxValue)
            ) {
              y =
                numericalTop +
                chartHeight -
                ((value - minValue) / (maxValue - minValue)) * chartHeight;

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
          const tooltipHeight =
            tooltipLines.length * lineHeight + verticalPadding;

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

      animationFrameId.current = requestAnimationFrame(render);
    };

    const scheduleRender = () => {
      if (animationFrameId.current !== null) return;
      animationFrameId.current = requestAnimationFrame(() => {
        animationFrameId.current = null;
        render();
      });
    };

    const handleWheel = (event: WheelEvent) => {
      if (Math.abs(event.deltaX) > Math.abs(event.deltaY)) {
        event.preventDefault();
      }

      if (event.ctrlKey && onZoomChange) {
        event.preventDefault();
        const sensitivity = 0.005;
        const delta = -event.deltaY * sensitivity * zoomLevel;
        const newZoom = zoomLevel + delta;
        onZoomChange(newZoom);
      }
    };

    const handleMouseMove = (event: MouseEvent) => {
      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      const y = event.clientY - rect.top;
      hoverPixelRef.current = { x, y };

      if (layoutRef.current && onHoverTimestampChange) {
        const { minTime, timeRange, chartWidth, paddingLeft } =
          layoutRef.current;
        if (chartWidth > 0) {
          const calculatedTime =
            minTime + ((x - paddingLeft) / chartWidth) * timeRange;
          onHoverTimestampChange(calculatedTime);
        }
      }
      scheduleRender();
    };

    const handleMouseLeave = () => {
      hoverPixelRef.current = null;
      if (onHoverTimestampChange) {
        onHoverTimestampChange(null);
      }
      scheduleRender();
    };

    render();

    canvas.addEventListener("mousemove", handleMouseMove);
    canvas.addEventListener("mouseleave", handleMouseLeave);
    canvas.addEventListener("wheel", handleWheel, { passive: false });

    return () => {
      if (animationFrameId.current) {
        cancelAnimationFrame(animationFrameId.current);
        animationFrameId.current = null;
      }
      canvas.removeEventListener("mousemove", handleMouseMove);
      canvas.removeEventListener("mouseleave", handleMouseLeave);
      canvas.removeEventListener("wheel", handleWheel);
      hoverPixelRef.current = null;
    };
  }, [
    preparedData,
    series,
    width,
    height,
    panOffset,
    scrollProgress,
    zoomLevel,
    frozenTimeWindow,
    timeTickCount,
    onZoomChange,
    externalHoverTimestamp,
    onHoverTimestampChange,
    domainStart,
    domainEnd,
  ]);

  return (
    <canvas
      ref={canvasRef}
      width={width}
      height={height}
      style={{ display: "block" }}
    />
  );
}
