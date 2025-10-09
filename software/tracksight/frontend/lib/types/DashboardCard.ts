import { Widget, WIDGET_TYPE } from "@/lib/types/Widget"

type Card = {
    title: string,
    widgets: Widget<WIDGET_TYPE>[],
}

type CardRenderer = React.FC<Card>;

export type {
    Card
}

export default CardRenderer;

