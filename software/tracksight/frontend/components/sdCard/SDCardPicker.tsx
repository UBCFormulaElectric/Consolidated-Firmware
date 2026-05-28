"use client";

import { CardSim } from "lucide-react";

type SDCardPickerProps = {
  selected: string | null;
  onSelect: (sdCard: string) => void;
  sdCards: string[] | undefined;
  isLoading: boolean;
  error: unknown;
};

const SDCardPickerCardSkeleton = () => {
  return (
    <div className="rounded px-4 py-4 text-left flex gap-2 items-center transition-colors border border-gray-300 bg-gray-100 animate-pulse">
      <CardSim className="mb-1 text-gray-300 size-6" />
      <div className="flex flex-col leading-4">
        <span className="font-semibold text-sm text-gray-300">SD Card</span>
        <span className="text-xs font-mono text-gray-300">Loading...</span>
      </div>
    </div>
  );
};

const SDCardPickerSkeleton = () => {
  return (
    <div className="flex flex-row gap-3">
      <SDCardPickerCardSkeleton />
      <SDCardPickerCardSkeleton />
    </div>
  );
};

const SDCardPicker = (props: SDCardPickerProps) => {
  const { selected, onSelect, sdCards, isLoading, error } = props;

  if (isLoading) {
    return <SDCardPickerSkeleton />;
  }

  if (error || !sdCards) {
    return <div className="text-sm text-red-500">Failed to list SD cards.</div>;
  }

  if (sdCards.length === 0) {
    return <div className="text-sm text-gray-400">No SD cards detected. Please insert an SD card.</div>;
  }

  return (
    <div className="flex flex-row gap-3">
      {sdCards.map((sdCard) => {
        const isSelected = selected === sdCard;

        return (
          <button
            key={sdCard}
            onClick={() => onSelect(sdCard)}
            className={`rounded px-4 py-4 text-left flex gap-2 items-center transition-colors hover:cursor-pointer ${
              isSelected
                ? "border-blue-500 border bg-blue-100"
                : "border-black hover:border-black/75 border"
            }`}
          >
            <CardSim className={`mb-1 ${isSelected ? "text-blue-500" : ""}`} />
            <div className="flex flex-col leading-4">
              <span className="font-semibold text-sm">SD Card</span>
              <span className="text-xs font-mono">{sdCard}</span>
            </div>
          </button>
        );
      })}
    </div>
  );
};

export default SDCardPicker;
