"use client";

import React, { RefObject } from "react";

import Selector from "@/components/common/Selector";
import { SignalMetadata } from "@/lib/types/Signal";
import useSignalMetadataList from "@/lib/hooks/useSignalMetadataList";

type SignalSelectorProps = {
  filter: (signal: SignalMetadata) => boolean;

  selectedSignal: SignalMetadata | null;
  onSelect: (signal: SignalMetadata) => void;

  buttonElement: RefObject<HTMLElement>;
};

function SignalItemRenderer(props: { data: SignalMetadata; isSelected?: boolean }) {
  const { data } = props;

  // TODO(evan): Add styling for selected state

  return (
    <div className="flex gap-2">
      <p>{data.name}</p>
      {data.unit && <p className="text-gray-500">({data.unit})</p>}
    </div>
  );
}

function SignalSelector(props: SignalSelectorProps) {
  const { filter, selectedSignal, onSelect, buttonElement } = props;

  const signalMetadata = useSignalMetadataList("*");

  if (signalMetadata.isLoading || !signalMetadata.data) {
    return <div>Loading...</div>;
  }

  const filteredSignals = Array.from(signalMetadata.data.values().filter(filter));

  // TODO(evan): Fetch this from the backend when the api route is ready
  const signalCategories = filteredSignals.reduce(
    (categories, signal) => {
      const category = signal.tx_node || "Uncategorized";
      if (!categories[category]) {
        categories[category] = [];
      }

      categories[category].push(signal);
      return categories;
    },
    {} as Record<string, SignalMetadata[]>
  );

  const options = Object.entries(signalCategories).map(([category, signals]) => ({
    label: category,
    next: signals as any,
  }));

  return (
    <Selector
      options={options}
      selectedOption={selectedSignal}
      onSelect={onSelect}
      ItemRenderer={SignalItemRenderer}
      getSearchableText={(signal) => `${signal.name} ${signal.unit || ""}`}
      buttonElement={buttonElement}
    />
  );
}

export default SignalSelector;
