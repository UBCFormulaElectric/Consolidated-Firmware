// react
import { createContext, useCallback, useContext, useMemo, ReactNode } from "react";
// types
import { SignalType } from "@/lib/types/Signal";
import { EnumSignalConfig, NumericalSignalConfig, WidgetData, WidgetDataEnum, WidgetDataNumerical, WidgetSignalConfig } from "./WidgetTypes";
import chroma from "chroma-js";
// constants
import { IS_DEBUG } from "@/lib/constants";
// functions
import { v4 as uuidv4 } from 'uuid';
// hooks
import { useLocalState } from "@/lib/hooks/useLocalState";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";

interface WidgetManagerContext {
    widgets: WidgetData[];
    initializedFromLocalStorage: boolean;
    appendWidget: (newWidget: WidgetData) => void;
    removeWidget: (widgetToRemove: string) => void;
    // setEnumSignal: (widgetID: string, newSignal: string) => void;
    appendSignal: (widgetID: string, new_signal: WidgetSignalConfig) => void;
    removeSignal: (widgetID: string, signalToRemove: string) => void;
    updateWidget: (widgetID: string, updater: (prevWidget: WidgetData) => WidgetData) => void;
}
const WidgetManagerContext = createContext<WidgetManagerContext | null>(null);
export function useWidgetManager() {
    const ctx = useContext(WidgetManagerContext);
    if (!ctx) { throw new Error("useWidgetManagerContext must be used within a WidgetManagerProvider"); }
    return ctx;
}

function WidgetSerialize(widgets: WidgetData[]): string {
    return JSON.stringify(widgets.map(w => {
        if (w.type == SignalType.ENUM) {
            return {
                ...w,
                signals: w.signals.map(s => ({
                    ...s,
                    color: s.color.hex(),
                    options: {
                        colorPalette: s.options.colorPalette.map(c => c.hex())
                    }
                }))
            }
        }
        else if (w.type == SignalType.NUMERICAL) {
            return {
                ...w,
                signals: w.signals.map(s => ({
                    ...s,
                    color: s.color.hex()
                }))
            }
        }
    }));
}

// yolo if it doesn't have it it is what it is fr
function WidgetDeserialize(widgetString: string): WidgetData[] {
    const back: unknown = JSON.parse(widgetString);
    if (!Array.isArray(back)) {
        throw new Error("Deserialized widget config is not an array");
    }
    return back.map(b => {
        if (b.type === SignalType.ENUM) {
            return {
                id: b.id,
                type: SignalType.ENUM,
                mock: b.mock,
                signals: b.signals.map((s: any) => ({
                    signal_name: s.signal_name,
                    delay: s.delay,
                    initialPoints: s.initialPoints,
                    options: {
                        colorPalette: s.options.colorPalette.map((c: string) => chroma(c))
                    },
                    color: chroma(s.color)
                })) satisfies EnumSignalConfig[]
            } satisfies WidgetDataEnum;
        } else if (b.type === SignalType.NUMERICAL) {
            return {
                id: b.id,
                type: SignalType.NUMERICAL,
                mock: b.mock,
                signals: b.signals.map((s: any) => ({
                    signal_name: s.signal_name,
                    delay: s.delay,
                    min: s.min,
                    max: s.max,
                    color: chroma(s.color)
                }) satisfies NumericalSignalConfig)
            } satisfies WidgetDataNumerical;
        }
        else {
            throw new Error("Deserialized widget has invalid type: " + (b as any).type);
        }
    })
}

export function WidgetManager({ children }: { children: ReactNode }) {
    const [widgets, setWidgets, isInitialized] = useLocalState<WidgetData[]>(LOCAL_STORAGE_KEY, [], WidgetSerialize, WidgetDeserialize);

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

    const appendSignal = useCallback((widgetID: string, new_signal: WidgetSignalConfig) => {
        setWidgets((prev) => {
            const newWidgets = [...prev];
            const widgetIndex = newWidgets.findIndex((w) => w.id === widgetID);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }
            const updateWidget = newWidgets[widgetIndex];

            // avoid duplicates
            if (updateWidget.signals.some(c => c.signal_name === new_signal.signal_name)) {
                IS_DEBUG && console.warn("Signal already exists in widget: " + new_signal.signal_name);
                return prev;
            }
            if (updateWidget.type === SignalType.NUMERICAL) {
                newWidgets[widgetIndex] = {
                    ...updateWidget,
                    signals: [...updateWidget.signals, (new_signal as NumericalSignalConfig)] // inshallah ig
                };
            } else {
                newWidgets[widgetIndex] = {
                    ...updateWidget,
                    signals: [...updateWidget.signals, (new_signal as EnumSignalConfig)] // inshallah ig
                };
            }

            return newWidgets;
        });
    }, []);

    const removeSignal = useCallback(function (widgetID: string, remove_signal_name: string) {
        setWidgets((prev) => {
            const newWidgets = [...prev];
            const widgetIndex = newWidgets.findIndex((w) => w.id === widgetID);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }
            const updateWidget = newWidgets[widgetIndex];
            newWidgets[widgetIndex] = {
                ...updateWidget,
                signals: updateWidget.signals.filter(c => c.signal_name !== remove_signal_name) as Array<any> // trust
            };
            return newWidgets;
        });
    }, []);

    const updateWidget = useCallback((
        widgetID: string,
        updater: (prevWidget: WidgetData) => WidgetData
    ) => {
        setWidgets((prev) => {
            const newWidgets = [...prev];
            const widgetIndex = newWidgets.findIndex((w) => w.id === widgetID);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }
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
