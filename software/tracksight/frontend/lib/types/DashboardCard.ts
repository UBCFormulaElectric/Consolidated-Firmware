import { Widget } from "./Widget"

type Card = {
    title: string,
    widgets: Widget<any>[],
}

type CardRenderer = React.FC<Card>;

export type {
    Card
}

export default CardRenderer;

