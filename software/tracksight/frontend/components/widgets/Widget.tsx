"use client";
// react
import { RefObject, use, useCallback, useRef, useState } from "react";

// components
import CanvasChart from "./CanvasChart";
import { MockWidgetAddSignalModal } from "./MockWidget";

// types
import { EnumSignalConfig, NumericalSignalConfig, WidgetData } from "@/components/widgets/WidgetTypes";
import { ChartData } from "./CanvasChartTypes";

// constants
import { signalColors } from "@/components/widgets/signalColors";

// hooks
import { useDisplayControlContext as useDisplayControlInfo } from "../PausePlayControl";
import { useMockData } from "./MockWidget";
import { useWidgetManager } from "./WidgetManagerContext";

function SignalButton({ cfg, idx, handleRemoveSignal, onHover, onHoverEnd }: {
	cfg: EnumSignalConfig | NumericalSignalConfig, idx: number,
	handleRemoveSignal: (signalName: string) => void,
	onHover: (signalName: string) => void,
	onHoverEnd: () => void,
}) {
	const color = signalColors[idx % signalColors.length];
	return (
		<div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity cursor-crosshair"
			style={{ backgroundColor: color + "20", borderColor: color + "80" }}
			onMouseEnter={() => onHover(cfg.signal_name)} onMouseLeave={() => onHoverEnd()}
		>
			<div className="w-3 h-3 rounded-full" style={{ backgroundColor: color }} />
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
	// TODO populate data for numerical and enum from the store
	return useRef({} as ChartData);
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
					{widgetData.configs.map((cfg, idx) =>
						<SignalButton
							key={cfg.signal_name} cfg={cfg} idx={idx}
							handleRemoveSignal={() => removeSignal(widgetData.id, cfg.signal_name)}
							onHover={(name) => hoveredSignal.current = name} onHoverEnd={() => hoveredSignal.current = null}
						/>
					)}
					{
						widgetData.mock &&
						<MockWidgetAddSignalModal configs={widgetData.configs} dataRef={dataRef}
							widgetData={widgetData} updateWidget={() => {
								throw new Error("updateWidget function not implemented for non-mock widget");
							}} />
					}

				</div>
			</div>

			{/* Actual chart */}
			<CanvasChart chartData={dataRef} height={chartHeight} hoveredSignal={hoveredSignal} />
		</div>
	)
}
