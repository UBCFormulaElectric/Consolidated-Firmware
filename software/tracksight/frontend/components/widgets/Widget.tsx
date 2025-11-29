"use client";
import EnumWidget from "@/components/widgets/EnumWidget";
import NumericalWidget from "@/components/widgets/NumericalWidget";
import MockWidget from "@/components/widgets/MockWidget";
import { SignalType } from "@/lib/types/Signal";
import { MockGraphConfig, WidgetData } from "@/lib/types/Widget";
import { useCallback } from "react";

export function Widget({ 
	widgetData, 
	setEnumSignal, 
	appendNumSignal, 
	removeNumSignal,
	updateMockConfig,
	removeWidget
}: {
	widgetData: WidgetData;
	setEnumSignal: (widgetId: string, newSignal: string) => void;
	appendNumSignal: (widgetId: string, newSignal: string) => void;
	removeNumSignal: (widgetId: string, signalToRemove: string) => void;
	updateMockConfig: (widgetId: string, updater: (prevConfigs: MockGraphConfig[]) => MockGraphConfig[]) => void;
	removeWidget: (widgetId: string) => void;
}) {
	const setEnumSignalCurried = useCallback(
		(newSignal: string) => setEnumSignal(widgetData.id, newSignal),
		[widgetData.id, setEnumSignal]
	);

	const handleDelete = useCallback(() => {
		removeWidget(widgetData.id);
	}, [widgetData.id, removeWidget]);

	switch (widgetData.type) {
		case SignalType.ENUM:
			return <EnumWidget
				widgetData={widgetData}
				setEnumSignal={setEnumSignalCurried}
				key={widgetData.id} />;
		case SignalType.NUMERICAL:
			return <NumericalWidget 
				widgetData={widgetData}
				appendNumSignal={appendNumSignal}
				removeNumSignal={removeNumSignal}
				onDelete={handleDelete}
				key={widgetData.id}
			/>;
		case SignalType.MOCK:
			return <MockWidget
				widgetData={widgetData}
				updateMockConfig={updateMockConfig}
				onDelete={handleDelete}
				key={widgetData.id}
			/>;
		default:
			throw new Error(`Should be unreachable: Unsupported widget type: ${(widgetData as any).type}`);
	}
}
