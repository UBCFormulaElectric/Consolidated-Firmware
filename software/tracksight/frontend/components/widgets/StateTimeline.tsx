import Widget from "@/lib/types/Widget";
import Legend from "@/components/Legend";

const StateTimeline: Widget<"stateTimeline"> = (props) => {
    const {
        signals,
        options
    } = props;

    const {
        colorPalette
    } = options;

    // TODO(evan): Implement this when the renderer is done
    return (
        <div className="flex flex-col gap-5">
            <div className="w-full h-6 relative flex">
                {
                    signals.map((_, i) => (
                        <div className="w-full" style={{
                            backgroundColor: colorPalette[i],
                        }} />
                    ))
                }
            </div>

            <Legend theme="label" signals={signals} colorPalette={colorPalette} />
        </div>
    )
}

export default StateTimeline;
