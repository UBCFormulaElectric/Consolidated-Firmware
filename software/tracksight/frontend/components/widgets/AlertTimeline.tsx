"use client";

import { WidgetData } from "@/lib/types/Widget";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useEffect, useMemo, useRef } from "react";
import { isAlertSignalMetadata } from "@/lib/types/Signal";
import { SignalStoreEntry } from "@/lib/signals/SignalStore";

const SLIP_STREAM_LANES = 5;
const SLIP_STREAM_GAP = 1;
const PROCESSING_INTERVAL_MS = 1000 / 60;

const FULL_VIEW_DURATION_MS = 20_000 // 2 seconds at no zoom

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
  data: Map<string, SignalStoreEntry>,
  signals: string[],
  cursorIndices: CursorIndices,
  previousState: AlertTimelineRenderState,
  populationMap: boolean[],
  signalColors?: { [signal: string]: string },
) => {
  signals.forEach((signal) => {
    const cursor = cursorIndices[signal] || { cursorIndex: 0, renderIndex: null };
    const signalData = data.get(signal);

    if (!signalData || signalData.error || !signalData.isSubscribed) return;

    while (cursor.cursorIndex < signalData.data.timePoints.length) {
      const timestamp = signalData.data.timePoints.get(cursor.cursorIndex);
      const isAlertActive = signalData.data.values.get(cursor.cursorIndex) === 1;

      // If the alert turned on and we haven't already rendered it, create a new alert entry
      if (isAlertActive && cursor.renderIndex === null) {
        const newAlertIndex = previousState.length;
        const streamIndex = populationMap.findIndex((populated) => !populated);

        previousState.push({
          signal,
          streamIndex: streamIndex === -1 ? 0 : streamIndex,
          startTime: timestamp,
          endTime: timestamp,
          color: signalColors ? signalColors[signal] : undefined,
        });

        populationMap[streamIndex] = true;
        cursor.renderIndex = newAlertIndex;
      } else if (isAlertActive && cursor.renderIndex !== null) {
        previousState[cursor.renderIndex].endTime = timestamp;
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

const renderAlertTimeline = (
  ctx: CanvasRenderingContext2D,
  state: AlertTimelineRenderState,
  width: number,
  height: number,
  rightEdge: number,
  zoom: number,
) => {
  ctx.clearRect(0, 0, width, height);

  const laneGap = SLIP_STREAM_GAP;
  const heightPerLane = (height - (SLIP_STREAM_LANES - 1) * laneGap) / SLIP_STREAM_LANES;

  const pixelsPerMillisecond = width / FULL_VIEW_DURATION_MS;
  const startTime = rightEdge - FULL_VIEW_DURATION_MS;

  state.forEach((alert) => {
    if (alert.streamIndex >= SLIP_STREAM_LANES) return;

    const laneY = alert.streamIndex * (heightPerLane + laneGap);

    const isStartBeforeView = alert.startTime < startTime;

    const alertStartX = isStartBeforeView ? 0 : (alert.startTime - startTime) * pixelsPerMillisecond;
    const alertEndX = alert.endTime ? (alert.endTime - startTime) * pixelsPerMillisecond : width;

    ctx.fillStyle = alert.color || "red";
    ctx.fillRect(alertStartX, laneY, alertEndX - alertStartX, heightPerLane);
  });
};

function AlertTimeline(props: WidgetData<"alertTimeline">) {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const canvasRef = useRef<HTMLCanvasElement>(null);
  const scrollableRef = useRef<HTMLDivElement>(null);
  const animationFrame = useRef<number | null>(null);

  const cursorIndices = useRef<CursorIndices>({});
  const slipstreamPopulated = useRef<boolean[]>(new Array(SLIP_STREAM_LANES).fill(false));
  const renderState = useRef<AlertTimelineRenderState>([]);

  const signalColors = useMemo(() => {
    const colors: { [signal: string]: string } = {};

    signals.forEach((signal, index) => {
      colors[signal] = colorPalette?.[index % colorPalette.length] || "#FF637E";
    });

    return colors;
  }, [signals, colorPalette]);

  const data = useSignalDataStores(signals);

  const signalMetadata = useSignalMetadata(signals[0] ?? "");

  useEffect(() => {
    let isRunning = true;

    const renderSlipStream = () => {
      if (!isRunning) return;

      const canvas = canvasRef.current;

      if (!canvas) {
        animationFrame.current = requestAnimationFrame(renderSlipStream);
        return;
      }

      const dpr = window.devicePixelRatio || 1;
      const rect = canvas.getBoundingClientRect();
      const targetWidth = Math.round(rect.width * dpr);
      const targetHeight = Math.round(rect.height * dpr);

      // Only resize if dimensions changed to avoid clearing the canvas unnecessarily
      if (canvas.width !== targetWidth || canvas.height !== targetHeight) {
        canvas.width = targetWidth;
        canvas.height = targetHeight;
      }

      const ctx = canvas.getContext("2d")!;

      ctx.setTransform(1, 0, 0, 1, 0, 0);
      ctx.scale(dpr, dpr);

      renderAlertTimeline(
        ctx,
        renderState.current,
        window.innerWidth,
        rect.height,
        // TODO(evan): Start time should be adjustable via panning or auto follow
        Date.now() - 50,
        1
      );

      animationFrame.current = requestAnimationFrame(renderSlipStream);
    };

    animationFrame.current = requestAnimationFrame(renderSlipStream);

    return () => {
      isRunning = false;
      if (animationFrame.current) {
        cancelAnimationFrame(animationFrame.current);
      }
    };
  }, [signals, colorPalette]);

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
        signals,
        cursorIndices.current,
        renderState.current,
        slipstreamPopulated.current,
        signalColors
      );

      renderState.current = newRenderState;
      slipstreamPopulated.current = newSlipstreamPopulation;
      cursorIndices.current = newCursorIndices;
    };

    const intervalId = setInterval(processRenderData, PROCESSING_INTERVAL_MS);

    return () => {
      clearInterval(intervalId);
    };
  }, [data.current, signals]);

  if (signalMetadata.isLoading) {
    return (
      <div className="flex flex-col gap-5">
        <div className="px-8">
          Loading...
        </div>

        <div className="relative flex h-6 w-full">
          <div
            className="w-full"
            style={{
              backgroundColor: "lightgray",
            }}
          />
        </div>
      </div>
    );
  }

  if (!signalMetadata.data || signalMetadata.isError) {
    return (
      <div className="flex flex-col items-center">
        Error loading signal metadata.

        <p className="p-4 text-center text-sm">
          Signal metadata not found for signal "{signals[0]}". Please select a valid enum signal.
        </p>
      </div>
    )
  }

  if (!isAlertSignalMetadata(signalMetadata.data)) {
    return (
      <div className="flex flex-col items-center">
        Invalid signal type.
        <p className="p-4 text-center text-sm">
          The signal "{signals[0]}" is not an alert signal. Please select a valid alert signal.
        </p>
      </div>
    );
  }

  return (
    <div className="flex flex-col gap-5">
      <div className="px-8">
        {/* TODO(evan): Allow multiple selection */}
      </div>

      <div
        className="relative flex w-full"
        ref={scrollableRef}
        style={{
          backgroundColor: "lightgray",
          height: `${SLIP_STREAM_LANES * 32}px`,
        }}
      >
        <canvas
          className="w-full"
          ref={canvasRef}
        ></canvas>
      </div>
    </div>
  );
}

export default AlertTimeline;
