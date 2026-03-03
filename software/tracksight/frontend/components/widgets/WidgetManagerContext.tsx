import { createContext, useCallback, useContext, useMemo, ReactNode } from "react";
import { SignalType } from "@/lib/types/Signal";
import { EnumSignalConfig, WidgetData } from "./WidgetTypes";
import { IS_DEBUG } from "@/lib/constants";
import { v4 as uuidv4 } from 'uuid';
import { useLocalState } from "@/lib/hooks/useLocalState";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";


interface WidgetManagerContext {
    widgets: WidgetData[];
    initializedFromLocalStorage: boolean;
    appendWidget: (newWidget: WidgetData) => void;
    removeWidget: (widgetToRemove: string) => void;
    // setEnumSignal: (widgetID: string, newSignal: string) => void;
    appendSignal: (widgetID: string, newSignal: string) => void;
    removeSignal: (widgetID: string, signalToRemove: string) => void;
    updateWidget: (widgetID: string, updater: (prevWidget: WidgetData) => WidgetData) => void;
}
const WidgetManagerContext = createContext<WidgetManagerContext | null>(null);
export function useWidgetManager() {
    const ctx = useContext(WidgetManagerContext);
    if (!ctx) { throw new Error("useWidgetManagerContext must be used within a WidgetManagerProvider"); }
    return ctx;
}

export function WidgetManager({ children }: { children: ReactNode }) {
    const [widgets, setWidgets, isInitialized] = useLocalState<WidgetData[]>(LOCAL_STORAGE_KEY, []);

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

    const appendSignal = useCallback((widgetID: string, new_signal_name: string) => {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((w) => w.id === widgetID);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }
            const newWidgets = [...prev];

            // avoid duplicates
            if (newWidgets[widgetIndex].signals.some(c => c.signal_name === new_signal_name)) {
                IS_DEBUG && console.warn("Signal already exists in widget: " + new_signal_name);
                return prev;
            }
            if (newWidgets[widgetIndex].type === SignalType.NUMERICAL) {
                newWidgets[widgetIndex] = {
                    ...newWidgets[widgetIndex],
                    signals: [...newWidgets[widgetIndex].signals, {
                        signal_name: new_signal_name,
                        delay: 0,
                        initialPoints: 100,
                        min: 0,
                        max: 100
                    }]
                };
            } else {
                newWidgets[widgetIndex] = {
                    ...newWidgets[widgetIndex],
                    signals: [...newWidgets[widgetIndex].signals, ({
                        signal_name: new_signal_name,
                        delay: 0,
                        initialPoints: 0,
                        options: {
                            colorPalette: []
                        }
                    } satisfies EnumSignalConfig)]
                }
            }

            return newWidgets;
        });
    }, []);

    const removeSignal = useCallback(function (widgetID: string, remove_signal_name: string) {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((w) => w.id === widgetID);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }
            const newWidgets = [...prev];
            newWidgets[widgetIndex] = {
                ...newWidgets[widgetIndex],
                signals: newWidgets[widgetIndex].signals.filter(c => c.signal_name !== remove_signal_name) as Array<any> // trust
            };
            return newWidgets;
        });
    }, []);

    const updateWidget = useCallback((
        widgetID: string,
        updater: (prevWidget: WidgetData) => WidgetData
    ) => {
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
    }, []);

    const CTXVAL = useMemo<WidgetManagerContext>(() => ({
        widgets, appendWidget, removeWidget,
        appendSignal, removeSignal,
        updateWidget,
        initializedFromLocalStorage: isInitialized
    }), [
        widgets, appendWidget, removeWidget,
        appendSignal, removeSignal,
        updateWidget, isInitialized
    ]);

    return (
        <WidgetManagerContext value={CTXVAL} >
            {children}
        </WidgetManagerContext>
    )
}
