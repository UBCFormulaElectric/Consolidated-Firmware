"use client";

import { WidgetAdder } from "@/app/live/WidgetAdder";
import { useRef } from "react";
import { Widget } from "./widgets/Widget";
import { useWidgetManager } from "./widgets/WidgetManagerContext";

function DataDashboard(props: { emptyMessage: string }) {
    const { widgets } = useWidgetManager();

    const hoveredSignal = useRef<string | null>(null);

    return (
        <section aria-labelledby="charts-heading" className="w-full h-fit">
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
                    <div className="flex h-full min-w-full flex-col gap-16">
                        {widgets.map((widget, index) => (
                            <div key={widget.id} className="sticky left-0 w-full" style={{ zIndex: widgets.length - index }}>
                                <Widget {...widget} hoveredSignal={hoveredSignal} />
                            </div>
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
