"use client";

import LabelLegend from "@/components/legends/LabelLegend";
import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import { WidgetRenderer } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";

const EnumTimeline: WidgetRenderer<"enumTimeline"> = (props) => {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const signalMetadata = useSignalMetadata(signals[0]);

  const enumOptions = Object.values(signalMetadata?.enum?.items || {});

  if (!signalMetadata) {
    return (
      <div className="flex flex-col items-center">
        <p className="p-4 text-center text-sm">
          Signal metadata not found.
        </p>
      </div>
    )
  }

  // TODO(evan): Implement this when the renderer is done
  return (
    <div className="flex flex-col gap-5">
      <div className="px-8">
        <EnumSignalSelector
          currentSignal={signals[0]}
          onSignalChange={(newSignal) => {
            editWidget(props.id, {
              signals: [newSignal],
            });
          }}
        />
      </div>

      <div className="relative flex h-6 w-full">
        {signals.map((_, i) => (
          <div
            className="w-full"
            style={{
              backgroundColor: colorPalette[i],
            }}
          />
        ))}
      </div>

      <LabelLegend signals={enumOptions} colorPalette={colorPalette} />
    </div>
  );
};

export default EnumTimeline;
