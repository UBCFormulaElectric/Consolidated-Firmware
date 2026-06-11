"use client";

import { useAlertStore } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useEffect, useRef } from "react";
import { LODAwareAlertSeries } from "./CanvasChartTypes";
import { useSyncedGraph } from "../SyncedGraphContainer";
import { render_hover_line, selectLOD } from "./render";

const MAX_SLIP_STREAM_LANES = 5;
const SLIP_STREAM_TOP_PADDING_LANES = 1;

const SLIP_STREAM_GAP = 5;
const SLIP_STREAM_LANE_HEIGHT = 32;
const SLIP_STREAM_ROUNDING_RADIUS = 8;
const SLIP_STREAM_ROUNDING_SPEED = 0.5;

const ALERT_COLOR = "#0a5efa";

const LABEL_FONT = "11px sans-serif";
const LABEL_PADDING = 8;

const TOOLTIP_FONT = "11px sans-serif";

const TOOLTIP_PADDING_X = 8;
const TOOLTIP_PADDING_Y = 4;

const TOOLTIP_TRIANGLE_SIZE = 5;

const TOOLTIP_GAP = 2;

const TOOLTIP_COLOR = "#000000ac";
const TOOLTIP_TEXT_COLOR = "white";
const TOOLTIP_BORDER_COLOR = "black";

const containerHeightForLanes = (lanes: number) =>
  (lanes + SLIP_STREAM_TOP_PADDING_LANES) * SLIP_STREAM_LANE_HEIGHT +
  (lanes + SLIP_STREAM_TOP_PADDING_LANES - 1) * SLIP_STREAM_GAP;

type HoverInfo = {
  alertIndex: number;
} | null;

type MousePosition = {
  x: number;
  y: number;
} | null;

type AlertBar = {
  signal: string;
  lane: number;
  startTime: number;
  endTime: number;
};

type AlertModel = {
  bars: AlertBar[];
  laneCount: number;
};

const computeAlertModel = (
  alertSeries: { [signalName: string]: LODAwareAlertSeries },
  leftEdge: number,
  rightEdge: number,
  width: number,
): AlertModel => {
  const bars: AlertBar[] = [];

  Object.keys(alertSeries).forEach((name) => {
    const series = alertSeries[name];
    if (series.lods.length === 0) return;

    const lod = series.lods[selectLOD(series, leftEdge, rightEdge, width)];
    const { data, timestamps } = lod;

    let openStart: number | null = null;
    let lastActiveTime = 0;

    for (let i = 0; i < timestamps.length; i++) {
      const isActive = data[i] >= 0.5;
      const timestamp = timestamps[i];

      if (isActive) {
        if (openStart === null) openStart = timestamp;
        lastActiveTime = timestamp;
      } else if (openStart !== null) {
        bars.push({ signal: name, lane: 0, startTime: openStart, endTime: timestamp });
        openStart = null;
      }
    }

    if (openStart === null) return;

    bars.push({ signal: name, lane: 0, startTime: openStart, endTime: lastActiveTime });
  });

  bars.sort((a, b) => a.startTime - b.startTime);

  const laneEndTimes: number[] = [];
  bars.forEach((bar) => {
    let lane = laneEndTimes.findIndex((endTime) => endTime <= bar.startTime);
    if (lane === -1) {
      lane = laneEndTimes.length;
      laneEndTimes.push(bar.endTime);
    } else {
      laneEndTimes[lane] = bar.endTime;
    }
    bar.lane = lane;
  });

  return { bars, laneCount: laneEndTimes.length };
};

const modelSignature = (
  alertSeries: { [signalName: string]: LODAwareAlertSeries },
  leftEdge: number,
  rightEdge: number,
  width: number,
): string => {
  const parts: string[] = [];
  Object.keys(alertSeries)
    .sort()
    .forEach((name) => {
      const series = alertSeries[name];
      if (series.lods.length === 0) return;
      const lodIndex = selectLOD(series, leftEdge, rightEdge, width);
      parts.push(`${name}:${lodIndex}:${series.lods[lodIndex].timestamps.length}`);
    });
  return parts.join("|");
};

const hitTestAlerts = (
  mousePos: MousePosition,
  bars: AlertBar[],
  width: number,
  leftEdge: number,
  rightEdge: number,
  ctx: CanvasRenderingContext2D,
): HoverInfo => {
  if (!mousePos) return null;

  const { x: mouseX, y: mouseY } = mousePos;
  const laneGap = SLIP_STREAM_GAP;
  const heightPerLane = SLIP_STREAM_LANE_HEIGHT;
  const pixelsPerMs = width / (rightEdge - leftEdge);

  for (let i = bars.length - 1; i >= 0; i--) {
    const bar = bars[i];
    if (bar.endTime < leftEdge || bar.startTime > rightEdge) continue;

    const laneY = (bar.lane + SLIP_STREAM_TOP_PADDING_LANES) * (heightPerLane + laneGap);
    if (mouseY < laneY || mouseY > laneY + heightPerLane) continue;

    const barStartX = bar.startTime < leftEdge ? 0 : (bar.startTime - leftEdge) * pixelsPerMs;
    const barEndX = (bar.endTime - leftEdge) * pixelsPerMs;
    if (mouseX < barStartX || mouseX > barEndX) continue;

    const visibleStartX = Math.max(barStartX, 0);
    const visibleEndX = Math.min(barEndX, width);
    const visibleWidth = visibleEndX - visibleStartX;

    ctx.font = LABEL_FONT;
    const textWidth = ctx.measureText(bar.signal).width;

    if (visibleWidth < textWidth + LABEL_PADDING * 2) return { alertIndex: i };
    return null;
  }

  return null;
};

const renderAlertTimeline = (
  ctx: CanvasRenderingContext2D,
  bars: AlertBar[],
  width: number,
  height: number,
  leftEdge: number,
  rightEdge: number,
  bottom: number,
  hover: HoverInfo,
) => {
  ctx.clearRect(0, 0, width, height);

  const laneGap = SLIP_STREAM_GAP;
  const heightPerLane = SLIP_STREAM_LANE_HEIGHT;

  const pixelsPerMillisecond = width / (rightEdge - leftEdge);

  bars.forEach((bar) => {
    if (bar.endTime < leftEdge || bar.startTime > rightEdge) return;

    const laneY = (bar.lane + SLIP_STREAM_TOP_PADDING_LANES) * (heightPerLane + laneGap);

    const isStartBeforeView = bar.startTime < leftEdge;

    const barStartX = isStartBeforeView ? 0 : (bar.startTime - leftEdge) * pixelsPerMillisecond;
    const barEndX = (bar.endTime - leftEdge) * pixelsPerMillisecond;

    ctx.fillStyle = ALERT_COLOR;

    // NOTE(evan): The rounding instantly going away jumps out at ur eyes so
    //             smoothe it it doesn't look good if u stare at it but because it
    //             doesn't jump out you shouldn't see it most of the time.
    const distanceFromRightEdgePx = (rightEdge - bar.endTime) * pixelsPerMillisecond;
    const rightRadius = Math.min(Math.max(distanceFromRightEdgePx * SLIP_STREAM_ROUNDING_SPEED, 0), SLIP_STREAM_ROUNDING_RADIUS);

    ctx.beginPath();
    ctx.roundRect(barStartX, laneY, barEndX - barStartX, heightPerLane, [
      isStartBeforeView ? 0 : SLIP_STREAM_ROUNDING_RADIUS,
      rightRadius,
      rightRadius,
      isStartBeforeView ? 0 : SLIP_STREAM_ROUNDING_RADIUS,
    ]);
    ctx.fill();

    const visibleStartX = Math.max(barStartX, 0);
    const visibleEndX = Math.min(barEndX, width);
    const visibleWidth = visibleEndX - visibleStartX;

    ctx.font = LABEL_FONT;
    const textWidth = ctx.measureText(bar.signal).width;

    if (visibleWidth < textWidth + LABEL_PADDING * 2) return;

    ctx.save();
    ctx.beginPath();
    ctx.rect(visibleStartX, laneY, visibleWidth, heightPerLane);
    ctx.clip();

    ctx.fillStyle = "#ffffff";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillText(bar.signal, visibleStartX + visibleWidth / 2, laneY + heightPerLane / 2);
    ctx.restore();
  });

  if (!hover || hover.alertIndex >= bars.length) return;
  const bar = bars[hover.alertIndex];

  const laneY = (bar.lane + SLIP_STREAM_TOP_PADDING_LANES) * (heightPerLane + laneGap);
  const barStartX = bar.startTime < leftEdge ? 0 : (bar.startTime - leftEdge) * pixelsPerMillisecond;
  const barEndX = (bar.endTime - leftEdge) * pixelsPerMillisecond;
  const visibleStartX = Math.max(barStartX, 0);
  const visibleEndX = Math.min(barEndX, width);
  const visibleWidth = visibleEndX - visibleStartX;
  const centerX = visibleStartX + visibleWidth / 2;
  const barBottom = laneY + heightPerLane;

  ctx.font = TOOLTIP_FONT;
  const textWidth = ctx.measureText(bar.signal).width;
  const tooltipWidth = textWidth + TOOLTIP_PADDING_X * 2;
  const tooltipHeight = 14 + TOOLTIP_PADDING_Y * 2;

  const isAboveTooltip = laneY + heightPerLane + tooltipHeight + TOOLTIP_TRIANGLE_SIZE + TOOLTIP_GAP > bottom;

  const triangleTop = isAboveTooltip ? laneY - TOOLTIP_GAP : barBottom + TOOLTIP_GAP;
  const tooltipTop = isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE;

  ctx.strokeStyle = TOOLTIP_BORDER_COLOR;
  ctx.lineWidth = 1;
  ctx.fillStyle = TOOLTIP_COLOR;
  ctx.beginPath();
  ctx.moveTo(centerX - TOOLTIP_TRIANGLE_SIZE, isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE);
  ctx.lineTo(centerX, triangleTop);
  ctx.lineTo(centerX + TOOLTIP_TRIANGLE_SIZE, isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE);
  ctx.stroke();
  ctx.closePath();
  ctx.fill();

  const tooltipX = Math.max(0, Math.min(centerX - tooltipWidth / 2, width - tooltipWidth));
  ctx.beginPath();
  ctx.roundRect(tooltipX, tooltipTop + (isAboveTooltip ? -tooltipHeight : 0), tooltipWidth, tooltipHeight, 4);
  ctx.fill();
  ctx.stroke();

  ctx.fillStyle = TOOLTIP_TEXT_COLOR;
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  ctx.fillText(bar.signal, tooltipX + tooltipWidth / 2, tooltipTop + (isAboveTooltip ? -tooltipHeight : tooltipHeight) / 2);
};

function AlertTimeline() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const scrollableRef = useRef<HTMLDivElement>(null);
  const wrapperRef = useRef<HTMLDivElement>(null);
  const animationFrame = useRef<number | null>(null);

  const {
    globalTimeRangeRef,
    XToTime,
    timeToX,
    hoverXRef,
  } = useSyncedGraph();

  const XToTimeRef = useRef(XToTime);
  XToTimeRef.current = XToTime;
  const timeToXRef = useRef(timeToX);
  timeToXRef.current = timeToX;

  const alertModel = useRef<AlertModel>({ bars: [], laneCount: 0 });
  const lastSignature = useRef<string | null>(null);
  const hoverInfo = useRef<HoverInfo>(null);
  const mousePos = useRef<MousePosition>(null);

  const storeRef = useAlertStore();

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    const dpr = window.devicePixelRatio || 1;

    const renderSlipStream = () => {
      const store = storeRef.current;
      if (!globalTimeRangeRef.current || !store) {
        animationFrame.current = requestAnimationFrame(renderSlipStream);
        return;
      }

      const rect = canvas.getBoundingClientRect();
      const targetWidth = Math.max(1, Math.floor(rect.width * dpr));
      const targetHeight = Math.max(1, Math.floor(rect.height * dpr));

      if (canvas.width !== targetWidth || canvas.height !== targetHeight) {
        canvas.width = targetWidth;
        canvas.height = targetHeight;
      }

      ctx.setTransform(1, 0, 0, 1, 0, 0);
      ctx.scale(dpr, dpr);

      const leftEdge = XToTimeRef.current(0);
      const rightEdge = XToTimeRef.current(rect.width);

      const alertSeries = store.getAlertSeries();
      const signature = modelSignature(alertSeries, leftEdge, rightEdge, rect.width);
      if (signature !== lastSignature.current) {
        alertModel.current = computeAlertModel(alertSeries, leftEdge, rightEdge, rect.width);
        lastSignature.current = signature;

        if (scrollableRef.current) {
          scrollableRef.current.style.height = `${containerHeightForLanes(Math.max(alertModel.current.laneCount, 1))}px`;
        }
      }

      const bars = alertModel.current.bars;

      hoverInfo.current = hitTestAlerts(mousePos.current, bars, rect.width, leftEdge, rightEdge, ctx);

      const wrapperClientRect = wrapperRef.current?.getBoundingClientRect();
      const top = wrapperRef.current?.scrollTop ?? 0;
      const bottom = top + (wrapperClientRect?.height ?? 0);

      renderAlertTimeline(ctx, bars, rect.width, rect.height, leftEdge, rightEdge, bottom, hoverInfo.current);

      if (hoverXRef.current !== null) {
        render_hover_line(
          ctx,
          rect.width,
          rect.height,
          XToTimeRef.current(hoverXRef.current),
          timeToXRef.current,
          false,
        );
      }

      animationFrame.current = requestAnimationFrame(renderSlipStream);
    };

    animationFrame.current = requestAnimationFrame(renderSlipStream);

    return () => {
      if (animationFrame.current) {
        cancelAnimationFrame(animationFrame.current);
      }
    };
  }, [canvasRef.current]);

  useEffect(() => {
    const wrapper = wrapperRef.current;
    if (!wrapper) return;

    const findScrollableAncestor = (el: HTMLElement | null): HTMLElement | Window => {
      let node = el?.parentElement ?? null;

      while (node) {
        if (node === document.body) break;

        const style = getComputedStyle(node);
        const overflowY = style.overflowY;

        const canScroll = (overflowY === "auto" || overflowY === "scroll" || overflowY === "overlay") && node.scrollHeight > node.clientHeight;

        if (canScroll) return node;

        node = node.parentElement;
      }
      return window;
    };

    const handleWheel = (e: WheelEvent) => {
      const deltaY = e.deltaY;
      if (deltaY === 0) return;

      const atTop = wrapper.scrollTop <= 0;
      const atBottom = wrapper.scrollTop + wrapper.clientHeight >= wrapper.scrollHeight - 1;

      const wouldChain = (deltaY < 0 && atTop) || (deltaY > 0 && atBottom);
      if (!wouldChain) return;

      e.preventDefault();

      const ancestor = findScrollableAncestor(wrapper);
      ancestor.scrollBy({ top: deltaY });
    };

    wrapper.addEventListener("wheel", handleWheel, { passive: false });

    return () => {
      wrapper.removeEventListener("wheel", handleWheel);
    };
  }, []);

  const handleMouseMove = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) {
      mousePos.current = null;
      return;
    }

    const rect = canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    mousePos.current = {
      x,
      y: e.clientY - rect.top,
    };
    hoverXRef.current = x;
  };

  const handleMouseLeave = () => {
    mousePos.current = null;
    hoverXRef.current = null;
  };

  return (
    <div className="flex flex-col gap-5">
      <div className="overflow-y-scroll scrollbar-hidden overscroll-none"
        ref={wrapperRef}
        style={{
          maxHeight: `${containerHeightForLanes(MAX_SLIP_STREAM_LANES)}px`,
        }}
      >
        <div
          className="relative flex w-full"
          ref={scrollableRef}
          style={{
            height: `${containerHeightForLanes(MAX_SLIP_STREAM_LANES)}px`,
          }}
        >
          <canvas
            className="w-full h-full"
            ref={canvasRef}
            onMouseMove={handleMouseMove}
            onMouseLeave={handleMouseLeave}
          ></canvas>
        </div>
      </div>
    </div>
  );
}

export default AlertTimeline;
