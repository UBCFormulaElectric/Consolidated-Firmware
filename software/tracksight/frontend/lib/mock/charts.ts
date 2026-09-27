import chroma from "chroma-js";

import { buildEnumPalette } from "@/components/widgets/Widget";
import { BooleanSignalMetadata, EnumSignalMetadata, isBooleanSignalMetadata, isEnumSignalMetadata, isNumericalSignalMetadata, NumericalSignalMetadata, SignalMetadata } from "@/lib/types/Signal";
import { WidgetData } from "@/lib/types/Widget";

const CHART_OPTIONS = { height: 256, timeTickCount: 6 };

function pick<T>(items: T[], count: number): T[] {
    const pool = [...items];
    const picked: T[] = [];
    while (picked.length < count && pool.length > 0) picked.push(pool.splice(Math.floor(Math.random() * pool.length), 1)[0]);
    return picked;
}

/** Random charts over the mock catalog: ~70% graphs of 1–3 numerical signals, ~30% timelines of 1–2 enum/boolean signals. */
export function createMockCharts(count: number, catalog: SignalMetadata[]): WidgetData[] {
    const numerical = catalog.filter(isNumericalSignalMetadata) as NumericalSignalMetadata[];
    const states = catalog.filter((signal) => isEnumSignalMetadata(signal) || isBooleanSignalMetadata(signal)) as (EnumSignalMetadata | BooleanSignalMetadata)[];

    return Array.from({ length: count }, (): WidgetData => {
        if (states.length === 0 || (numerical.length > 0 && Math.random() < 0.7)) {
            const signals = pick(numerical, 1 + Math.floor(Math.random() * 3));
            return { id: "", type: "numericalGraph", data: [], signals, options: { ...CHART_OPTIONS, colorPalette: Object.fromEntries(signals.map((signal) => [signal.name, chroma.random()])) } };
        }

        const signals = pick(states, 1 + Math.floor(Math.random() * 2));
        return { id: "", type: "enumTimeline", data: [], signals, options: { ...CHART_OPTIONS, colorPalette: Object.fromEntries(signals.map((signal) => [signal.name, buildEnumPalette(signal)])) } };
    });
}
