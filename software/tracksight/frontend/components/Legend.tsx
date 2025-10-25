import ChipLegendRenderer from "@/components/ChipLegendItem";
import LabelLegendItem from "@/components/LabelLegendItem";
import React, { FC, useMemo } from "react";

type LegendThemes = "chip" | "label";

type LegendProps = {
  theme: LegendThemes;

  // TODO(evan): Implement editable legends
  onAddSignal?: (signal: string) => void;
  onRemoveSignal?: (signal: string) => void;

  signals: string[];
  colorPalette: string[];
};

type LegendItemRendererProps = {
  content: string;
  color: string;

  removable?: boolean;
  onRemoved?: () => {};
};

export type LegendItemRenderer = FC<LegendItemRendererProps>;

const LEGEND_ITEM_RENDERERS: Record<LegendThemes, LegendItemRenderer> = {
  chip: ChipLegendRenderer,
  label: LabelLegendItem,
};

const Legend: FC<LegendProps> = (props) => {
  const {
    theme,

    signals,
    colorPalette,
  } = props;

  const ItemRenderer = useMemo(() => LEGEND_ITEM_RENDERERS[theme], [theme]);

  return (
    <div
      className="grid w-full max-w-full gap-4 px-8"
      style={{
        gridTemplateColumns: "repeat(auto-fit, 250px)",
      }}
    >
      {signals.map((signal, i) => (
        <ItemRenderer key={signal} content={signal} color={colorPalette[i] || "#000000"} />
      ))}
    </div>
  );
};

export default Legend;
