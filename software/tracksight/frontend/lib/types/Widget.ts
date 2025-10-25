import type { FC } from "react";

type WidgetSchema = {
  type: "stateTimeline",
  name: "Horizontal Stacked Bar",
  options: {
    colorPalette: string[],
  },
};

type WidgetType = WidgetSchema["type"];

type WidgetData<Type extends WidgetType> = {
  signals: string[];
  type: Type;
  options: WidgetSchema["type"] extends Type ? WidgetSchema["options"] : never;
  id: string;
};

type WidgetRenderer<Type extends WidgetType> = FC<WidgetData<Type>>;

export type {
  WidgetData,
  WidgetType,
  WidgetRenderer
};
