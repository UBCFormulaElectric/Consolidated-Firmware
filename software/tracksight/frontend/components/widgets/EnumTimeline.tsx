"use client";

import Legend from "@/components/Legend";
import { IS_VERBOSE_DEBUG } from "@/lib/constants";
import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import { useDataVersion, useSignalDataRef } from "@/lib/signalData";
import { WidgetRenderer } from "@/lib/types/Widget";
import { useEffect } from "react";
import EnumSignalSelector from "@/components/widgets/EnumSignalSelector";
import { useDashboardLayout } from "@/lib/contexts/DashboardLayout";

const EnumTimeline: WidgetRenderer<"enumTimeline"> = (props) => {
  const { signals, options } = props;

  const { colorPalette } = options;

  const { editWidget } = useDashboardLayout();

  const signalsDataRef = useSignalDataRef(signals);
  const dataVersion = useDataVersion(signals);

  const signalMetadata = useSignalMetadata(signals[0]);

  const enumOptions = Object.values(signalMetadata?.enum?.items || {});

  useEffect(() => {
    if (!IS_VERBOSE_DEBUG) return;

    console.groupCollapsed("%c[StateTimeline]: Receieved new Data", "color: #81a1c1;");
    console.log("%cSignals: %o", "color: #d08770;", signals);
    console.log("%cData Version: %d", "color: #d08770;", dataVersion);
    console.log("%cSignal Data Ref: %o", "color: #d08770;", signalsDataRef);
    console.groupEnd();
  }, [dataVersion]);

  if (!signalMetadata) return null;

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

      <div className="relative flex h-6 w-full -z-50">
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

export default EnumTimeline;
