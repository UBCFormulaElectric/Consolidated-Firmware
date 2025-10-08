import WidgetRenderer, { WidgetTypes } from "@/lib/types/Widget";
import StateTimeline from "./StateTimeline";

const WIDGET_RENDERERS: Record<WidgetTypes, WidgetRenderer<any>> = {
    stateTimline: StateTimeline,
};

const Widget: WidgetRenderer<any> = (props) => {
    const { type } = props;

    const Renderer = WIDGET_RENDERERS[type];

    if (!Renderer) {
        console.error(`Attempted to render widget with non-existant type '${type}'`)

        return;
    }

    return (
        <Renderer
            {...props}
        />
    );
}

export default Widget;