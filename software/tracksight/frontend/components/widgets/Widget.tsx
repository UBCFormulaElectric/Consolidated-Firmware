"use client";

import StateTimeline from "@/components/widgets/StateTimeline";
import { WidgetRenderer, WidgetType } from "@/lib/types/Widget";

const Widget: WidgetRenderer<WidgetType> = (props) => {
  const { type } = props;

  if (type === "stateTimeline") {
    return <StateTimeline {...props} />;
  }

  throw new Error(`Unsupported widget type: ${type}`);
};

export default Widget;
