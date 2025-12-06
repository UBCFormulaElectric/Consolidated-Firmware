"use client";

import LabelLegend from "@/components/legends/LabelLegend";
import { WidgetData } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { useSignalDataStore } from "@/lib/contexts/SignalDataStore";
import { useEffect, useState } from "react";
import { isEnumSignalMetadata } from "@/lib/types/Signal";

function EnumTimeline(props: WidgetData<"enumTimeline">) {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const [dataVersion, setDataVersion] = useState(0);
  const data = useSignalDataStore(signals[0]);

  useEffect(() => {
    setInterval(() => {
      setDataVersion((prev) => prev + 1);
    }, 100);
  }, []);

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

  if (!isEnumSignalMetadata(signalMetadata.data)) {
    return (
      <div className="flex flex-col items-center">
        Invalid signal type.
        <p className="p-4 text-center text-sm">
          The signal "{signals[0]}" is not an enum signal. Please select a valid enum signal.
        </p>
      </div>
    );
  }

  const enumOptions = Object.values(signalMetadata.data.enum.items);

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
        {
          // NOTE(evan): This is just here for my personal testing ignore how horrible
          Array.from({ length: data.data.values.size() }, (_, i) => {
            const previousPoint = data.data.values.get(i - 1);
            const lengthPerElement = 100 / Math.max(data.data.values.size(), 1);
            let widthPercent = lengthPerElement;

            if (previousPoint === undefined || previousPoint != data.data.values.get(i)) {
              let lengthOfSegment = 1;

              while (data.data.values.get(i + lengthOfSegment) === data.data.values.get(i)) {
                lengthOfSegment += 1;
              }

              widthPercent = lengthOfSegment * lengthPerElement;
            } else {
              return null;
            }

            const value = data.data.values.get(i);

            if (value === undefined) return null;

            const enumOption = enumOptions[value];
            const color = colorPalette[value % colorPalette.length];

            return (
              <div
                key={i}
                className="h-full"
                style={{
                  width: `${widthPercent}%`,
                  backgroundColor: color,
                  display: "inline-block",
                }}
              />
            );
          })
        }
      </div>

      <LabelLegend signals={enumOptions} colorPalette={colorPalette} />
    </div>
  );
}

export default EnumTimeline;
