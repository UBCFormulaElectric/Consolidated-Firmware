"use client";

import LabelLegend from "@/components/legends/LabelLegend";
import { WidgetData } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { useSignalDataStore } from "@/lib/contexts/SignalDataStore";
import { useEffect, useRef, useState } from "react";
import { isEnumSignalMetadata } from "@/lib/types/Signal";

function EnumTimeline(props: WidgetData<"enumTimeline">) {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const canvasRef = useRef<HTMLCanvasElement>(null);

  const data = useSignalDataStore(signals[0]);

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
        <canvas
          className="w-full"
          style={{
            backgroundColor: "lightgray",
          }}
          ref={canvasRef}
        ></canvas>
      </div>

      <LabelLegend signals={enumOptions} colorPalette={colorPalette} />
    </div>
  );
}

export default EnumTimeline;
