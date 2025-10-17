import React from 'react';

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

type WidgetRendererProps<Type extends WIDGET_TYPE> = {
  editWidget?: EditWidgetFunction<Type>;
} & Widget<Type>;

type WidgetRenderer<Type extends WIDGET_TYPE> = React.FC<WidgetRendererProps<Type>>;

export type { Widget, WIDGET_TYPE, WIDGET_OPTIONS, EditWidgetFunction };

export default WidgetRenderer;
