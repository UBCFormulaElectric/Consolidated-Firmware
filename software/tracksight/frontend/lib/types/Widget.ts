import type { FC } from "react";

type WidgetSchema =
  | {
      type: "enumTimeline";
      options: {
        colorPalette: string[];
      };
    }
  | {
      type: "alertTimeline";
      options: {
        colorPalette: string[];
      };
    };

type WidgetType = WidgetSchema["type"];

type WidgetData<Type extends WidgetType = WidgetType> = {
  signals: string[];
  id: string;
} & Extract<WidgetSchema, { type: Type }>;

type WidgetRenderer<Type extends WidgetType> = FC<WidgetData<Type>>;

export type { WidgetData, WidgetRenderer, WidgetType };
