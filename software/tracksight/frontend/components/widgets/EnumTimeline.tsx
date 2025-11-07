"use client";

import LabelLegend from "@/components/legends/LabelLegend";
import { WidgetRenderer } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";

const EnumTimeline: WidgetRenderer<"enumTimeline"> = (props) => {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const signalMetadata = useSignalMetadata(signals[0] ?? "");

  if (signalMetadata.isLoading) {
    return (
      <div className="flex flex-col gap-5">
        <div className="px-8">
          Loading...
        </div>

        <div className="relative flex h-6 w-full">
          <div
            className="w-full"
            style={{
              backgroundColor: "lightgray",
            }}
          />
        </div>
      </div>
    );
  }

  if (!signalMetadata.data || signalMetadata.isError) {
    return (
      <div className="flex flex-col items-center">
        Error loading signal metadata.

        <p className="p-4 text-center text-sm">
          Signal metadata not found for signal "{signals[0]}". Please select a valid enum signal.
        </p>
      </div>
    )
  }

  const enumOptions = Object.values(signalMetadata.data.enum?.items || {});

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
