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

const API_BASE_URL = process.env.NEXT_PUBLIC_API_BASE_URL || "http://127.0.0.1:5001/";

export {
    WIDGET_SCHEMAS,
    API_BASE_URL,
};
