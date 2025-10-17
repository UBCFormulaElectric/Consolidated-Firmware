"use client";

import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import getWidgetSchema from "@/lib/getWidgetSchema";
import Widget from "@/lib/types/Widget";
import EnumValueEditor from "@/components/widgets/EnumValueEditor";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";

const StateTimelineEditor: Widget<"stateTimeline"> = (props) => {
  const {
    signals,
    options,
    type,
    editWidget
  } = props;

  const signalMetadata = useSignalMetadata(signals[0]);
  const enumOptions = Object.values(signalMetadata?.enum?.items || {});

  const {
    colorPalette
  } = options;

  const widgetSchema = getWidgetSchema(type);

  return (
    <div className="flex flex-col gap-2">
      <div className="text-xl font-medium">
        {widgetSchema.name}
      </div>
      <div className="ml-2 pl-2 border-l-2 border-gray-200 flex flex-col gap-2">
        <EnumSignalSelector
          currentSignal={signals[0]}
          editWidget={editWidget}
        />

        <EnumValueEditor
          enumOptions={enumOptions}
          colorPalette={colorPalette}
        />
      </div>
    </div>
  )
}

export default StateTimelineEditor;
