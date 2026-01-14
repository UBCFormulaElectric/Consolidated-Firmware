import { useCallback, useEffect, useState } from "react";
import { SignalType } from "@/lib/types/Signal";
import { MockGraphConfig, WidgetData } from "@/lib/types/Widget";
import { IS_DEBUG } from "@/lib/constants";
import { v4 as uuidv4 } from 'uuid';

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";

export default function useWidgetManager() {
    const [widgets, setWidgets] = useState<WidgetData[]>([]);
    // used to initialize widgets from localStorage
    const [isInitialized, setIsInitialized] = useState(false);

    // recover from local storage on mount
    useEffect(() => {
        if (typeof window !== "undefined") {
            const saved = localStorage.getItem(LOCAL_STORAGE_KEY);
            if (saved) {
                try {
                    const parsed = JSON.parse(saved);
                    // basic validation could go here
                    if (Array.isArray(parsed)) {
                        setWidgets(parsed);
                    }
                } catch (e) {
                    console.error("Failed to load widgets from local storage", e);
                }
            }
            setIsInitialized(true);
        }
    }, []);

    useEffect(() => {
        if (isInitialized && typeof window !== "undefined") {
            localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(widgets));
        }
    }, [widgets, isInitialized]);

    const appendWidget = useCallback((newWidget: WidgetData) => {
        newWidget.id = uuidv4();
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

                // avoid duplicates
                if (newWidgets[widgetIndex].signals.includes(newSignal)) {
                    IS_DEBUG && console.warn("Signal already exists in widget: " + newSignal);
                    return prev;
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

    const updateWidget = useCallback(
        (widgetID: string, updater: (prevWidget: WidgetData) => WidgetData) => {
            setWidgets((prev) => {
                const widgetIndex = prev.findIndex((w) => w.id === widgetID);
                if (widgetIndex === -1) {
                    IS_DEBUG && console.warn("Widget to update not found");
                    return prev;
                }

                const newWidgets = [...prev];
                newWidgets[widgetIndex] = updater(newWidgets[widgetIndex]);
                return newWidgets;
            });
        },
        []
    );

    return {
        widgets,
        appendWidget,
        removeWidget,
        setEnumSignal,
        appendNumSignal,
        removeNumSignal,
        updateWidget
    };
}
