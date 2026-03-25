"use client";

import { useAlertDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { RefObject, useEffect, useRef } from "react";
import { AlertSeries } from "./CanvasChartTypes";
import { useSyncedGraph } from "../SyncedGraphContainer";

const INITIAL_SLIP_STREAM_LANES = 5;
const MAX_SLIP_STREAM_LANES = 5;
const SLIP_STREAM_GAP = 5;
const PROCESSING_INTERVAL_MS = 1;
const SLIP_STREAM_ROUNDING_RADIUS = 8;
const SLIP_STREAM_ROUNDING_SPEED = 0.5;
const LABEL_FONT = "11px sans-serif";
const LABEL_PADDING = 8;
const TOOLTIP_FONT = "11px sans-serif";
const TOOLTIP_PADDING_X = 8;
const TOOLTIP_PADDING_Y = 4;
const TOOLTIP_TRIANGLE_SIZE = 5;
const TOOLTIP_GAP = 2;

type HoverInfo = {
  alertIndex: number;
} | null;

type MousePosition = {
  x: number;
  y: number;
} | null;

type CursorIndices = {
  [signal: string]: {
    /**
     * Index of the next data point to process for this signal in the `timePoints` array.
     */
    cursorIndex: number;
    /**
     * Index into `AlertTimelineRenderState` of the last rendered alert for this signal. If it is null,
     * no alert was rendered on the right edge of the processed data for this signal.
     */
    renderIndex: number | null;
  }
}

type AlertTimelineRenderState = {
  signal: string;
  streamIndex: number;
  startTime: number;
  endTime?: number;
  color?: string;
}[];

const processAlertTimelineData = (
  data: { [signalName: string]: AlertSeries },
  cursorIndices: CursorIndices,
  previousState: AlertTimelineRenderState,
  populationMap: boolean[],
  slipStreamLanes: RefObject<number>,
) => {
  Object.keys(data).forEach((signal) => {
    const cursor = cursorIndices[signal] || { cursorIndex: 0, renderIndex: null };
    const signalData = data[signal];

    while (cursor.cursorIndex < signalData.data.length) {
      const timestamp = signalData.timestamps[cursor.cursorIndex];
      const isAlertActive = signalData.data[cursor.cursorIndex] === 1;

      // If the alert turned on and we haven't already rendered it, create a new alert entry
      if (isAlertActive && cursor.renderIndex === null) {
        const newAlertIndex = previousState.length;
        let streamIndex = populationMap.findIndex((populated) => !populated);

        if (streamIndex === -1) {
          slipStreamLanes.current += 1;
          streamIndex = slipStreamLanes.current - 1;
          populationMap.push(true);
        } else {
          populationMap[streamIndex] = true;
        }

        previousState.push({
          signal,
          streamIndex: streamIndex === -1 ? 0 : streamIndex,
          startTime: timestamp,
          endTime: undefined,
          color: "#0a5efa",
        });

        populationMap[streamIndex] = true;
        cursor.renderIndex = newAlertIndex;
      }

      // If the alert turns off and there was a previously rendered alert, we need to update its end time
      if (!isAlertActive && cursor.renderIndex !== null) {
        previousState[cursor.renderIndex].endTime = timestamp;

        const streamIndex = previousState[cursor.renderIndex].streamIndex;
        populationMap[streamIndex] = false;

        cursor.renderIndex = null;
      }

      cursor.cursorIndex += 1;
    }

    cursorIndices[signal] = cursor;
  });

  return { previousState, populationMap, cursorIndices };
};

const hitTestAlerts = (
  mousePos: MousePosition,
  state: AlertTimelineRenderState,
  width: number,
  height: number,
  leftEdge: number,
  rightEdge: number,
  liveTime: number,
  slipStreamLanes: number,
  ctx: CanvasRenderingContext2D,
): HoverInfo => {
  if (!mousePos) return null;

  const { x: mouseX, y: mouseY } = mousePos;
  const laneGap = SLIP_STREAM_GAP;
  const heightPerLane = (height - (slipStreamLanes - 1) * laneGap) / slipStreamLanes;
  const pixelsPerMs = width / (rightEdge - leftEdge);

  for (let i = state.length - 1; i >= 0; i--) {
    const alert = state[i];
    if (alert.streamIndex >= slipStreamLanes) continue;
    if (alert.endTime && alert.endTime < leftEdge) continue;

    const laneY = alert.streamIndex * (heightPerLane + laneGap);
    if (mouseY < laneY || mouseY > laneY + heightPerLane) continue;

    const alertStartX = alert.startTime < leftEdge ? 0 : (alert.startTime - leftEdge) * pixelsPerMs;
    const alertEndX = alert.endTime ? (alert.endTime - leftEdge) * pixelsPerMs : (liveTime - leftEdge) * pixelsPerMs;
    if (mouseX < alertStartX || mouseX > alertEndX) continue;

    const visibleStartX = Math.max(alertStartX, 0);
    const visibleEndX = Math.min(alertEndX, width);
    const visibleWidth = visibleEndX - visibleStartX;

    ctx.font = LABEL_FONT;
    const textWidth = ctx.measureText(alert.signal).width;

    if (visibleWidth < textWidth + LABEL_PADDING * 2) return { alertIndex: i };
    return null;
  }

  return null;
};

const renderAlertTimeline = (
  ctx: CanvasRenderingContext2D,
  state: AlertTimelineRenderState,
  width: number,
  height: number,
  leftEdge: number,
  rightEdge: number,
  bottom: number,
  liveTime: number,
  slipStreamLanes: number,
  hover: HoverInfo,
) => {
  ctx.clearRect(0, 0, width, height);

  const laneGap = SLIP_STREAM_GAP;
  const heightPerLane = (height - (slipStreamLanes - 1) * laneGap) / slipStreamLanes;

  const pixelsPerMillisecond = width / (rightEdge - leftEdge);

  state.forEach((alert) => {
    if (alert.streamIndex >= slipStreamLanes) return;
    if (alert.endTime && alert.endTime < leftEdge) return;

    const laneY = alert.streamIndex * (heightPerLane + laneGap);

    const isStartBeforeView = alert.startTime < leftEdge;

    const alertStartX = isStartBeforeView ? 0 : (alert.startTime - leftEdge) * pixelsPerMillisecond;
    const alertEndX = alert.endTime ? (alert.endTime - leftEdge) * pixelsPerMillisecond : (liveTime - leftEdge) * pixelsPerMillisecond;

    ctx.fillStyle = alert.color || "#0a5efa";

    const isOffLeftEdge = alert.startTime < leftEdge;

    const endTime = alert.endTime ?? liveTime;
    
    // NOTE(evan): The rounding instantly going away jumps out at ur eyes so 
    //             smoothe it it doesn't look good if u stare at it but because it
    //             doesn't jump out you shouldn't see it most of the time.
    const distanceFromRightEdgePx = (rightEdge - endTime) * pixelsPerMillisecond;
    const rightRadius = Math.min(Math.max(distanceFromRightEdgePx * SLIP_STREAM_ROUNDING_SPEED, 0), SLIP_STREAM_ROUNDING_RADIUS);

    ctx.beginPath();
    ctx.roundRect(alertStartX, laneY, alertEndX - alertStartX, heightPerLane, [
      isOffLeftEdge ? 0 : SLIP_STREAM_ROUNDING_RADIUS,
      rightRadius,
      rightRadius,
      isOffLeftEdge ? 0 : SLIP_STREAM_ROUNDING_RADIUS,
    ]);
    ctx.fill();

    const visibleStartX = Math.max(alertStartX, 0);
    const visibleEndX = Math.min(alertEndX, width);
    const visibleWidth = visibleEndX - visibleStartX;

    ctx.font = LABEL_FONT;
    const textWidth = ctx.measureText(alert.signal).width;

    if (visibleWidth < textWidth + LABEL_PADDING * 2) return;

    ctx.save();
    ctx.beginPath();
    ctx.rect(visibleStartX, laneY, visibleWidth, heightPerLane);
    ctx.clip();

    ctx.fillStyle = "#ffffff";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillText(
      alert.signal,
      visibleStartX + visibleWidth / 2,
      laneY + heightPerLane / 2,
    );
    ctx.restore();
  });

  if (!hover || hover.alertIndex >= state.length) return;
  const alert = state[hover.alertIndex];
  
  if (alert.streamIndex >= slipStreamLanes) return;

  const laneY = alert.streamIndex * (heightPerLane + laneGap);
  const alertStartX = alert.startTime < leftEdge ? 0 : (alert.startTime - leftEdge) * pixelsPerMillisecond;
  const alertEndX = alert.endTime ? (alert.endTime - leftEdge) * pixelsPerMillisecond : (liveTime - leftEdge) * pixelsPerMillisecond;
  const visibleStartX = Math.max(alertStartX, 0);
  const visibleEndX = Math.min(alertEndX, width);
  const visibleWidth = visibleEndX - visibleStartX;
  const centerX = visibleStartX + visibleWidth / 2;
  const barBottom = laneY + heightPerLane;

  ctx.font = TOOLTIP_FONT;
  const textWidth = ctx.measureText(alert.signal).width;
  const tooltipWidth = textWidth + TOOLTIP_PADDING_X * 2;
  const tooltipHeight = 14 + TOOLTIP_PADDING_Y * 2;
  
  const isAboveTooltip = laneY + heightPerLane + tooltipHeight + TOOLTIP_TRIANGLE_SIZE + TOOLTIP_GAP > bottom;
  
  const triangleTop = isAboveTooltip ? laneY - TOOLTIP_GAP : barBottom + TOOLTIP_GAP;
  const tooltipTop = isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE;

  ctx.fillStyle = "#ffffff";
  ctx.beginPath();
  ctx.moveTo(centerX - TOOLTIP_TRIANGLE_SIZE, isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE);
  ctx.lineTo(centerX, triangleTop);
  ctx.lineTo(centerX + TOOLTIP_TRIANGLE_SIZE, isAboveTooltip ? triangleTop - TOOLTIP_TRIANGLE_SIZE : triangleTop + TOOLTIP_TRIANGLE_SIZE);
  ctx.closePath();
  ctx.fill();

  const tooltipX = Math.max(0, Math.min(centerX - tooltipWidth / 2, width - tooltipWidth));
  ctx.beginPath();
  ctx.roundRect(tooltipX, tooltipTop + (isAboveTooltip ? -tooltipHeight : 0), tooltipWidth, tooltipHeight, 4);
  ctx.fill();

  ctx.fillStyle = "#333333";
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  ctx.fillText(alert.signal, tooltipX + tooltipWidth / 2, tooltipTop + (isAboveTooltip ? -tooltipHeight : tooltipHeight) / 2);
};

function AlertTimeline() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const scrollableRef = useRef<HTMLDivElement>(null);
  const wrapperRef = useRef<HTMLDivElement>(null);
  const animationFrame = useRef<number | null>(null);

  const {
    globalTimeRangeRef,
    scalePxPerSecRef,
    XToTime,
  } = useSyncedGraph();

  const XToTimeRef = useRef(XToTime);
  XToTimeRef.current = XToTime;

  const cursorIndices = useRef<CursorIndices>({});
  const slipStreamLanes = useRef(INITIAL_SLIP_STREAM_LANES);
  const slipstreamPopulated = useRef<boolean[]>(new Array(INITIAL_SLIP_STREAM_LANES).fill(false));
  const renderState = useRef<AlertTimelineRenderState>([]);
  const hoverInfo = useRef<HoverInfo>(null);
  const mousePos = useRef<MousePosition>(null);

  const data = useAlertDataStores();

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    const dpr = window.devicePixelRatio || 1;

    const renderSlipStream = () => {
      if (!globalTimeRangeRef.current) {
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
      const liveTime = globalTimeRangeRef.current!.max;;

      hoverInfo.current = hitTestAlerts(
        mousePos.current,
        renderState.current,
        rect.width,
        rect.height,
        leftEdge,
        rightEdge - 15 / scalePxPerSecRef.current,
        liveTime,
        slipStreamLanes.current,
        ctx,
      );

      const wrapperClientRect = wrapperRef.current?.getBoundingClientRect();
      const top = wrapperRef.current?.scrollTop ?? 0;
      const bottom = top + (wrapperClientRect?.height ?? 0);

      renderAlertTimeline(
        ctx,
        renderState.current,
        rect.width,
        rect.height,
        leftEdge,
        rightEdge - 15 / scalePxPerSecRef.current,
        bottom,
        liveTime,
        slipStreamLanes.current,
        hoverInfo.current,
      );

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
    if (!data.current) return;

    const processRenderData = () => {
      if (!data.current) return;

      const {
        previousState: newRenderState,
        populationMap: newSlipstreamPopulation,
        cursorIndices: newCursorIndices
      } = processAlertTimelineData(
        data.current,
        cursorIndices.current,
        renderState.current,
        slipstreamPopulated.current,
        slipStreamLanes,
      );

      if (scrollableRef.current) {
        const newHeight = slipStreamLanes.current * 32 + (slipStreamLanes.current - 1) * SLIP_STREAM_GAP;

        scrollableRef.current.style.height = `${newHeight}px`;
      }

      renderState.current = newRenderState;
      slipstreamPopulated.current = newSlipstreamPopulation;
      cursorIndices.current = newCursorIndices;
    };

    const intervalId = setInterval(processRenderData, PROCESSING_INTERVAL_MS);

    return () => {
      clearInterval(intervalId);
    };
  }, [data.current]);
  
  const handleMouseMove = (e: React.MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) {
      mousePos.current = null;
      return;
    }

    const rect = canvas.getBoundingClientRect();
    mousePos.current = {
      x: e.clientX - rect.left,
      y: e.clientY - rect.top,
    };
  };

  const handleMouseLeave = () => {
    mousePos.current = null;
  };

  return (
    <div className="flex flex-col gap-5">
      <div className="overflow-y-scroll scrollbar-hidden overscroll-none"
        ref={wrapperRef}
        style={{
          maxHeight: `${MAX_SLIP_STREAM_LANES * 32 + (MAX_SLIP_STREAM_LANES - 1) * SLIP_STREAM_GAP}px`,
        }}
      >
        <div
          className="relative flex w-full"
          ref={scrollableRef}
          style={{
            height: `${INITIAL_SLIP_STREAM_LANES * 32}px`,
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
