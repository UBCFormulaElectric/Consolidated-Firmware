"use client";

import EnumTimeline from "@/components/widgets/EnumTimeline";
import { WidgetData, WidgetType } from "@/lib/types/Widget";
import AlertTimeline from "@/components/widgets/AlertTimeline";

function Widget(props: WidgetData<WidgetType>) {
  switch (props.type) {
    case "enumTimeline":
      return <EnumTimeline {...props} />;
    case "alertTimeline":
      return <AlertTimeline {...props} />;
  }

  throw new Error(`Should be unreachable: Unsupported widget type: ${(props as any).type}`);
}

export default Widget;
