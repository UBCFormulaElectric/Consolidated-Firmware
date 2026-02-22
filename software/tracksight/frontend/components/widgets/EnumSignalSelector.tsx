import { ChevronDown } from "lucide-react";
import { RefObject, useRef } from "react";

import SignalSelector from "@/components/widgets/SignalSelector";
import { useQueryEnumSignalMetadata } from "@/lib/hooks/useQuerySignalMetadata";

function EnumSignalSelector({ currentSignal, onSignalChange }: {
  currentSignal: string | null;
  onSignalChange?: (signalName: string) => void;
}) {
  const { isPending, error, data } = useQueryEnumSignalMetadata();
  const signalDropdownRef = useRef<HTMLDivElement>(null);

  if (isPending) {
    return <div>Loading...</div>;
  }
  if (error) {
    return <div>Error loading signals</div>;
  }
  return (
    <div className="flex flex-row items-center gap-2 text-lg font-medium">
      Signal:
      <div
        className="relative flex flex-row gap-2 text-base font-normal select-none hover:cursor-pointer"
        ref={signalDropdownRef}
      >
        {currentSignal ?? "Select Signal"}
        <ChevronDown size={16} />
        <SignalSelector signals={data} selectedSignal={currentSignal}
          buttonElement={signalDropdownRef as RefObject<HTMLElement>}
          onSelect={(signal) => {
            if (!onSignalChange) return;
            onSignalChange(signal.name);
          }}
        />
      </div>
    </div>
  );
}

export default EnumSignalSelector;
