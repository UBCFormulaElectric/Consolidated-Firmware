"use client";
import EnumWidget from "@/components/widgets/EnumWidget";
import { SignalType } from "@/lib/types/Signal";
import { WidgetData } from "@/lib/types/Widget";
import { useCallback } from "react";

export function Widget({ widgetData, setEnumSignal, appendNumSignal, removeNumSignal }: {
	widgetData: WidgetData;
	setEnumSignal: (widgetId: string, newSignal: string) => void;
	appendNumSignal: (widgetId: string, newSignal: string) => void;
	removeNumSignal: (widgetId: string, signalToRemove: string) => void;
}) {
	const setEnumSignalCurried = useCallback(
		(newSignal: string) => setEnumSignal(widgetData.id, newSignal),
		[widgetData.id, setEnumSignal]
	);
	const setAppendNumSignalCurried = useCallback(
		(newSignal: string) => appendNumSignal(widgetData.id, newSignal),
		[widgetData.id, appendNumSignal]
	);
	const setRemoveNumSignalCurried = useCallback(
		(signalToRemove: string) => removeNumSignal(widgetData.id, signalToRemove),
		[widgetData.id, removeNumSignal]
	);

	switch (widgetData.type) {
		case SignalType.ENUM:
			return <EnumWidget
				widgetData={widgetData}
				setEnumSignal={setEnumSignalCurried}
				key={widgetData.id} />;
		case SignalType.NUMERICAL:
			return <div key={widgetData.id}>Numerical Widget - To Be Implemented</div>;
		default:
			throw new Error(`Should be unreachable: Unsupported widget type: ${(widgetData as any).type}`);
	}
}
