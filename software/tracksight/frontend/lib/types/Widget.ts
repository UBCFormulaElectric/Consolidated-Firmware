import {
  ChartData,
  ChartDataEnum,
  ChartDataNumerical,
} from "@/components/widgets/CanvasChartTypes";
import {
  WidgetConfigs,
  WidgetDataEnum,
  WidgetDataNumerical,
} from "@/components/widgets/WidgetTypes";
import type { FC } from "react";
import { SignalType } from "./Signal";

type WidgetSchema =
  | {
      type: "enumTimeline";
      data: ChartDataEnum[];
      options: {
        colorPalette: string[];
      };
    }
  | {
      type: "numericalGraph";
      data: ChartDataNumerical[];
      options: {
        colorPalette: string[];
      };
    };

type WidgetType = WidgetSchema["type"];

type WidgetData<Type extends WidgetType = WidgetType> = {
  signals: WidgetConfigs;
  id: string;
} & Extract<WidgetSchema, { type: Type }>;

type WidgetRendererProps<Type extends WidgetType> = WidgetData<Type>;
type WidgetRenderer<Type extends WidgetType> = FC<WidgetRendererProps<Type>>;

type UnknownWidgetRendererProps = Omit<WidgetRendererProps<WidgetType>, "data">;
type UnknownWidgetRenderer = FC<UnknownWidgetRendererProps>;

type NarrowedWidgetRendererProps<DataType extends SignalType> = Omit<
  Extract<WidgetDataEnum | WidgetDataNumerical, { type: DataType }>,
  "type" | "id"
>;

export type {
  NarrowedWidgetRendererProps,
  UnknownWidgetRenderer,
  UnknownWidgetRendererProps,
  WidgetData,
  WidgetRenderer,
  WidgetRendererProps,
  WidgetType,
};
