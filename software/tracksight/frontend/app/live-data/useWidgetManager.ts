import { useCallback, useState } from "react";
import { SignalType } from "@/lib/types/Signal";
import { WidgetData } from "@/lib/types/Widget";
import { IS_DEBUG } from "@/lib/constants";

export default function useWidgetManager() {
	// TODO(evan): Remove this it's just temporary data, should be pulled from local storage or something similar
	const [widgets, setWidgets] = useState<WidgetData[]>([
		{
			type: SignalType.ENUM,
			options: {
				colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
			},
			signal: "VC_State",
			id: "widget-1",
		},
		{
			type: SignalType.ENUM,
			options: {
				colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
			},
			signal: "VC_State",
			id: "widget-2",
		},
		{
			type: SignalType.ENUM,
			options: {
				colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
			},
			signal: "VC_State",
			id: "widget-3",
		},
		{
			type: SignalType.ENUM,
			options: {
				colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
			},
			signal: "VC_State",
			id: "widget-4",
		},
	]);
	const appendWidget = useCallback((newWidget: WidgetData) => {
		newWidget.id = crypto.randomUUID();
		setWidgets((prev) => [...prev, newWidget]);
	}, []);
	const removeWidget = useCallback((widgetToRemove: string) => {
		setWidgets((prev) => {
			const widgetIndex = prev.findIndex((w) => w.id === widgetToRemove);
			if (widgetIndex === -1) {
				IS_DEBUG && console.warn("Widget to remove not found");
				return prev;
			}
			const newWidgets = [...prev];
			newWidgets.splice(widgetIndex, 1);
			return newWidgets;
		});
	}, []);
	const setEnumSignal = useCallback(
		(widgetID: string, newSignal: string) => {
			setWidgets((prev) => {
				const widgetIndex = prev.findIndex((w) => w.id === widgetID);
				if (widgetIndex === -1) {
					IS_DEBUG && console.warn("Widget to edit not found");
					return prev;
				}
				const newWidgets = [...prev];
				if (newWidgets[widgetIndex].type !== SignalType.ENUM) {
					IS_DEBUG && console.warn("Widget is not enum type");
					return newWidgets;
				}
				newWidgets[widgetIndex] = {
					...newWidgets[widgetIndex],
					signal: newSignal,
				};
				return newWidgets;
			});
		}, [setWidgets]
	);
	const appendNumSignal = useCallback(
		(widgetID: string, newSignal: string) => {
			setWidgets((prev) => {
				const widgetIndex = prev.findIndex((w) => w.id === widgetID);
				if (widgetIndex === -1) {
					IS_DEBUG && console.warn("Widget to edit not found");
					return prev;
				}

				const newWidgets = [...prev];
				if (newWidgets[widgetIndex].type !== SignalType.NUMERICAL) {
					IS_DEBUG && console.warn("Widget is not numerical type");
					return newWidgets;
				}

				newWidgets[widgetIndex] = {
					...newWidgets[widgetIndex],
					signals: [...newWidgets[widgetIndex].signals, newSignal],
				};
				return newWidgets;
			});
		}, []
	);
	const removeNumSignal = useCallback(
		(widgetID: string, signalToRemove: string) => {
			setWidgets((prev) => {
				const widgetIndex = prev.findIndex((w) => w.id === widgetID);
				if (widgetIndex === -1) {
					IS_DEBUG && console.warn("Widget to edit not found");
					return prev;
				}

				const newWidgets = [...prev];
				if (newWidgets[widgetIndex].type !== SignalType.NUMERICAL) {
					IS_DEBUG && console.warn("Widget is not numerical type");
					return newWidgets;
				}

				newWidgets[widgetIndex] = {
					...newWidgets[widgetIndex],
					signals: newWidgets[widgetIndex].signals.filter(s => s !== signalToRemove)
				};
				return newWidgets;
			});
		}, []
	);

	return {
		widgets,
		appendWidget,
		removeWidget,
		setEnumSignal,
		appendNumSignal,
		removeNumSignal,
	};
}