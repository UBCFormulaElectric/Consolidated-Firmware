"use client";

import { ChevronDown } from "lucide-react";
import React, { useRef } from "react";

import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import isEnumSignal from "@/lib/isEnumSignal";
import { EditWidgetFunction, WIDGET_TYPE } from "@/lib/types/Widget";
import SignalSelector from "@/components/SignalSelector";

type EnumSignalSelectorProps = {
  currentSignal: string;
  editWidget: EditWidgetFunction<WIDGET_TYPE> | undefined;
};

const EnumSignalSelector: React.FC<EnumSignalSelectorProps> = (props) => {
  const { currentSignal, editWidget } = props;

  const signalMetadata = useSignalMetadata(currentSignal);
  const signalDropdownRef = useRef<HTMLDivElement>(null) as React.RefObject<HTMLDivElement>;

  return (
    <div className="flex flex-row items-center gap-2 text-lg font-medium">
      Signal:
      <div
        className="relative flex flex-row gap-2 text-base font-normal select-none hover:cursor-pointer"
        ref={signalDropdownRef}
      >
        {signalMetadata ? signalMetadata.name : "No signal selected"}
        <ChevronDown size={16} />
        <SignalSelector
          filter={isEnumSignal}
          selectedSignal={signalMetadata || null}
          onSelect={(signal) => {
            if (!editWidget) return;

            editWidget({
              signals: [signal.name],
            });
          }}
          buttonElement={signalDropdownRef}
        />
      </div>
    </div>
  );
};

export default EnumSignalSelector;
