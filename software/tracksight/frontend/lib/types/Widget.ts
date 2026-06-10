import { LODAwareEnumSeries, LODAwareNumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { Color } from "chroma-js";
import type { FC, RefObject } from "react";
import { BooleanSignalMetadata, EnumSignalMetadata, NumericalSignalMetadata } from "./Signal";

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

export type DiagnosticCardWidgetSchema = {
    type: "diagnosticCard";
    metric: "packet_loss";
    options: {
        label: string;
    };
};

export type WidgetSchema = NumericalGraphWidgetSchema | EnumTimelineWidgetSchema | DiagnosticCardWidgetSchema;

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

export type DiagnosticCardWidgetData = BaseWidgetRenderer & DiagnosticCardWidgetSchema;

type WidgetData = EnumTimelineWidgetData | NumericalGraphWidgetData | DiagnosticCardWidgetData;

type WidgetRendererProps = WidgetData;
type WidgetRenderer = FC<WidgetRendererProps>;

export type { WidgetData, WidgetRenderer, WidgetRendererProps, WidgetType };
