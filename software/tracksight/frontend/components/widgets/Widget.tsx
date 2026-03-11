"use client";
// react
import { RefObject, useCallback, useRef, useState } from "react";

// components
import CanvasChart from "./CanvasChart";
import { MockWidgetAddSignalModal } from "./MockWidget";

// types
import { EnumSignalConfig, NumericalSignalConfig, WidgetData } from "@/components/widgets/WidgetTypes";
import { ChartData } from "./CanvasChartTypes";

// hooks
import { useDisplayControlContext as useDisplayControlInfo } from "../PausePlayControl";
import { useMockData } from "./MockWidget";
import { useWidgetManager } from "./WidgetManagerContext";
import { SignalType } from "@/lib/types/Signal";
import { NumericalWidgetAddSignalModal } from "./NumericalWidget";

function createEmptyChartData(type: SignalType): ChartData {
	if (type === SignalType.NUMERICAL) {
		return { type: SignalType.NUMERICAL, all_series: [] };
	}

	return { type: SignalType.ENUM, all_series: [] };
}

function SignalButton({ cfg, handleRemoveSignal, hoverSignalName }: {
	cfg: EnumSignalConfig | NumericalSignalConfig, idx: number,
	handleRemoveSignal: (signalName: string) => void,
	hoverSignalName: RefObject<string | null>,
}) {
	// const color = signalColors[idx % signalColors.length];
	return (
		<div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity cursor-crosshair"
			style={{ backgroundColor: cfg.color.brighten(1).hex(), borderColor: cfg.color.darken(1).hex() }}
			onMouseEnter={() => hoverSignalName.current = cfg.signal_name} onMouseLeave={() => hoverSignalName.current = null}
		>
			{/* Left circle */}
			<div className="w-3 h-3 rounded-full" style={{ backgroundColor: cfg.color.hex() }} />
			<span className="font-medium">{cfg.signal_name}</span>
			<span className="text-xs text-gray-500">({cfg.delay}ms)</span>
			<button
				onClick={() => handleRemoveSignal(cfg.signal_name)}
				className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none"
				title="Remove signal"
			>
				×
			</button>
		</div>
	);
}

function useWidgetData(widgetData: WidgetData): RefObject<ChartData> {
	const { isPaused } = useDisplayControlInfo();
	if (widgetData.mock) { // this should be fine as it is constant
		return useMockData(isPaused, widgetData);
	}
	return useRef<ChartData>(createEmptyChartData(widgetData.type)); // TODO populate data from the store
}

export function Widget({ widgetData }: { widgetData: WidgetData; }) {
	const [chartHeight, setChartHeight] = useState(256);
	const { removeWidget, removeSignal } = useWidgetManager();
	const hoveredSignal = useRef<string | null>(null);

	// note that the type of this data should be bound to the type of widgetData
	const dataRef: RefObject<ChartData> = useWidgetData(widgetData);

	const deleteSelfWidget = useCallback(() => { removeWidget(widgetData.id); }, [widgetData.id, removeWidget]);

	return (
		<div className="w-full border-red-500 py-4">
			<div className="px-6">
				{/* header */}
				<div className="flex items-center gap-2 mb-4">
					<h3 className="font-semibold">Widget {widgetData.id}</h3>
					<button onClick={deleteSelfWidget} title="Remove graph"
						className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
					>
						×
					</button>
				</div>

				{/* TODO remove, keep for debugging for now */}
				<label className="text-sm block">Vertical Scale: {chartHeight}px</label>
				<input type="range" min={100} max={600} step={50} value={chartHeight}
					onChange={(e) => setChartHeight(+e.target.value)}
				/>


				{/* Signal buttons */}
				<div className="flex flex-wrap items-center gap-3">
					{widgetData.signals.map((cfg, idx) =>
						<SignalButton key={cfg.signal_name} cfg={cfg} idx={idx}
							handleRemoveSignal={() => removeSignal(widgetData.id, cfg.signal_name)} hoverSignalName={hoveredSignal} />
					)}
					{
						widgetData.mock &&
						<MockWidgetAddSignalModal configs={widgetData.signals} dataRef={dataRef} widgetData={widgetData} />
					}
					{
						!widgetData.mock && widgetData.type == SignalType.NUMERICAL &&
						<NumericalWidgetAddSignalModal configs={widgetData.signals} dataRef={dataRef} widgetData={widgetData} />
					}
				</div>
			</div>

			{/* Actual chart */}
			<CanvasChart widgetData={widgetData} chartData={dataRef} height={chartHeight} hoveredSignal={hoveredSignal} />
		</div>
	)
}
