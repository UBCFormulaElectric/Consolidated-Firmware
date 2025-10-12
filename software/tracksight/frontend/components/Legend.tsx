import { useMemo } from "react";
import ChipLegendRenderer from "@/components/ChipLegendItem";
import LabelLegendItem from "@/components/LabelLegendItem";

type LegendThemes = 'chip' | 'label';

type LegendProps = {
    theme: LegendThemes;

    editable?: boolean;
    onAddSignal?: (signal: string) => void;
    onRemoveSignal?: (signal: string) => void;
    
    signals: string[];
    colorPalette: string[];
}

type LegendItemRendererProps = {
    content: string;
    color: string;

    removable?: boolean;
    onRemoved?: () => {};
}

export type LegendItemRenderer = React.FC<LegendItemRendererProps>;

const LEGEND_ITEM_RENDERERS: Record<LegendThemes, LegendItemRenderer> = {
    chip: ChipLegendRenderer,
    label: LabelLegendItem,
}

const Legend: React.FC<LegendProps> = (props) => {
    const {
        theme,
        
        signals,
        colorPalette,
    } = props;

    const ItemRenderer = useMemo(() => LEGEND_ITEM_RENDERERS[theme], [theme]);

    return (
        <div
            className="grid w-full max-w-full gap-4 justify-evenly"
            style={{
            gridTemplateColumns: "repeat(auto-fit, 200px)",
            }}
        >
            {signals.map((signal, i) => (
            <ItemRenderer
                key={signal}
                content={signal}
                color={colorPalette[i] || "#000000"}
            />
            ))}
        </div>
    )
};

export default Legend;

