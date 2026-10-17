import { createContext, ReactNode, useCallback, useContext, useMemo } from "react";

import chroma from "chroma-js";
import { v4 as uuidv4 } from "uuid";

import { IS_DEBUG } from "@/lib/constants";
import { useLocalState } from "@/lib/hooks/useLocalState";
import { isBooleanSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { canWidgetAcceptSignal, WidgetData, WidgetType } from "@/lib/types/Widget";

const LOCAL_STORAGE_KEY = "tracksight_widgets_config_v1";

interface WidgetManagerContext {
    widgets: WidgetData[];
    initializedFromLocalStorage: boolean;
    appendWidget: (newWidget: WidgetData) => void;
    removeWidget: (widgetToRemove: string) => void;
    moveWidget: (widgetToMove: string, insertionIndex: number) => void;
    moveWidgetToIndex: (widgetToMove: string, targetIndex: number) => void;
    appendSignal: <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: any) => void;
    removeSignal: (widget: WidgetData, nameOfSignalToRemove: string) => void;
    moveSignal: (signalName: string, fromWidgetId: string, toWidgetId: string, atIndex?: number) => void;
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
    return JSON.stringify(
        widgets.map((widget) => {
            if (widget.type === "enumTimeline") {
                return {
                    ...widget,
                    options: {
                        ...widget.options,
                        colorPalette: Object.fromEntries(
                            Object.entries(widget.options.colorPalette).map(([signalName, enumColors]) => [
                                signalName,
                                {
                                    color: enumColors.color.hex(),
                                    enumValueColors: Object.fromEntries(Object.entries(enumColors.enumValueColors).map(([enumVal, color]) => [enumVal, color.hex()])),
                                },
                            ])
                        ),
                    },
                };
            }

            return {
                ...widget,
                options: {
                    ...widget.options,
                    colorPalette: Object.fromEntries(Object.entries(widget.options.colorPalette).map(([signalName, color]) => [signalName, color.hex()])),
                },
            };
        })
    );
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
                    colorPalette: Object.fromEntries(
                        Object.entries(candidate.options.colorPalette).map(([signalName, palette]) => [
                            signalName,
                            {
                                color: chroma((palette as { color: string }).color),
                                enumValueColors: Object.fromEntries(Object.entries((palette as { enumValueColors: Record<string, string> }).enumValueColors).map(([enumVal, color]) => [enumVal, chroma(color)])),
                            },
                        ])
                    ),
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
                    colorPalette: Object.fromEntries(Object.entries(candidate.options.colorPalette).map(([signalName, color]) => [signalName, chroma(color as string)])),
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

function moveArrayItem<T>(items: T[], fromIndex: number, toIndex: number): T[] {
    const nextItems = [...items];
    const direction = toIndex > fromIndex ? 1 : -1;

    for (let index = fromIndex; index !== toIndex; index += direction) {
        nextItems[index] = items[index + direction];
    }
    nextItems[toIndex] = items[fromIndex];

    return nextItems;
}

function insertAt<T>(items: T[], item: T, index: number = items.length): T[] {
    return [...items.slice(0, index), item, ...items.slice(index)];
}

function withPaletteEntry<T>(palette: Record<string, T>, signalName: string, entry: T | undefined): Record<string, T> {
    if (entry === undefined) return palette;

    return { ...palette, [signalName]: entry };
}

function addSignalFromWidget(targetWidget: WidgetData, sourceWidget: WidgetData, signal: SignalMetadata, atIndex?: number): WidgetData | null {
    if (targetWidget.type === "numericalGraph" && sourceWidget.type === "numericalGraph" && isNumericalSignalMetadata(signal)) {
        return {
            ...targetWidget,
            signals: insertAt(targetWidget.signals, signal, atIndex),
            options: {
                ...targetWidget.options,
                colorPalette: withPaletteEntry(targetWidget.options.colorPalette, signal.name, sourceWidget.options.colorPalette[signal.name]),
            },
        };
    }

    if (targetWidget.type === "enumTimeline" && sourceWidget.type === "enumTimeline" && (isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal))) {
        return {
            ...targetWidget,
            signals: insertAt(targetWidget.signals, signal, atIndex),
            options: {
                ...targetWidget.options,
                colorPalette: withPaletteEntry(targetWidget.options.colorPalette, signal.name, sourceWidget.options.colorPalette[signal.name]),
            },
        };
    }

    return null;
}

export function WidgetManager({ children, storageKey = LOCAL_STORAGE_KEY }: { children: ReactNode; storageKey?: string }) {
    const [widgets, setWidgets, isInitialized] = useLocalState<WidgetData[]>(storageKey, [], WidgetSerialize, WidgetDeserialize);

    const appendWidget = useCallback(
        (newWidget: WidgetData) => {
            setWidgets((prev) => [...prev, { ...newWidget, id: uuidv4() }]);
        },
        [setWidgets]
    );

    const removeWidget = useCallback(
        (widgetToRemove: string) => {
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
        },
        [setWidgets]
    );

    const moveWidget = useCallback(
        (widgetToMove: string, insertionIndex: number) => {
            setWidgets((prev) => {
                const fromIndex = prev.findIndex((widget) => widget.id === widgetToMove);
                if (fromIndex === -1) {
                    IS_DEBUG && console.warn("Widget to move not found");
                    return prev;
                }

                const toIndex = insertionIndex > fromIndex ? insertionIndex - 1 : insertionIndex;
                if (toIndex === fromIndex) {
                    return prev;
                }

                return moveArrayItem(prev, fromIndex, toIndex);
            });
        },
        [setWidgets]
    );

    const moveWidgetToIndex = useCallback(
        (widgetToMove: string, targetIndex: number) => {
            setWidgets((prev) => {
                const fromIndex = prev.findIndex((widget) => widget.id === widgetToMove);
                if (fromIndex === -1) {
                    IS_DEBUG && console.warn("Widget to move not found");
                    return prev;
                }

                if (targetIndex < 0 || targetIndex >= prev.length) {
                    IS_DEBUG && console.warn(`Widget target index ${targetIndex} is out of range for ${prev.length} widgets`);
                    return prev;
                }

                if (targetIndex === fromIndex) return prev;

                return moveArrayItem(prev, fromIndex, targetIndex);
            });
        },
        [setWidgets]
    );

    const appendSignal = useCallback(
        <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, newSignal: any) => {
            setWidgets((prev) => {
                const widgetIndex = prev.findIndex((candidate) => candidate.id === widget.id);
                if (widgetIndex === -1) {
                    IS_DEBUG && console.warn("Widget to edit not found");
                    return prev;
                }

                const existingWidget = prev[widgetIndex] as any;
                if (!("signals" in existingWidget)) {
                    IS_DEBUG && console.warn("Widget does not support signals");
                    return prev;
                }

                if (existingWidget.signals.some((signal: any) => signal.name === newSignal.name)) {
                    IS_DEBUG && console.warn(`Signal already exists in widget: ${newSignal.name}`);
                    return prev;
                }

                const nextWidgets = [...prev];
                nextWidgets[widgetIndex] = {
                    ...existingWidget,
                    // TODO(evan): I'll fix these generics in the future 🙏🏽 promise.
                    // NOTE(evan): any cast shifts the burden of type safety to the caller, which is bad
                    //             so it should be fixed eventually, but currently the only callers
                    //             are the enum and numerical signal pickers which are fully sound
                    //             and already guard their own types correctly.
                    signals: [...existingWidget.signals, newSignal] as any,
                };
                return nextWidgets;
            });
        },
        [setWidgets]
    );

    const removeSignal = useCallback(
        (widget: WidgetData, nameOfSignalToRemove: string) => {
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
        },
        [setWidgets]
    );

    const moveSignal = useCallback(
        (signalName: string, fromWidgetId: string, toWidgetId: string, atIndex?: number) => {
            setWidgets((prev) => {
                const fromIndex = prev.findIndex((widget) => widget.id === fromWidgetId);
                const toIndex = prev.findIndex((widget) => widget.id === toWidgetId);
                if (fromIndex === -1 || toIndex === -1) {
                    IS_DEBUG && console.warn("Widget to move signal between not found");
                    return prev;
                }

                const sourceWidget = prev[fromIndex];

                const signal = sourceWidget.signals.find((candidate) => candidate.name === signalName);
                if (!signal) {
                    IS_DEBUG && console.warn(`Signal to move not found in source widget: ${signalName}`);
                    return prev;
                }

                const nextSourceWidget = removeSignalFromWidget(sourceWidget, signalName);
                const targetWidget = fromIndex === toIndex ? nextSourceWidget : prev[toIndex];

                if (!canWidgetAcceptSignal(targetWidget, signal)) {
                    IS_DEBUG && console.warn(`Target widget cannot accept signal: ${signalName}`);
                    return prev;
                }

                const nextTargetWidget = addSignalFromWidget(targetWidget, sourceWidget, signal, atIndex);
                if (!nextTargetWidget) {
                    IS_DEBUG && console.warn(`Signal palette cannot be moved between these widget types: ${signalName}`);
                    return prev;
                }

                const nextWidgets = [...prev];
                nextWidgets[fromIndex] = nextSourceWidget;
                nextWidgets[toIndex] = nextTargetWidget;
                return nextWidgets;
            });
        },
        [setWidgets]
    );

    const updateWidget = useCallback(
        <T extends WidgetType, Widget extends Extract<WidgetData, { type: T }>>(widget: Widget, updater: (prevWidget: Widget) => Widget) => {
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
        },
        [setWidgets]
    );

    const contextValue = useMemo<WidgetManagerContext>(
        () => ({
            widgets,
            appendWidget,
            removeWidget,
            moveWidget,
            moveWidgetToIndex,
            appendSignal,
            removeSignal,
            moveSignal,
            updateWidget,
            initializedFromLocalStorage: isInitialized,
        }),
        [widgets, appendWidget, removeWidget, moveWidget, moveWidgetToIndex, appendSignal, removeSignal, moveSignal, updateWidget, isInitialized]
    );

    return <WidgetManagerContext value={contextValue}>{children}</WidgetManagerContext>;
}
