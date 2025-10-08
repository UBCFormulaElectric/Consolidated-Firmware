// TODO(evan): Make this convuluted unmaintainable generic type so this doesn't need to
//             be written manually.
type WidgetTypes = "stateTimline"

type Widget<T extends Record<string, unknown>> = {
    signals: string[];
    type: WidgetTypes,
    options: T;
};

type WidgetRenderer<T extends Record<string, unknown>> = React.FC<Widget<T>>;

export type {
    Widget,
    WidgetTypes
};

export default WidgetRenderer;
