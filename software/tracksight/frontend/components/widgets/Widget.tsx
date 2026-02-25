"use client";
import { RefObject, useCallback, useRef, useState } from "react";

// components
import { Dialog, DialogContent, DialogTrigger } from "@/components/ui/dialog"
import { PlusButton } from "@/components/icons/PlusButton";

// types
import { EnumSignalConfig, NumericalSignalConfig, WidgetData } from "@/lib/types/Widget";
import { signalColors } from "@/components/widgets/signalColors";
import { useWidgetManager } from "./WidgetManagerContext";
import CanvasChart, { SeriesData } from "./CanvasChart";
import { MockWidgetAddSignalModal, useMockData } from "./MockWidget";
import { useDisplayControl } from "../PausePlayControl";
import { ChartData } from "./chart_types";

function SignalButton({ cfg, idx, handleRemoveSignal, onHover, onHoverEnd }: {
	cfg: EnumSignalConfig | NumericalSignalConfig, idx: number,
	handleRemoveSignal: (signalName: string) => void,
	onHover: (signalName: string) => void,
	onHoverEnd: () => void,
}) {
	const color = signalColors[idx % signalColors.length];
	// TODO hover highlights the signal in graph
	return (
		<div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity"
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
	const { isPaused } = useDisplayControl();
	if (widgetData.mock) { // this should be fine as it is constant
		return useMockData(isPaused, widgetData);
	} else {
		return useRef({} as ChartData);
	}
}

export function Widget({ widgetData }: { widgetData: WidgetData; }) {
	const [modalOpen, setModalOpen] = useState(false);
	const [chartHeight, setChartHeight] = useState(256);
	const { removeWidget, removeSignal } = useWidgetManager();
	const dataRef = useWidgetData(widgetData);

	const deleteSelfWidget = useCallback(() => { removeWidget(widgetData.id); }, [widgetData.id, removeWidget]);

	return (
		<div className="w-full border-red-500 py-4">
			<div className="px-6">
				{/* header */}
				<div className="flex items-center gap-2 mb-4">
					<h3 className="font-semibold">Widget {widgetData.id}</h3>
					<button onClick={deleteSelfWidget} title="Remove mock graph"
						className="w-6 h-6 bg-red-500 hover:bg-red-600 text-white rounded-full flex items-center justify-center text-sm font-bold transition-colors cursor-pointer"
					>
						×
					</button>
				</div>

				{/* TODO remove, keep for debugging for now */}
				{/* <label className="text-sm block">Vertical Scale: {chartHeight}px</label>
					<input type="range" min={100} max={600} step={50} value={chartHeight}
						onChange={(e) => setChartHeight(+e.target.value)}
					/>
				*/}

				{/* Signal buttons */}
				<div className="flex flex-wrap items-center gap-3">
					{widgetData.configs.map((cfg, idx) =>
						<SignalButton
							key={cfg.signal_name} cfg={cfg} idx={idx}
							handleRemoveSignal={() => removeSignal(widgetData.id, cfg.signal_name)}
							onHover={() => { }} onHoverEnd={() => { }} // TODO hover handlers?
						/>
					)}
					<Dialog open={modalOpen} onOpenChange={setModalOpen}>
						<DialogTrigger>
							<PlusButton />
						</DialogTrigger>
						<DialogContent>
							<MockWidgetAddSignalModal configs={widgetData.configs} dataRef={dataRef}
								widgetData={widgetData} closeModal={() => setModalOpen(false)} updateWidget={() => { }} />
						</DialogContent>
					</Dialog>
				</div>
			</div>

			{/* Actual chart */}
			<CanvasChart chartData={dataRef} height={chartHeight} />
		</div>
	)
}
