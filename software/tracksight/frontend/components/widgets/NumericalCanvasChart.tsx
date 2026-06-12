"use client";

import { useRef } from "react";
import render, { CHART_PADDING, render_empty } from "@/components/widgets/render";
import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useCanvasRenderLoop } from "@/lib/hooks/useCanvasRenderLoop";
import { useCanvasHover } from "@/lib/hooks/useCanvasHover";
import { NumericalGraphWidgetData } from "@/lib/types/Widget";
import { getVisibleTelemetryMarkers } from "@/lib/telemetryMarkers";

export default function NumericalCanvasChart({
  id,
  options,
  signals,
  hoveredSignal,
  onHoverTimestampChange,
}: NumericalGraphWidgetData) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const layoutRef = useRef<ChartLayout | null>(null);
  const { globalTimeRangeRef, hoverXRef, XToTime } = useSyncedGraph();

  const { height, timeTickCount } = options;

  const chartData = useSignalDataStores(signals);

  useCanvasRenderLoop(canvasRef, height, (context, cssWidth) => {
    if (!globalTimeRangeRef.current) {
      render_empty(context, cssWidth, height);
      return;
    }

    const hoverTimestamp = hoverXRef.current === null ? null : XToTime(hoverXRef.current);

    render(
      context,
      cssWidth,
      height,
      layoutRef,
      {
        type: "numericalGraph",
        signals,
        options,
        data: chartData.current,
        id,
      },
      timeTickCount,
      hoverTimestamp,
      hoveredSignal,
      {
        min: XToTime(CHART_PADDING.left),
        max: XToTime(cssWidth - CHART_PADDING.right),
      },
      getVisibleTelemetryMarkers(XToTime(CHART_PADDING.left), XToTime(cssWidth - CHART_PADDING.right))
    );
  });

  const { handleMouseMove, handleMouseLeave } = useCanvasHover(
    canvasRef,
    hoverXRef,
    (x) => onHoverTimestampChange?.(x === null ? null : XToTime(x))
  );

  return (
    <canvas
      className="block w-full"
      ref={canvasRef}
      style={{ height }}
      onMouseMove={handleMouseMove}
      onMouseLeave={handleMouseLeave}
    />
  );
}
