"use client";

import StateTimeline from "@/components/widgets/StateTimeline";
import { WidgetData, WidgetType } from "@/lib/types/Widget";

const Widget = (props: WidgetData<WidgetType>) => {
  switch (props.type) {
    case "stateTimeline":
      return <StateTimeline {...props} />;
  }

  throw new Error(`Should be unreachable: Unsupported widget type: ${(props as any).type}`);
};

export default Widget;
