import { FC } from "react";

type LabelLegendProps = {
  signals: string[];
  colorPalette: string[];
};

type LabelLegendItemProps = {
  content: string;
  color: string;
};

const LabelLegendItem: FC<LabelLegendItemProps> = (props) => {
  const { content, color } = props;

  return (
    <div
      className="flex w-min items-center gap-2"
      style={{
        borderColor: color,
      }}
    >
      <div
        className="size-4"
        style={{
          backgroundColor: color,
        }}
      />

      <p className="text-primary-900 font-extralight">{content}</p>
    </div>
  );
};

const LabelLegend: FC<LabelLegendProps> = (props) => {
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
        <LabelLegendItem key={signal} content={signal} color={colorPalette[i] || "#000000"} />
      ))}
    </div>
  );
};

export default LabelLegend;
