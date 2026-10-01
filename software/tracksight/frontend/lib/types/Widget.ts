import { LODAwareEnumSeries, LODAwareNumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { Color } from "chroma-js";
import type { FC, RefObject } from "react";
import { BooleanSignalMetadata, EnumSignalMetadata, NumericalSignalMetadata, SignalMetadata, SignalType } from "./Signal";

export type EnumTimelineWidgetSchema = {
    type: "enumTimeline";
    data: LODAwareEnumSeries[];
    options: {
        colorPalette: {
            [signalName: string]: {
                color: Color;
                enumValueColors: {
                    [enumValue: number]: Color;
                };
            };
        };
        height: number;
        timeTickCount: number;
    };
};

export type NumericalGraphWidgetSchema = {
    type: "numericalGraph";
    data: LODAwareNumericalSeries[];
    options: {
        colorPalette: {
            [signalName: string]: Color;
        };
        height: number;
        timeTickCount: number;
    };
};

export type WidgetSchema = NumericalGraphWidgetSchema | EnumTimelineWidgetSchema;

type WidgetType = WidgetSchema["type"];

export type BaseWidgetRenderer = {
    id: string;
    hoveredSignal?: RefObject<string | null>;
    onHoverTimestampChange?: (timestamp: number | null) => void;
};

export type EnumTimelineWidgetData = BaseWidgetRenderer &
    EnumTimelineWidgetSchema & {
        signals: (EnumSignalMetadata | BooleanSignalMetadata)[];
    };

export type NumericalGraphWidgetData = BaseWidgetRenderer &
    NumericalGraphWidgetSchema & {
        signals: NumericalSignalMetadata[];
    };

type WidgetData = EnumTimelineWidgetData | NumericalGraphWidgetData;

export const WIDGET_ACCEPTED_SIGNAL_TYPES: Record<WidgetType, SignalType[]> = {
    numericalGraph: [SignalType.NUMERICAL],
    enumTimeline: [SignalType.ENUM, SignalType.BOOLEAN],
};

export type WidgetDragItem = {
    id: string;
};

export type SignalDragItem = {
    signal: SignalMetadata;
    currentWidgetId: string;
};

export const canWidgetAcceptSignal = (widget: WidgetData, signal: SignalMetadata): boolean => {
    if (!WIDGET_ACCEPTED_SIGNAL_TYPES[widget.type].includes(signal.type)) return false;

    return !widget.signals.some((existingSignal) => existingSignal.name === signal.name);
};

type WidgetRendererProps = WidgetData;
type WidgetRenderer = FC<WidgetRendererProps>;

export type { WidgetData, WidgetRenderer, WidgetRendererProps, WidgetType };
