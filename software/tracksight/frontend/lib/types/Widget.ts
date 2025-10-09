import { WIDGET_SCHEMAS } from "@/lib/constants";

type WIDGET_TYPE = (
    (typeof WIDGET_SCHEMAS)[number]["type"]
);

type WIDGET_OPTIONS = {
    [A in WIDGET_TYPE]: Extract<(typeof WIDGET_SCHEMAS)[number], { type: A }>['options'];
};

type Widget<Type extends WIDGET_TYPE> = {
    signals: string[];
    type: Type,
    options: WIDGET_OPTIONS[Type];
};

type WidgetRenderer<Type extends WIDGET_TYPE> = React.FC<Widget<Type>>;

export type {
    Widget,
    WIDGET_TYPE,
    WIDGET_OPTIONS
};

export default WidgetRenderer;
