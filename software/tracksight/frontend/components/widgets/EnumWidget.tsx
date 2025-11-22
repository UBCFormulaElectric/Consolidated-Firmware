"use client";

import LabelLegend from "@/components/legends/LabelLegend";
import { WidgetDataEnum } from "@/lib/types/Widget";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { useMemo } from "react";
import { EnumSignalMetadata, SignalType } from "@/lib/types/Signal";

function EnumWidgetBody({ signalName, setSignalName, signalMetadata, colorPalette }: {
  setSignalName: (name: string) => void;
  colorPalette: string[];
} & ({
  signalName: null;
  signalMetadata: null;
} | {
  signalName: string;
  signalMetadata: EnumSignalMetadata;
})) {
  const signals = useMemo(() => signalMetadata ? Object.values(signalMetadata.enum.items) : [], [signalMetadata])

  return (
    <div className="flex flex-col gap-5">
      <div className="px-8">
        <EnumSignalSelector currentSignal={signalName} onSignalChange={setSignalName} />
      </div>
      <LabelLegend signals={signals} colorPalette={colorPalette} />
      {/* TODO put the canvas here */}
    </div>
  );
}

export default function EnumWidget({ widgetData: { signal, options: { colorPalette } }, setEnumSignal }: {
  widgetData: WidgetDataEnum
  setEnumSignal: (newSignal: string) => void;
}) {
  const { isPending: signalMetadataLoading, error: signalMetadataError, data: signalMetadata } = useSignalMetadata(signal);

  if (signalMetadataLoading) {
    return (
      <div className="flex flex-col gap-5">
        <div className="px-8">
          Loading...
        </div>
        <div className="relative flex h-6 w-full">
          <div className="w-full" style={{ backgroundColor: "lightgray", }} />
        </div>
      </div>
    );
  }
  if (signalMetadataError) {
    return (
      <div className="flex flex-col items-center">
        Error loading signal metadata.
        <p className="p-4 text-center text-sm">
          Signal metadata not found for signal "{signal}". Please select a valid enum signal.
        </p>
      </div>
    )
  }
  if (signal === null || signalMetadata === null) {
    return <EnumWidgetBody
      signalName={null}
      setSignalName={setEnumSignal}
      signalMetadata={null}
      colorPalette={colorPalette} />
  }
  if (signalMetadata.type !== SignalType.ENUM) {
    return (
      <div className="flex flex-col items-center">
        Error: Signal metadata type is not Enum. THIS SHOULD NEVER HAPPEN
      </div>
    );
  }
  return <EnumWidgetBody
    signalName={signal}
    setSignalName={setEnumSignal}
    signalMetadata={signalMetadata}
    colorPalette={colorPalette} />
}
