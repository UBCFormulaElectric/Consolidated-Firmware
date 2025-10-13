import Widget from "@/lib/types/Widget";
import getWidgetSchema from "@/lib/getWidgetSchema";
import { useSignalMetadata } from "@/lib/contexts/SignalsContext";
import SignalSelector from "../SignalSelector";
import isEnumSignal from "@/lib/isEnumSignal";

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
    <div className="flex flex-col gap-2">
      <div className="text-xl font-medium">
        {widgetSchema.name}
      </div>
      <div className="ml-2 pl-2 border-l-2 border-gray-200">
        <div className="font-medium text-lg flex flex-row gap-2 items-center">
          Signal:
          <SignalSelector
            filter={isEnumSignal}
            selectedSignal={signalMetadata || null}
            onSelect={(signal) => {
              console.log("Selected signal:", signal);
            }}
          />
        </div>
      </div>
    </div>
  )
}

export default StateTimelineEditor;
