import Widget from "@/lib/types/Widget";
import getWidgetSchema from "@/lib/getWidgetSchema";
import { useSignalMetadata } from "@/lib/contexts/SignalsContext";

const StateTimelineEditor: Widget<"stateTimeline"> = (props) => {
  const {
    signals,
    options,
    type,
  } = props;

  const signalMetadata = useSignalMetadata(signals[0]);
  const enumOptions = Object.values(signalMetadata?.enum || {});

  const {
    colorPalette
  } = options;

  const widgetSchema = getWidgetSchema(type);

  return (
    <div>
      <div className="text-xl font-medium">
        {widgetSchema.name}
      </div>
    </div>
  )
}

export default StateTimelineEditor;
