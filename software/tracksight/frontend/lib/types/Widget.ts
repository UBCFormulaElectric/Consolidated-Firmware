import { EnumSeries, LODAwareNumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { Color } from "chroma-js";
import type { FC, RefObject } from "react";
import { EnumSignalMetadata, NumericalSignalMetadata } from "./Signal";

export type EnumTimelineWidgetSchema = {
  type: "enumTimeline";
  data: EnumSeries[];
  options: {
    colorPalette: {
      [signalName: string]: {
        color: Color;
        enumValueColors: {
          [enumValue: number]: Color;
        }
      };
    };
    height: number;
    timeTickCount: number;
  };
}

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
}

export type WidgetSchema =
  | NumericalGraphWidgetSchema
  | EnumTimelineWidgetSchema;

type WidgetType = WidgetSchema["type"];

export type BaseWidgetRenderer = {
  id: string;
  hoveredSignal?: RefObject<string | null>;
  onHoverTimestampChange?: (timestamp: number | null) => void;
};

export type EnumTimelineWidgetData = BaseWidgetRenderer & EnumTimelineWidgetSchema & {
  signals: EnumSignalMetadata[];
}

export type NumericalGraphWidgetData = BaseWidgetRenderer & NumericalGraphWidgetSchema & {
  signals: NumericalSignalMetadata[];
}

type WidgetData = (
  | EnumTimelineWidgetData
  | NumericalGraphWidgetData
);

type WidgetRendererProps = WidgetData;
type WidgetRenderer = FC<WidgetRendererProps>;

export type { WidgetData, WidgetRenderer, WidgetRendererProps, WidgetType };
