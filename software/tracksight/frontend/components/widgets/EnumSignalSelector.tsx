import { ChevronDown } from "lucide-react";
import React, { RefObject, useRef } from "react";

import SignalSelector from "@/components/SignalSelector";
import useSignalMetadata from "@/lib/hooks/useSignalMetadata";
import { isEnumSignalMetadata, SignalMetadata } from "@/lib/types/Signal";

type EnumSignalSelectorProps = {
  currentSignal: string;
  onSignalChange?: (signalName: string) => void;
};

function EnumSignalSelector(props: EnumSignalSelectorProps) {
  const { currentSignal, onSignalChange } = props;

  const signalMetadata = useSignalMetadata(currentSignal);
  const signalDropdownRef = useRef<HTMLDivElement>(null) as RefObject<HTMLDivElement>;

  if (signalMetadata.isLoading) {
    return <div>Loading...</div>;
  }

  if (signalMetadata.error) {
    return <div>Error loading signal metadata.</div>;
  }

  return (
    <div className="flex flex-row items-center gap-2 text-lg font-medium">
      Signal:
      <div
        className="relative flex flex-row gap-2 text-base font-normal select-none hover:cursor-pointer"
        ref={signalDropdownRef}
      >
        {signalMetadata ? signalMetadata.data?.name : "Select Signal"}
        <ChevronDown size={16} />
        <SignalSelector
          filter={isEnumSignalMetadata}
          selectedSignal={signalMetadata.data || null}
          onSelect={(signal) => {
            if (!onSignalChange) return;

            onSignalChange(signal.name);
          }}
          buttonElement={signalDropdownRef}
        />
      </div>
    </div>
  );
}

export default EnumSignalSelector;
