"use client";

import { useQuery } from "@tanstack/react-query";
import chroma from "chroma-js";
import { Plus } from "lucide-react";
import { useState } from "react";

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle, DialogTrigger } from "@/components/ui/dialog";
import { buildEnumPalette } from "@/components/widgets/Widget";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { fetchSignalMetadata } from "@/lib/api/signals";
import { API_BASE_URL } from "@/lib/constants";
import { isBooleanSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

const DEFAULT_OPTIONS = { height: 256, timeTickCount: 6 };

export function WidgetAdder() {
    const [open, setOpen] = useState(false);
    const [query, setQuery] = useState("");
    const { appendWidget } = useWidgetManager();
    const {
        data: signals = [],
        isPending,
        isError,
        refetch,
    } = useQuery({
        queryKey: ["available-signals"],
        queryFn: () => fetchSignalMetadata(API_BASE_URL),
        enabled: open,
        staleTime: 5 * 60 * 1000,
    });

    const search = query.trim().toLowerCase();
    const matches = signals.filter((signal) => (isNumericalSignalMetadata(signal) || isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal)) && [signal.name, signal.msg_name, signal.tx_node].some((value) => value.toLowerCase().includes(search))).slice(0, 100);

    const addChart = (signal: SignalMetadata) => {
        if (isNumericalSignalMetadata(signal)) {
            appendWidget({ id: "", type: "numericalGraph", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: chroma.random() } } });
        } else if (isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal)) {
            appendWidget({ id: "", type: "enumTimeline", data: [], signals: [signal], options: { ...DEFAULT_OPTIONS, colorPalette: { [signal.name]: buildEnumPalette(signal) } } });
        }
        setOpen(false);
        setQuery("");
    };

    return (
        <Dialog open={open} onOpenChange={setOpen}>
            <DialogTrigger asChild>
                <button type="button" className="inline-flex items-center gap-2 rounded border border-blue-600 bg-blue-600 px-4 py-2 text-sm font-semibold text-white hover:bg-blue-700 focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600">
                    <Plus className="size-4" /> Add chart
                </button>
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle>Add chart</DialogTitle>
                    <DialogDescription>Choose a signal; the chart type is selected automatically.</DialogDescription>
                </DialogHeader>
                <label htmlFor="new-chart-signal" className="text-sm font-medium">
                    Search signals
                </label>
                <input
                    id="new-chart-signal"
                    value={query}
                    onChange={(event) => setQuery(event.target.value)}
                    onKeyDown={(event) => {
                        if (event.key === "Enter" && matches[0]) addChart(matches[0]);
                    }}
                    autoFocus
                    placeholder="Signal, message, or node"
                    className="rounded border px-3 py-2 text-sm"
                />
                <div className="max-h-72 overflow-y-auto rounded border" aria-label="Matching signals">
                    {isPending ? (
                        <p className="p-3 text-sm text-gray-500">Loading signals…</p>
                    ) : isError ? (
                        <div className="flex items-center justify-between gap-3 p-3 text-sm text-red-700">
                            Could not load signals.{" "}
                            <button type="button" onClick={() => refetch()} className="font-semibold underline">
                                Retry
                            </button>
                        </div>
                    ) : matches.length === 0 ? (
                        <p className="p-3 text-sm text-gray-500">No matching signals.</p>
                    ) : (
                        matches.map((signal) => (
                            <button key={signal.name} type="button" onClick={() => addChart(signal)} className="flex w-full items-center justify-between gap-3 border-b px-3 py-2 text-left text-sm last:border-0 hover:bg-blue-50 focus-visible:bg-blue-50">
                                <span className="min-w-0">
                                    <strong className="block truncate">{signal.name}</strong>
                                    <span className="block truncate text-xs text-gray-500">
                                        {signal.msg_name} · {signal.tx_node}
                                    </span>
                                </span>
                                <span className="shrink-0 text-xs text-gray-500">{isNumericalSignalMetadata(signal) ? "Graph" : "Timeline"}</span>
                            </button>
                        ))
                    )}
                </div>
                {!isPending && !isError && matches.length === 100 && <p className="text-xs text-gray-500">Showing the first 100 matches. Refine your search to see more.</p>}
            </DialogContent>
        </Dialog>
    );
}
