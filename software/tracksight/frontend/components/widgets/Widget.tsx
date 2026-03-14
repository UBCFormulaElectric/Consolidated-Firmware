"use client";
// react
import { RefObject, useCallback, useEffect, useRef, useState } from "react";

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
import { NumericalWidgetAddSignalModal, useLiveNumericalData } from "./NumericalWidget";
import { fetchNumericalSignalMetadata } from "@/lib/api/signalMetadata";

function SignalButton({ cfg, handleRemoveSignal, hoverSignalName, delay }: {
	cfg: EnumSignalConfig | NumericalSignalConfig, idx: number,
	handleRemoveSignal: (signalName: string) => void,
	hoverSignalName: RefObject<string | null>,
	delay?: number, // if we have fetched delay (i.e. not mock)
}) {
	return (
		<div className="select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 hover:opacity-80 transition-opacity"
			style={{ backgroundColor: cfg.color.brighten(1).hex(), borderColor: cfg.color.darken(1).hex() }}
			onMouseEnter={() => hoverSignalName.current = cfg.signal_name} onMouseLeave={() => hoverSignalName.current = null}
			>
			{/* Left circle */}
			<div className="w-3 h-3 rounded-full" style={{ backgroundColor: cfg.color.hex() }} />
			<span className="font-medium">{cfg.signal_name}</span>
			<span className="text-xs text-gray-500">({delay ?? cfg.delay}ms)</span>
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
	if (widgetData.type === SignalType.NUMERICAL) {
		return useLiveNumericalData(widgetData);
	}
	return useRef<ChartData>(); // TODO populate data from the store
}

function useSignalButtonDelays(widgetData: WidgetData): Record<string, number> {
	const [delays, setDelays] = useState<Record<string, number>>({});

	useEffect(() => {
		if (widgetData.mock || widgetData.type !== SignalType.NUMERICAL) {
			setDelays({});
			return;
		}

		let cancelled = false;

		// fetch metadat for signals
		Promise.all(
			widgetData.signals.map(async (signal) => {
				const metadata = await fetchNumericalSignalMetadata(signal.signal_name);
				return [signal.signal_name, metadata.cycle_time_ms] as const;
			})
		).then((entries) => {
			if (cancelled) {
				return;
			}
			setDelays(Object.fromEntries(entries));
		}).catch((error) => {
			if (!cancelled) {
				console.warn("Failed to fetch signal button metadata:", error);
				setDelays({});
			}
		});

		return () => {
			cancelled = true;
		};
	}, [widgetData]);

	return delays;
}

export function Widget({ widgetData }: { widgetData: WidgetData; }) {
	const [chartHeight, setChartHeight] = useState(256);
	const { removeWidget, removeSignal } = useWidgetManager();
	const hoveredSignal = useRef<string | null>(null);
	const isEmptyWidget = widgetData.signals.length === 0;

	// note that the type of this data should be bound to the type of widgetData
	const dataRef: RefObject<ChartData> = useWidgetData(widgetData);
	const signalButtonDelays = useSignalButtonDelays(widgetData);

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
								handleRemoveSignal={() => removeSignal(widgetData.id, cfg.signal_name)}
								hoverSignalName={hoveredSignal}
								delay={signalButtonDelays[cfg.signal_name]}
							/>
						)}
					{
						widgetData.mock &&
						<MockWidgetAddSignalModal configs={widgetData.signals} dataRef={dataRef} widgetData={widgetData} />
					}
					{
						!widgetData.mock && widgetData.type == SignalType.NUMERICAL &&
						<NumericalWidgetAddSignalModal configs={widgetData.signals} widgetData={widgetData} />
					}
				</div>
			</div>

			{/* Actual chart */}
			{isEmptyWidget ? (
				<div className="mx-6 mt-4 rounded-lg border border-gray-300 bg-gray-50 px-6 py-12 text-center text-sm text-gray-500">
					No signals added.
				</div>
			) : (
				<CanvasChart widgetData={widgetData} chartData={dataRef} height={chartHeight} hoveredSignal={hoveredSignal} />
			)}
		</div>
	)
}
