"use client";

import EnumTimeline from "@/components/widgets/EnumTimeline";
import { WidgetData, WidgetType } from "@/lib/types/Widget";

const Widget = (props: WidgetData<WidgetType>) => {
  switch (props.type) {
    case "enumTimeline":
      return <EnumTimeline {...props} />;
  }

  throw new Error(`Should be unreachable: Unsupported widget type: ${(props as any).type}`);
};

export default Widget;
