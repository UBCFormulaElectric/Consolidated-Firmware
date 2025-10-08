type WidgetProps<T extends Record<string, unknown>> = {
    signals: string[];
    options: T;
};

type Widget<T extends Record<string, unknown>> = React.FC<WidgetProps<T>>;

export default Widget;