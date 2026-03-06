"use client";

import { useRef, RefObject, useEffect } from "react";
import render, { render_empty } from "@/components/widgets/render";
import { ChartLayout } from "@/components/widgets/CanvasChartTypes";
import { NumericalSignalConfig } from "@/components/widgets/WidgetTypes";
import { useSyncedGraph } from "@/components/SyncedGraphContainer";
import { useSignalDataStores } from "@/lib/contexts/signalStores/SignalStoreContext";
import { useCanvasRenderLoop } from "@/lib/hooks/useCanvasRenderLoop";
import { useCanvasHover } from "@/lib/hooks/useCanvasHover";
import { SignalType } from "@/lib/types/Signal";

type NumericalCanvasChartProps = {
    id: string;
    signals: NumericalSignalConfig[];
    height: number;
    timeTickCount?: number;
    hoveredSignal?: RefObject<string | null>;
    onHoverTimestampChange?: (timestamp: number | null) => void;
};

export default function NumericalCanvasChart({
    id,
    signals,
    height,
    hoveredSignal,
    onHoverTimestampChange,
    timeTickCount = 6,
}: NumericalCanvasChartProps) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
    const layoutRef = useRef<ChartLayout | null>(null);
    const {
        globalTimeRangeRef,
        hoverTimestampRef: externalHoverTimestampRef,
        XToTime,
    } = useSyncedGraph();

    const chartData = useSignalDataStores(signals);

    useCanvasRenderLoop(canvasRef, height, (context, cssWidth) => {
        if (!globalTimeRangeRef.current) {
            render_empty(context, cssWidth, height);
        } else {
            render(
                context,
                cssWidth,
                height,
                layoutRef,
                {
                    type: SignalType.NUMERICAL,
                    all_series: chartData.current,
                },
                {
                    id,
                    type: SignalType.NUMERICAL,
                    signals,
                },
                timeTickCount,
                externalHoverTimestampRef.current,
                {
                    min: XToTime(0),
                    max: XToTime(cssWidth),
                }
            );
        }
    });

    const { handleMouseMove, handleMouseLeave } = useCanvasHover(
        canvasRef,
        layoutRef,
        externalHoverTimestampRef,
        onHoverTimestampChange
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
