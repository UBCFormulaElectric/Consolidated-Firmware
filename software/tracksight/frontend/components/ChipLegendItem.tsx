import type { LegendItemRenderer } from "@/components/Legend";

const ChipLegendItem: LegendItemRenderer = (props) => {
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

export default ChipLegendItem;
