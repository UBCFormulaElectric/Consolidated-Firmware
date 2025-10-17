import StateTimeline from "@/components/widgets/StateTimeline";
import StateTimelineEditor from "@/components/widgets/StateTimelineEditor";

const WIDGET_SCHEMAS = [
    {
        type: "stateTimeline",
        name: "Horizontal Stacked Bar",
        options: {
            colorPalette: [] as string[],
        },
        renderer: StateTimeline,
        editor: StateTimelineEditor,
    }
] as const;

export {
    WIDGET_SCHEMAS,
};
