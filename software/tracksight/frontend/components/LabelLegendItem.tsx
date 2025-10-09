import type { LegendItemRenderer } from "@/components/Legend";

const LabelLegendItem: LegendItemRenderer = (props) => {
    const {
        content,
        color
    } = props;

    return (
        <div
            className="w-min flex gap-2 items-center"
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

            <p className="text-primary-900 font-extralight">
                {content}
            </p>
        </div>
    );
}

export default LabelLegendItem;