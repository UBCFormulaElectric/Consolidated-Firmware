type ChipLegendProps = {
  signals: string[];
  colorPalette: string[];
};

type ChipLegendItemProps = {
    content: string;
    color: string;
};

function ChipLegendItem(props: ChipLegendItemProps) {
  const { content, color } = props;

  return (
    <div
      className="relative flex w-min items-center gap-2 overflow-clip rounded-full border-1 px-2 py-1"
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
}

function ChipLegend(props: ChipLegendProps) {
  const {
    signals,
    colorPalette,
  } = props;

  return (
    <div className="flex flex-wrap w-full max-w-full gap-8 px-8">
      {signals.map((signal, i) => (
        <ChipLegendItem key={signal} content={signal} color={colorPalette[i] || "#000000"} />
      ))}
    </div>
  );
}

export default ChipLegend;