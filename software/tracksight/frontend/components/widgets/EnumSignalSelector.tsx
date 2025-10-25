"use client";

import { ChevronDown } from "lucide-react";
import React, { FC, RefObject, useRef } from "react";

import SignalSelector from "@/components/SignalSelector";
import { useSignalMetadata } from "@/lib/contexts/SignalsMetadataContext";
import isEnumSignal from "@/lib/isEnumSignal";

type EnumSignalSelectorProps = {
  currentSignal: string;
  onSignalChange?: (signalName: string) => void;
};

const EnumSignalSelector: FC<EnumSignalSelectorProps> = (props) => {
  const { currentSignal, onSignalChange } = props;

  const signalMetadata = useSignalMetadata(currentSignal);
  const signalDropdownRef = useRef<HTMLDivElement>(null) as RefObject<HTMLDivElement>;

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
            if (!onSignalChange) return;

            onSignalChange(signal.name);
          }}
          buttonElement={signalDropdownRef}
        />
      </div>
    </div>
  );
};

export default EnumSignalSelector;
