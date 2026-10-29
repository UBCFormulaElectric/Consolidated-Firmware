"use client";

import chroma, { Color } from "chroma-js";
import { ChevronDown, GripVertical, Pencil } from "lucide-react";
import { memo, ReactNode, RefObject, useCallback, useEffect, useRef, useState } from "react";
import { ConnectDragSource, useDrag, useDragLayer } from "react-dnd";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { DRAGGABLE_TYPES } from "@/lib/constants";
import { BooleanSignalMetadata, EnumSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { EnumTimelineWidgetData, NumericalGraphWidgetData, SignalDragItem, SignalDropResult, WidgetData, WidgetDragItem } from "@/lib/types/Widget";
import EnumCanvasChart from "./EnumCanvasChart";
import NumericalCanvasChart from "./NumericalCanvasChart";
import { isStateSignal, SignalPicker } from "./SignalPicker";
import { useWidgetManager } from "./WidgetManagerContext";

export function buildEnumPalette(signal: EnumSignalMetadata | BooleanSignalMetadata): { color: Color; enumValueColors: Record<number, Color> } {
    const enumValues = isEnumSignalMetadata(signal) ? Object.values(signal.enum_signal.enum_values).sort((left, right) => left - right) : [0, 1];
    const startingHue = Math.random() * 360;
    const hueStep = enumValues.length > 0 ? 360 / enumValues.length : 360;
    const enumValueColors: Record<number, Color> = {};

    enumValues.forEach((enumValue, index) => {
        enumValueColors[enumValue] = chroma.hsl((startingHue + hueStep * index) % 360, 0.72, 0.56);
    });

    return {
        color: chroma.hsl(startingHue, 0.72, 0.56),
        enumValueColors,
    };
}

function removeColorPaletteEntry<T>(palette: Record<string, T>, signalName: string): Record<string, T> {
    const nextPalette = { ...palette };
    delete nextPalette[signalName];
    return nextPalette;
}

function useDraggedAwaySignal(widgetId: string): SignalMetadata | null {
    return useDragLayer((monitor) => {
        if (monitor.getItemType() !== DRAGGABLE_TYPES.SIGNAL) return null;

        const item = monitor.getItem<SignalDragItem>();
        const isDraggedAwayFromWidget = item.originalWidgetId === widgetId && item.currentWidgetId !== widgetId;

        return isDraggedAwayFromWidget ? item.signal : null;
    });
}

function SignalButton(props: { signal: SignalMetadata; signalIndex: number; widgetId: string; isHidden: boolean; handleRemoveSignal: (signalName: string) => void; hoverSignalName: RefObject<string | null>; color: Color }) {
    const { signal, signalIndex, widgetId, isHidden, handleRemoveSignal, hoverSignalName, color } = props;
    const { moveSignal } = useWidgetManager();

    const [{ isDragging }, drag] = useDrag(
        () => ({
            type: DRAGGABLE_TYPES.SIGNAL,
            item: (): SignalDragItem => {
                hoverSignalName.current = null;

                return { signal, originalWidgetId: widgetId, originalIndex: signalIndex, currentWidgetId: widgetId };
            },
            end: (item, monitor) => {
                if (monitor.getDropResult<SignalDropResult>()?.isAccepted) return;

                moveSignal(item.signal.name, item.currentWidgetId, item.originalWidgetId, item.originalIndex);
            },
            isDragging: (monitor) => {
                const item = monitor.getItem<SignalDragItem>();

                return item.signal.name === signal.name && item.currentWidgetId === widgetId;
            },
            collect: (monitor) => ({
                isDragging: monitor.isDragging(),
            }),
        }),
        [signal, signalIndex, widgetId, hoverSignalName, moveSignal]
    );

    return (
        <div
            ref={(node) => {
                drag(node);
            }}
            className={`select-none flex items-center gap-2 px-3 py-1.5 rounded-full border-2 transition-opacity cursor-grab ${isDragging ? "opacity-50" : "hover:opacity-80"} ${isHidden ? "hidden" : ""}`}
            style={{ backgroundColor: color.brighten(1).hex(), borderColor: color.darken(1).hex() }}
            onMouseEnter={() => {
                hoverSignalName.current = signal.name;
            }}
            onMouseLeave={() => {
                hoverSignalName.current = null;
            }}
        >
            <div className="w-3 h-3 rounded-full" style={{ backgroundColor: color.hex() }} />
            <span className="font-medium">{signal.name}</span>
            {signal.cycle_time_ms !== null && <span className="text-xs text-gray-500">({signal.cycle_time_ms}ms)</span>}
            <button type="button" onClick={() => handleRemoveSignal(signal.name)} className="ml-1 text-gray-500 hover:text-red-500 transition-colors font-bold cursor-pointer focus:text-red-500 focus:outline-none" title="Remove signal">
                ×
            </button>
        </div>
    );
}

function CollapseToggle(props: { isCollapsed: boolean; onToggle: () => void; title: string; children?: ReactNode }) {
    const { isCollapsed, onToggle, title, children } = props;

    return (
        <button type="button" onClick={onToggle} className="flex shrink-0 items-center gap-1 rounded text-xs font-medium text-gray-500 transition-colors hover:cursor-pointer hover:text-gray-900 focus-visible:outline-2 focus-visible:outline-blue-600" title={title} aria-expanded={!isCollapsed}>
            <ChevronDown className={`size-4 shrink-0 transition-transform duration-200 ${isCollapsed ? "-rotate-90" : ""}`} strokeWidth={2.25} />
            {children}
        </button>
    );
}

function Collapsible(props: { isOpen: boolean; children?: ReactNode }) {
    const { isOpen, children } = props;
    const [isHidden, setIsHidden] = useState(!isOpen);
    const [isExpanded, setIsExpanded] = useState(isOpen);
    const [isSettled, setIsSettled] = useState(isOpen);
    const previousIsOpen = useRef(isOpen);

    useEffect(() => {
        if (previousIsOpen.current === isOpen) return;
        previousIsOpen.current = isOpen;

        setIsSettled(false);

        if (!isOpen) {
            setIsExpanded(false);
            return;
        }

        setIsHidden(false);

        let frame = requestAnimationFrame(() => {
            frame = requestAnimationFrame(() => setIsExpanded(true));
        });

        return () => cancelAnimationFrame(frame);
    }, [isOpen]);

    return (
        <div
            className={`${isHidden ? "hidden" : "grid"} transition-[grid-template-rows,opacity] duration-200 ease-in-out ${isExpanded ? "grid-rows-[1fr] opacity-100" : "grid-rows-[0fr] opacity-0"}`}
            onTransitionEnd={(e) => {
                if (e.target !== e.currentTarget || e.propertyName !== "grid-template-rows") return;

                if (!isOpen) setIsHidden(true);
                else setIsSettled(true);
            }}
        >
            <div className={`min-h-0 ${isSettled ? "" : "overflow-hidden"}`}>{children}</div>
        </div>
    );
}

function WidgetConfiguration(props: WidgetData & { children?: ReactNode; dragHandle: ConnectDragSource; signalCount: number; isCollapsed: boolean; onToggleCollapsed: () => void }) {
    const { id, name, children, dragHandle, signalCount, isCollapsed, onToggleCollapsed } = props;
    const [areSignalsCollapsed, setAreSignalsCollapsed] = useState(false);
    const { removeWidget, updateWidget } = useWidgetManager();

    const nameRef = useRef<HTMLDivElement>(null);

    const deleteSelfWidget = useCallback(() => {
        removeWidget(id);
    }, [id, removeWidget]);

    const updateWidgetName = useCallback(
        (newName: string) => {
            updateWidget(props, (previousWidget) => ({
                ...previousWidget,
                name: newName,
            }));
        },
        [id, updateWidget]
    );

    useEffect(() => {
        if (!nameRef.current || nameRef.current.textContent === name) return;

        nameRef.current.textContent = name;
    }, [name]);

    return (
        <div className="px-6">
            <div className="flex items-center gap-2">
                <CollapseToggle isCollapsed={isCollapsed} onToggle={onToggleCollapsed} title={isCollapsed ? "Expand chart" : "Collapse chart"} />
                <div
                    ref={(node) => {
                        dragHandle(node);
                    }}
                    title="Drag to reorder"
                >
                    <GripVertical className="w-5 h-5 cursor-grab text-gray-500" />
                </div>
                <div className="flex min-w-0 text-sm font-semibold hover:cursor-text items-center">
                    <span
                        ref={nameRef}
                        contentEditable="plaintext-only"
                        autoCorrect="off"
                        role="textbox"
                        className="col-start-1 row-start-1 min-w-0 whitespace-pre text-gray-900 empty:before:content-['Untitled_Chart'] empty:before:opacity-30 focus:outline-none"
                        onInput={(e) => {
                            const newName = e.currentTarget.textContent ?? "";

                            if (newName === "") e.currentTarget.replaceChildren();

                            if (newName === name) return;

                            updateWidgetName(newName);
                        }}
                        onKeyDown={(e) => {
                            if (e.key !== "Enter") return;

                            e.preventDefault();
                            e.currentTarget.blur();
                        }}
                    />
                </div>
                <button
                    onClick={() => {
                        nameRef.current?.focus();

                        setTimeout(() => {
                            const selection = window.getSelection();
                            if (!selection || !nameRef.current) return;

                            const range = document.createRange();
                            range.selectNodeContents(nameRef.current);
                            selection.removeAllRanges();
                            selection.addRange(range);
                        }, 0);
                    }}
                    className="shrink-0 rounded px-2 py-1 text-xs font-medium"
                    title="Edit chart name"
                >
                    <Pencil className="w-4 h-4 text-gray-500" />
                </button>
                <button type="button" onClick={deleteSelfWidget} className="shrink-0 rounded px-2 py-1 text-xs font-medium text-red-700 hover:bg-red-50 focus-visible:outline-2 focus-visible:outline-red-600">
                    Remove chart
                </button>
            </div>
            <Collapsible isOpen={!isCollapsed}>
                <div className="pt-4">
                    <CollapseToggle isCollapsed={areSignalsCollapsed} onToggle={() => setAreSignalsCollapsed((previous) => !previous)} title={areSignalsCollapsed ? "Show signals" : "Hide signals"}>
                        Signals ({signalCount})
                    </CollapseToggle>
                </div>
                <Collapsible isOpen={!areSignalsCollapsed}>
                    <div className="pt-2 flex flex-wrap items-center gap-3">{children}</div>
                </Collapsible>
            </Collapsible>
        </div>
    );
}

function EmptyWidgetState(props: { message: string }) {
    return <div className="mx-6 mt-4 rounded-lg border border-dashed border-gray-300 bg-gray-50 px-6 py-12 text-center text-sm text-gray-500">{props.message}</div>;
}

function AddSignalModal<T extends SignalMetadata>(props: { title: string; description: string; accept: (signal: SignalMetadata) => signal is T; addedSignalNames: string[]; onAdd: (signal: T) => void }) {
    const { title, description, accept, addedSignalNames, onAdd } = props;
    const [modalOpen, setModalOpen] = useState(false);

    return (
        <Dialog open={modalOpen} onOpenChange={setModalOpen}>
            <DialogTrigger asChild>
                <button type="button" className="rounded border border-blue-600 px-3 py-1.5 text-sm font-medium text-blue-700 hover:bg-blue-50 focus-visible:outline-2 focus-visible:outline-blue-600">
                    Add signal
                </button>
            </DialogTrigger>
            <DialogContent className="max-w-2xl">
                <DialogHeader>
                    <DialogTitle className="text-xl">{title}</DialogTitle>
                    <DialogDescription className="text-base text-gray-700">{description}</DialogDescription>
                </DialogHeader>
                <SignalPicker
                    accept={accept}
                    addedSignalNames={addedSignalNames}
                    onAdd={(signals) => {
                        signals.forEach(onAdd);
                        setModalOpen(false);
                    }}
                />
            </DialogContent>
        </Dialog>
    );
}

export const Widget = memo(function Widget(props: WidgetData & { hoveredSignal: RefObject<string | null> }) {
    const { hoveredSignal, id } = props;
    const { widgets, updateWidget, moveWidgetToIndex } = useWidgetManager();
    const draggedAwaySignal = useDraggedAwaySignal(id);
    const [isCollapsed, setIsCollapsed] = useState(false);
    const toggleCollapsed = useCallback(() => setIsCollapsed((previous) => !previous), []);

    const [, drag] = useDrag(
        () => ({
            type: DRAGGABLE_TYPES.WIDGET,
            item: (): WidgetDragItem => ({ id, originalIndex: widgets.findIndex((widget) => widget.id === id) }),
            end: (item, monitor) => {
                if (monitor.didDrop()) return;

                moveWidgetToIndex(item.id, item.originalIndex);
            },
        }),
        [id, widgets, moveWidgetToIndex]
    );

    switch (props.type) {
        case "numericalGraph": {
            const widget = props as NumericalGraphWidgetData;
            const handleRemoveSignal = (signalName: string) => {
                updateWidget(widget, (previousWidget) => ({
                    ...previousWidget,
                    signals: previousWidget.signals.filter((signal) => signal.name !== signalName),
                    options: {
                        ...previousWidget.options,
                        colorPalette: removeColorPaletteEntry(previousWidget.options.colorPalette, signalName),
                    },
                }));
            };

            return (
                <>
                    <WidgetConfiguration {...widget} dragHandle={drag} signalCount={widget.signals.length} isCollapsed={isCollapsed} onToggleCollapsed={toggleCollapsed}>
                        {(draggedAwaySignal ? [...widget.signals, draggedAwaySignal] : widget.signals).map((signal, signalIndex) => (
                            <SignalButton key={signal.name} signal={signal} signalIndex={signalIndex} widgetId={widget.id} isHidden={signal.name === draggedAwaySignal?.name} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name] ?? chroma("#ffffff")} />
                        ))}
                        <AddSignalModal
                            title="Add numerical signal"
                            description="Check the numerical signals to plot on this graph, then add them."
                            accept={isNumericalSignalMetadata}
                            addedSignalNames={widget.signals.map((signal) => signal.name)}
                            onAdd={(signal) =>
                                updateWidget(widget, (previousWidget) => ({
                                    ...previousWidget,
                                    signals: [...previousWidget.signals, signal],
                                    options: { ...previousWidget.options, colorPalette: { ...previousWidget.options.colorPalette, [signal.name]: chroma.random() } },
                                }))
                            }
                        />
                    </WidgetConfiguration>
                    <Collapsible isOpen={!isCollapsed}>{widget.signals.length === 0 ? <EmptyWidgetState message="Add a numerical signal to start graphing live data." /> : <NumericalCanvasChart {...widget} hoveredSignal={hoveredSignal} />}</Collapsible>
                </>
            );
        }
        case "enumTimeline": {
            const widget = props as EnumTimelineWidgetData;
            const handleRemoveSignal = (signalName: string) => {
                updateWidget(widget, (previousWidget) => ({
                    ...previousWidget,
                    signals: previousWidget.signals.filter((signal) => signal.name !== signalName),
                    options: {
                        ...previousWidget.options,
                        colorPalette: removeColorPaletteEntry(previousWidget.options.colorPalette, signalName),
                    },
                }));
            };

            return (
                <>
                    <WidgetConfiguration {...widget} dragHandle={drag} signalCount={widget.signals.length} isCollapsed={isCollapsed} onToggleCollapsed={toggleCollapsed}>
                        {(draggedAwaySignal ? [...widget.signals, draggedAwaySignal] : widget.signals).map((signal, signalIndex) => (
                            <SignalButton key={signal.name} signal={signal} signalIndex={signalIndex} widgetId={widget.id} isHidden={signal.name === draggedAwaySignal?.name} handleRemoveSignal={handleRemoveSignal} hoverSignalName={hoveredSignal} color={widget.options.colorPalette[signal.name]?.color ?? chroma("#ffffff")} />
                        ))}
                        <AddSignalModal
                            title="Add state signal"
                            description="Check the enum or on/off signals to show on this timeline, then add them."
                            accept={isStateSignal}
                            addedSignalNames={widget.signals.map((signal) => signal.name)}
                            onAdd={(signal) =>
                                updateWidget(widget, (previousWidget) => ({
                                    ...previousWidget,
                                    signals: [...previousWidget.signals, signal],
                                    options: { ...previousWidget.options, colorPalette: { ...previousWidget.options.colorPalette, [signal.name]: buildEnumPalette(signal) } },
                                }))
                            }
                        />
                    </WidgetConfiguration>
                    <Collapsible isOpen={!isCollapsed}>{widget.signals.length === 0 ? <EmptyWidgetState message="Add an enum signal to start the timeline." /> : <EnumCanvasChart {...widget} hoveredSignal={hoveredSignal} />}</Collapsible>
                </>
            );
        }
    }
});
