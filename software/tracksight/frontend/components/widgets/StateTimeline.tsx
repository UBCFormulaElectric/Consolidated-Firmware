"use client";

import Widget from "@/lib/types/Widget";
import Legend from "@/components/Legend";
import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import { useDataVersion, useSignalDataRef } from "@/lib/signalData";

const StateTimeline: Widget<"stateTimeline"> = (props) => {
  const { signals, options } = props;

  const { colorPalette } = options;

  const signalDataRef = useSignalDataRef(signals[0]);
  const dataVersion = useDataVersion(signals);

  const signalMetadata = useSignalMetadata(signals[0]);

  if (!signalMetadata) return;

  const enumOptions = Object.values(signalMetadata?.enum?.items || {});

  // TODO(evan): Implement this when the renderer is done
  return (
    <div className="flex flex-col gap-5">
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

      <Legend theme="label" signals={enumOptions} colorPalette={colorPalette} />
    </div>
  );
};

export default StateTimeline;
