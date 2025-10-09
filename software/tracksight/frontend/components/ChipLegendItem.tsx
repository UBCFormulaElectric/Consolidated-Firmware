import type { LegendItemRenderer } from "@/components/Legend";

const ChipLegendItem: LegendItemRenderer = (props) => {
    const {
        content,
        color
    } = props;

    return (
        <div
            className="w-min min-w-36 px-2 py-1 rounded-full border-1 relative overflow-clip flex gap-2 items-center"
            style={{
                borderColor: color,
            }}
        >
            <div
                className="opacity-20 absolute left-0 right-0 top-0 bottom-0"
                style={{
                    backgroundColor: color,
                }}
            />

            <div
                className="rounded-full size-4" 
                style={{
                    backgroundColor: color,
                }}
            />

            <p className="text-primary-900">
                {content}
            </p>
        </div>
    );
}

export default ChipLegendItem;