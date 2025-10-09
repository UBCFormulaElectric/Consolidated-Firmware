import WidgetRenderer, { WIDGET_TYPE } from "@/lib/types/Widget";
import { WIDGET_SCHEMAS } from '@/lib/constants';

const WIDGET_RENDERERS = Object.fromEntries(
    WIDGET_SCHEMAS.map((schema) => (
        [schema.type, schema.renderer]
    ))
)

const Widget: WidgetRenderer<WIDGET_TYPE> = (props) => {
    const { type } = props;

    const Renderer = WIDGET_RENDERERS[type];

    if (!Renderer) {
        console.error(`Attempted to render widget with non-existant type '${type}'`);

        return;
    }

    return (
        <Renderer
            {...props}
        />
    );
}

export default Widget;