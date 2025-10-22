import StateTimeline from "@/components/widgets/StateTimeline";

const WIDGET_SCHEMAS = [
  {
    type: "stateTimeline",
    name: "Horizontal Stacked Bar",
    options: {
      colorPalette: [] as string[],
    },
    renderer: StateTimeline,
  },
] as const;

export { WIDGET_SCHEMAS };
