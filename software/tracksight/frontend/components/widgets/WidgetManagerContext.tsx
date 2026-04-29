import { ReactNode, createContext, useCallback, useContext, useMemo } from "react";

import chroma from "chroma-js";
import { v4 as uuidv4 } from "uuid";

import { IS_DEBUG } from "@/lib/constants";
import { useLocalState } from "@/lib/hooks/useLocalState";
import { WidgetData, WidgetType } from "@/lib/types/Widget";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";

interface WidgetManagerContext {
    widgets: WidgetData[];
    initializedFromLocalStorage: boolean;
    appendWidget: (newWidget: WidgetData) => void;
    removeWidget: (widgetToRemove: string) => void;
    appendSignal: <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: Widget["signals"][number]) => void;
    removeSignal: (widget: WidgetData, nameOfSignalToRemove: string) => void;
    updateWidget: <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, updater: (prevWidget: Widget) => Widget) => void;
}

const WidgetManagerContext = createContext<WidgetManagerContext | null>(null);

export function useWidgetManager() {
    const ctx = useContext(WidgetManagerContext);
    if (!ctx) {
        throw new Error("useWidgetManagerContext must be used within a WidgetManagerProvider");
    }
    return ctx;
}

function WidgetSerialize(widgets: WidgetData[]): string {
    return JSON.stringify(widgets.map((widget) => {
        if (widget.type === "enumTimeline") {
            return {
                ...widget,
                options: {
                    ...widget.options,
                    colorPalette: Object.fromEntries(Object.entries(widget.options.colorPalette).map(([signalName, enumColors]) => [
                        signalName,
                        {
                            color: enumColors.color.hex(),
                            enumValueColors: Object.fromEntries(Object.entries(enumColors.enumValueColors).map(([enumVal, color]) => [
                                enumVal,
                                color.hex(),
                            ])),
                        },
                    ])),
                },
            };
        }

        return {
            ...widget,
            options: {
                ...widget.options,
                colorPalette: Object.fromEntries(Object.entries(widget.options.colorPalette).map(([signalName, color]) => [
                    signalName,
                    color.hex(),
                ])),
            },
        };
    }));
}

function WidgetDeserialize(widgetString: string): WidgetData[] {
    const back = JSON.parse(widgetString) as unknown;

    if (!Array.isArray(back)) {
        throw new Error("Deserialized widget config is not an array");
    }

    return back.map((widget) => {
        if (!widget || typeof widget !== "object" || !("type" in widget)) {
            throw new Error("Deserialized widget has invalid shape");
        }

        const candidate = widget as any;

        if (candidate.type === "enumTimeline") {
            return {
                id: candidate.id,
                type: "enumTimeline",
                data: candidate.data,
                signals: candidate.signals,
                options: {
                    height: candidate.options.height,
                    timeTickCount: candidate.options.timeTickCount,
                    colorPalette: Object.fromEntries(Object.entries(candidate.options.colorPalette).map(([signalName, palette]) => [
                        signalName,
                        {
                            color: chroma((palette as { color: string }).color),
                            enumValueColors: Object.fromEntries(Object.entries((palette as { enumValueColors: Record<string, string> }).enumValueColors).map(([enumVal, color]) => [
                                enumVal,
                                chroma(color),
                            ])),
                        },
                    ])),
                },
            } satisfies WidgetData;
        }

        if (candidate.type === "numericalGraph") {
            return {
                id: candidate.id,
                type: "numericalGraph",
                signals: candidate.signals,
                data: candidate.data,
                options: {
                    colorPalette: Object.fromEntries(Object.entries(candidate.options.colorPalette).map(([signalName, color]) => [
                        signalName,
                        chroma(color as string),
                    ])),
                    height: candidate.options.height,
                    timeTickCount: candidate.options.timeTickCount,
                },
            } satisfies WidgetData;
        }

        throw new Error(`Deserialized widget has invalid type: ${candidate.type}`);
    });
}

function removeSignalFromWidget(widget: WidgetData, signalName: string): WidgetData {
    if (widget.type === "enumTimeline") {
        const nextPalette = { ...widget.options.colorPalette };
        delete nextPalette[signalName];

        return {
            ...widget,
            signals: widget.signals.filter((signal) => signal.name !== signalName),
            options: {
                ...widget.options,
                colorPalette: nextPalette,
            },
        };
    }

    const nextPalette = { ...widget.options.colorPalette };
    delete nextPalette[signalName];

    return {
        ...widget,
        signals: widget.signals.filter((signal) => signal.name !== signalName),
        options: {
            ...widget.options,
            colorPalette: nextPalette,
        },
    };
}

export function WidgetManager({ children }: { children: ReactNode }) {
    const [widgets, setWidgets, isInitialized] = useLocalState<WidgetData[]>(LOCAL_STORAGE_KEY, [], WidgetSerialize, WidgetDeserialize);

    const appendWidget = useCallback((newWidget: WidgetData) => {
        setWidgets((prev) => [...prev, { ...newWidget, id: uuidv4() }]);
    }, [setWidgets]);

    const removeWidget = useCallback((widgetToRemove: string) => {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((widget) => widget.id === widgetToRemove);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to remove not found");
                return prev;
            }

            const nextWidgets = [...prev];
            nextWidgets.splice(widgetIndex, 1);
            return nextWidgets;
        });
    }, [setWidgets]);

    const appendSignal = useCallback(<T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: Widget["signals"][number]) => {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((candidate) => candidate.id === widget.id);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }

            const existingWidget = prev[widgetIndex] as Widget;
            if (existingWidget.signals.some((signal) => signal.name === newSignal.name)) {
                IS_DEBUG && console.warn(`Signal already exists in widget: ${newSignal.name}`);
                return prev;
            }

            const nextWidgets = [...prev];
            nextWidgets[widgetIndex] = {
                ...existingWidget,
                signals: [...existingWidget.signals, newSignal],
            };
            return nextWidgets;
        });
    }, [setWidgets]);

    const removeSignal = useCallback((widget: WidgetData, nameOfSignalToRemove: string) => {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((candidate) => candidate.id === widget.id);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to edit not found");
                return prev;
            }

            const nextWidgets = [...prev];
            nextWidgets[widgetIndex] = removeSignalFromWidget(prev[widgetIndex], nameOfSignalToRemove);
            return nextWidgets;
        });
    }, [setWidgets]);

    const updateWidget = useCallback(<T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, updater: (prevWidget: Widget) => Widget) => {
        setWidgets((prev) => {
            const widgetIndex = prev.findIndex((candidate) => candidate.id === widget.id);
            if (widgetIndex === -1) {
                IS_DEBUG && console.warn("Widget to update not found");
                return prev;
            }

            const nextWidgets = [...prev];
            nextWidgets[widgetIndex] = updater(prev[widgetIndex] as Widget);
            return nextWidgets;
        });
    }, [setWidgets]);

    const contextValue = useMemo<WidgetManagerContext>(() => ({
        widgets,
        appendWidget,
        removeWidget,
        appendSignal,
        removeSignal,
        updateWidget,
        initializedFromLocalStorage: isInitialized,
    }), [widgets, appendWidget, removeWidget, appendSignal, removeSignal, updateWidget, isInitialized]);

    return (
        <WidgetManagerContext value={contextValue}>
            {children}
        </WidgetManagerContext>
    );
}
