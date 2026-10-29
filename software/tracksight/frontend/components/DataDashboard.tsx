"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import { DRAGGABLE_TYPES } from "@/lib/constants";
import { canWidgetAcceptSignal, SignalDragItem, SignalDropResult, WidgetData, WidgetDragItem } from "@/lib/types/Widget";
import { RefObject, useEffect, useLayoutEffect, useRef } from "react";
import { useDragLayer, useDrop, XYCoord } from "react-dnd";
import { Widget } from "./widgets/Widget";
import { useWidgetManager } from "./widgets/WidgetManagerContext";

const SCROLL_SPEED_FACTOR = 2.5;
const SCROLL_THRESHOLD = { TOP: 100, BOTTOM: 50 };

const clamp01 = (value: number) => Math.min(Math.max(value, 0), 1);

const SWAP_ANIMATION: KeyframeAnimationOptions = { duration: 200, easing: "cubic-bezier(0.2, 0, 0, 1)" };

function getTranslateY(node: HTMLElement): number {
    const transform = getComputedStyle(node).transform;
    return transform === "none" ? 0 : new DOMMatrix(transform).m42;
}

function getDropIndex(list: HTMLDivElement | null, pointer: XYCoord | null): number | null {
    if (!list || !pointer) return null;

    const pointerY = pointer.y - list.getBoundingClientRect().top;
    const children = Array.from(list.children) as HTMLElement[];
    const index = children.findIndex((child) => {
        const midpoint = child.offsetTop + child.offsetHeight / 2;

        return pointerY < midpoint;
    });

    return index === -1 ? children.length : index;
}

const getFirstScrollableAncestor = (node: HTMLElement): HTMLElement | null => {
    let current = node;

    while (current) {
        if (current.scrollHeight > current.clientHeight) return current;
        if (current === document.documentElement) break;
        if (current.parentElement === null) break;

        current = current.parentElement;
    }

    return null;
};

function DashboardWidgetSlot(props: { widget: WidgetData; stackOrder: number; isDraggedWidget: boolean; hoveredSignal: RefObject<string | null>; itemRefs: RefObject<Map<string, HTMLDivElement>> }) {
    const { widget, stackOrder, isDraggedWidget, hoveredSignal, itemRefs } = props;
    const { moveSignal } = useWidgetManager();

    const [, drop] = useDrop(
        () => ({
            accept: DRAGGABLE_TYPES.SIGNAL,
            canDrop: (item: SignalDragItem) => item.currentWidgetId === widget.id || canWidgetAcceptSignal(widget, item.signal),
            drop: (): SignalDropResult => ({ isAccepted: true }),
            hover: (item: SignalDragItem, monitor) => {
                if (item.currentWidgetId === widget.id) return;
                if (!monitor.canDrop()) return;

                moveSignal(item.signal.name, item.currentWidgetId, widget.id);
                item.currentWidgetId = widget.id;
            },
        }),
        [widget, moveSignal]
    );

    return (
        <div
            ref={(node) => {
                drop(node);

                if (node) itemRefs.current.set(widget.id, node);
                else itemRefs.current.delete(widget.id);
            }}
            className={`sticky left-0 w-full ${isDraggedWidget ? "opacity-50" : ""}`}
            style={{ zIndex: stackOrder }}
        >
            <Widget {...widget} hoveredSignal={hoveredSignal} />
        </div>
    );
}

function DataDashboard(props: { emptyMessage: string }) {
    const { widgets, moveWidget } = useWidgetManager();

    const hoveredSignal = useRef<string | null>(null);
    const listRef = useRef<HTMLDivElement | null>(null);
    const itemRefs = useRef(new Map<string, HTMLDivElement>());
    const previousTops = useRef(new Map<string, number>());

    useLayoutEffect(() => {
        const nextTops = new Map<string, number>();

        itemRefs.current.forEach((node, id) => {
            const top = node.offsetTop;
            nextTops.set(id, top);

            const previousTop = previousTops.current.get(id);
            if (previousTop === undefined || previousTop === top) return;

            const visualTop = previousTop + getTranslateY(node);
            node.getAnimations().forEach((animation) => animation.cancel());
            node.animate([{ transform: `translateY(${visualTop - top}px)` }, { transform: "translateY(0)" }], SWAP_ANIMATION);
        });

        previousTops.current = nextTops;
    }, [widgets]);

    const [{ draggedWidgetId }, drop] = useDrop(
        () => ({
            accept: [DRAGGABLE_TYPES.WIDGET, DRAGGABLE_TYPES.SIGNAL],
            hover: (item: WidgetDragItem, monitor) => {
                if (monitor.getItemType() !== DRAGGABLE_TYPES.WIDGET) return;

                const index = getDropIndex(listRef.current, monitor.getClientOffset());

                if (index === null) return;

                moveWidget(item.id, index);
            },
            collect: (monitor) => ({
                draggedWidgetId: monitor.getItemType() === DRAGGABLE_TYPES.WIDGET ? (monitor.getItem() as WidgetDragItem).id : null,
            }),
        }),
        [moveWidget]
    );

    const { isDragging } = useDragLayer((monitor) => ({
        isDragging: monitor.isDragging() && (monitor.getItemType() === DRAGGABLE_TYPES.WIDGET || monitor.getItemType() === DRAGGABLE_TYPES.SIGNAL),
    }));

    useEffect(() => {
        const handleMouseMove = (event: MouseEvent) => {
            if (!isDragging || !listRef.current) return;

            const scrollableAncestor = getFirstScrollableAncestor(listRef.current);

            if (!scrollableAncestor) return;

            const rect = scrollableAncestor.getBoundingClientRect();

            const topProximity = clamp01(1 - (event.clientY - rect.top) / SCROLL_THRESHOLD.TOP);
            const bottomProximity = clamp01(1 - (rect.bottom - event.clientY) / SCROLL_THRESHOLD.BOTTOM);

            if (topProximity === 0 && bottomProximity === 0) return;

            const scrollSpeed = bottomProximity - topProximity;
            scrollableAncestor.scrollTop += scrollSpeed * SCROLL_SPEED_FACTOR;
            event.preventDefault();
            event.stopImmediatePropagation();
        };

        document.addEventListener("dragover", handleMouseMove);
        return () => {
            document.removeEventListener("dragover", handleMouseMove);
        };
    }, [isDragging]);

    return (
        <section
            ref={(node) => {
                drop(node);
            }}
            aria-labelledby="charts-heading"
            className="w-full h-fit"
        >
            <div className="mb-6 flex items-baseline gap-2 border-b border-gray-200 px-6 pt-5 pb-2">
                <h2 id="charts-heading" className="text-xs font-semibold tracking-wide text-gray-600 uppercase">
                    Charts
                </h2>
                {widgets.length > 0 && <span className="text-xs text-gray-500">{widgets.length}</span>}
            </div>
            {widgets.length === 0 ? (
                <div className="mx-6 flex flex-col items-center gap-4 rounded-lg border border-dashed border-gray-300 px-6 py-12 text-sm text-gray-500">
                    <p>{props.emptyMessage}</p>
                    <WidgetAdder />
                </div>
            ) : (
                <>
                    <div ref={listRef} className="relative flex h-full min-w-full flex-col gap-16 py-3">
                        {widgets.map((widget, index) => (
                            <DashboardWidgetSlot key={widget.id} widget={widget} stackOrder={widgets.length - index} isDraggedWidget={widget.id === draggedWidgetId} hoveredSignal={hoveredSignal} itemRefs={itemRefs} />
                        ))}
                    </div>
                    <div className="flex justify-center py-8">
                        <WidgetAdder />
                    </div>
                </>
            )}
        </section>
    );
}

export default DataDashboard;
