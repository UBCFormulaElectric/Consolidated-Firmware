import { EnumSeries, NumericalSeries } from "@/components/widgets/CanvasChartTypes";
import { Color } from "chroma-js";
import type { FC } from "react";
import { EnumSignalMetadata, NumericalSignalMetadata } from "./Signal";

export type WidgetSchema =
  | {
    type: "enumTimeline";
    data: EnumSeries[];
    options: {
      colorPalette: {
        [signalName: string]: {
          [enumValue: number]: Color;
        };
      };
      height: number;
      timeTickCount: number;
    };
  }
  | {
    type: "numericalGraph";
    data: NumericalSeries[];
    options: {
      colorPalette: {
        [signalName: string]: Color;
      };
      height: number;
      timeTickCount: number;
    };
  };

type WidgetType = WidgetSchema["type"];

type WidgetSchemaOf<T extends WidgetType> = Extract<WidgetSchema, { type: T }>;

type EnumWidgetData = Extract<WidgetSchema, { type: "enumTimeline" }> & {
  signals: EnumSignalMetadata[];
}

type NumericalWidgetData = Extract<WidgetSchema, { type: "numericalGraph" }> & {
  signals: NumericalSignalMetadata[];
}

type WidgetData = (
  EnumWidgetData
  | NumericalWidgetData
) & {
  id: string;
}

type WidgetRendererProps = WidgetData;
type WidgetRenderer = FC<WidgetRendererProps>;

export type { WidgetData, WidgetRenderer, WidgetRendererProps, WidgetSchemaOf, WidgetType };
