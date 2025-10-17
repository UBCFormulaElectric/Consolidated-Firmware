"use client";

import { useRef } from "react";
import { ChevronDown } from "lucide-react";

import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import { EditWidgetFunction, WIDGET_TYPE } from "@/lib/types/Widget";
import isEnumSignal from "@/lib/isEnumSignal";
import SignalSelector from "../SignalSelector";

type EnumSignalSelectorProps = {
  currentSignal: string;
  editWidget: EditWidgetFunction<WIDGET_TYPE> | undefined;
}

const EnumSignalSelector: React.FC<EnumSignalSelectorProps> = (props) => {
  const {
    currentSignal,
    editWidget
  } = props;

  const signalMetadata = useSignalMetadata(currentSignal);
  const signalDropdownRef = useRef<HTMLDivElement>(null) as React.RefObject<HTMLDivElement>;

  return (
    <div className="font-medium text-lg flex flex-row gap-2 items-center">
      Signal:
      <div className="font-normal text-base flex flex-row gap-2 relative select-none hover:cursor-pointer" ref={signalDropdownRef}>
        {signalMetadata ? signalMetadata.name : "No signal selected"}
        <ChevronDown size={16} />
        <SignalSelector
          filter={isEnumSignal}
          selectedSignal={signalMetadata || null}
          onSelect={(signal) => {
            if (!editWidget) return;

            editWidget({
              signals: [signal.name]
            })
          }}
          buttonElement={signalDropdownRef}
        />
      </div>
    </div>
  );
}

export default EnumSignalSelector;
