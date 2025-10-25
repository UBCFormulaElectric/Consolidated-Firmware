import { FC } from "react";

type ChipLegendProps = {
  signals: string[];
  colorPalette: string[];
};

type ChipLegendItemProps = {
    content: string;
    color: string;
};

const ChipLegendItem: FC<ChipLegendItemProps> = (props) => {
  const { content, color } = props;

  return (
    <div
      className="relative flex w-min min-w-36 items-center gap-2 overflow-clip rounded-full border-1 px-2 py-1"
      style={{
        borderColor: color,
      }}
    >
      <div
        className="absolute top-0 right-0 bottom-0 left-0 opacity-20"
        style={{
          backgroundColor: color,
        }}
      />

      <div
        className="size-4 rounded-full"
        style={{
          backgroundColor: color,
        }}
      />

      <p className="text-primary-900">{content}</p>
    </div>
  );
};

const ChipLegend: FC<ChipLegendProps> = (props) => {
  const {
    signals,
    colorPalette,
  } = props;

  return (
    <div
      className="grid w-full max-w-full gap-4 px-8"
      style={{
        gridTemplateColumns: "repeat(auto-fit, 250px)",
      }}
    >
      {signals.map((signal, i) => (
        <ChipLegendItem key={signal} content={signal} color={colorPalette[i] || "#000000"} />
      ))}
    </div>
  );
};

export default ChipLegend;