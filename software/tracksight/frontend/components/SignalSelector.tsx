"use client";

import { RefObject, useMemo } from "react";

import Selector from "@/components/common/Selector";
import { SignalMetadata } from "@/lib/types/Signal";

function SignalItemRenderer({ data }: { data: SignalMetadata; isSelected?: boolean }) {
  // TODO(evan): Add styling for selected state
  return (
    <div className="flex gap-2">
      <p>{data.name}</p>
      {/* {data.unit && <p className="text-gray-500">({data.unit})</p>} */}
    </div>
  );
}

function SignalSelector({ signals, selectedSignal, onSelect, buttonElement }: {
  signals: SignalMetadata[];
  selectedSignal: string | null;
  onSelect: (signal: SignalMetadata) => void;
  buttonElement: RefObject<HTMLElement>;
}) {
  const options = useMemo(() => {
    const groups = new Map<string, SignalMetadata[]>();
    for (const sig of signals) {
      const sender = sig.tx_node || "Unknown TX Node";
      const arr = groups.get(sender);
      if (arr) arr.push(sig);
      else groups.set(sender, [sig]);
    }
    // Sort groups by label and signals by name for stable ordering
    return Array.from(groups.entries())
      .sort(([a], [b]) => a.localeCompare(b))
      .map(([label, next]) =>
      ({
        label,
        next: next.slice().sort((x, y) => x.name.localeCompare(y.name)) as any
      }));
  }, [signals]);

  const selectedOption = useMemo(() =>
    selectedSignal
      ? signals.find((signal) => signal.name === selectedSignal) || null
      : null, [selectedSignal, signals]
  );

  return (
    <Selector
      options={options}
      selectedOption={selectedOption}
      onSelect={onSelect}
      ItemRenderer={SignalItemRenderer}
      getSearchableText={(signal) => signal.name}
      buttonElement={buttonElement}
    />
  );
}

export default SignalSelector;
