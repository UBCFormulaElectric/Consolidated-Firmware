import React from "react";

import { WIDGET_SCHEMAS } from "@/lib/widgetSchemas";

type WIDGET_TYPE = (typeof WIDGET_SCHEMAS)[number]["type"];

type WIDGET_OPTIONS = {
  [A in WIDGET_TYPE]: Extract<(typeof WIDGET_SCHEMAS)[number], { type: A }>["options"];
};

type Widget<Type extends WIDGET_TYPE> = {
  signals: string[];
  type: Type;
  options: WIDGET_OPTIONS[Type];
};

type EditWidgetFunction<Type extends WIDGET_TYPE> = (newWidgetData: Partial<Widget<Type>>) => void;

type WidgetRenderer<Type extends WIDGET_TYPE> = React.FC<Widget<Type>>;

export type { EditWidgetFunction, Widget, WIDGET_OPTIONS, WIDGET_TYPE };

export default WidgetRenderer;
