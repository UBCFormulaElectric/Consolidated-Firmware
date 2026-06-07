"use client";

import { useState } from "react";

import { CardSim, FileCog } from "lucide-react";
import useFormatSDCard from "@/lib/mutations/useFormatSDCard";
import AlertModal from "../common/AlertModal";

type SDCardPickerProps = {
  selected: string | null;
  onSelect: (sdCard: string) => void;
  sdCards: string[] | undefined;
  isLoading: boolean;
  error: unknown;
};

type FormatSDCardButtonProps = {
  sdCard?: string;
};

const FormatSDCardButton = (props: FormatSDCardButtonProps) => {
  const { sdCard } = props;

  const [hasConfirmed, setHasConfirmed] = useState(false);
  const [confirmationTimeout, setConfirmationTimeout] = useState<NodeJS.Timeout | null>(null);

  const formatSDCardMutation = useFormatSDCard();

  const onClick = () => {
    if (!sdCard) return;

    formatSDCardMutation.mutate({ drive: sdCard });
  };

  return (
    <>
      <button
        onClick={() => {
          if (!hasConfirmed) {
            setHasConfirmed(true);

            const timeout = setTimeout(() => {
              setHasConfirmed(false);
            }, 5000);

            setConfirmationTimeout(timeout);

            return;
          }
          
          setConfirmationTimeout((prev) => {
            if (prev) clearTimeout(prev);

            return null;
          });

          onClick();
          setHasConfirmed(false);
        }}
        className={`rounded px-4 py-4 ml-auto text-left flex gap-2 items-center transition-colors hover:cursor-pointer border ${hasConfirmed ? "bg-red-100 border-red-500 text-red-500" : "bg-red-100/10 border-red-500 text-red-500"}`}
      >
        <FileCog className="mb-1" />
        <div className="flex flex-col leading-4">
          {
            hasConfirmed
              ? (
                <>
                  <span className="font-semibold text-sm">Confirm Formatting</span>
                  <span className="text-xs font-mono">{sdCard}</span>
                </>
              )
              : (
                <>
                  <span className="font-semibold text-sm">Format SD Card</span>
                  <span className="text-xs font-mono">{sdCard}</span>
                </>
              )
          }
        </div>
      </button>
      {
        formatSDCardMutation.isError && (
          <AlertModal
            title="Error Formatting SD Card"
            errorMessage={formatSDCardMutation.error instanceof Error ? formatSDCardMutation.error.message : "An unknown error occurred."}
            onDismiss={() => {
              formatSDCardMutation.reset();
            }}
            options={[
              {
                label: "Okay",
                style: "default",
                onClick: () => {
                  formatSDCardMutation.reset();
                },
              },
            ]}
          />
        )
      }
    </>
  );
}

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

      <FormatSDCardButton
        sdCard={selected || undefined}
      />
    </div>
  );
};

export default SDCardPicker;
