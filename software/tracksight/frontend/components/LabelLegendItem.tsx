import type { LegendItemRenderer } from "@/components/Legend";

const LabelLegendItem: LegendItemRenderer = (props) => {
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

export default LabelLegendItem;
