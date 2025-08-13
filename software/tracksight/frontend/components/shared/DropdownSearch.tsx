"use client";

import { useSignals } from "@/hooks/SignalContext";
import { X } from "lucide-react";
import React, { useMemo } from "react";
import { PlusButton } from "./PlusButton";
import { SignalType } from "@/hooks/SignalConfig";

// Type for each row's state
export interface RowItem {
  isOpen: boolean;
  searchTerm: string;
  selectedSignal?: string;
  hasCreatedComponent?: boolean;
}

interface RowEditorProps {
  index: number;
  row: RowItem;
  onToggle: (idx: number) => void;
  onSearch: (idx: number, term: string) => void;
  onSelect: (idx: number, opt: string) => void;
  onCreateComponent?: (idx: number, signalName: string) => void;
  onDeleteRow?: (idx: number) => void;
}

export const RowEditor: React.FC<RowEditorProps> = ({
  index,
  row,
  onToggle,
  onSearch,
  onSelect,
  onCreateComponent,
  onDeleteRow,
}) => {
  const { subscribeToSignal } = useSignals();

  const availableSignals: any[] = useMemo(() => [], []);
  const isLoadingSignals = false;

  // Filter available signals based on search term
  const filteredSignals = availableSignals.filter(
    (signal) =>
      (signal.name && signal.name.toLowerCase().includes(row.searchTerm.toLowerCase())) ||
      (signal.msg_name && signal.msg_name.toLowerCase().includes(row.searchTerm.toLowerCase())) ||
      (!isNaN(Number(row.searchTerm)) && signal.msg_id === Number(row.searchTerm))
  );

  const handleSignalSelect = (signalName: string) => {
    // Subscribe to the signal
    subscribeToSignal(signalName, SignalType.Any);
    // Call the parent's onSelect handler
    onSelect(index, signalName);
    // Trigger component creation if callback is provided
    if (onCreateComponent) {
      onCreateComponent(index, signalName);
    }
  };

  return (
    <div className="inline-block p-4 sticky left-2 ">
      {/* Only show plus button and delete button if component hasn't been created */}
      {!row.hasCreatedComponent && (
        <div className="flex items-center gap-2">
          <div>
            <PlusButton variant="rowSide" onClick={() => onToggle(index)} />
          </div>

          {/* Delete row button */}
          {onDeleteRow && (
            <span title="Delete this row">
              <X
                size={32}
                onClick={() => onDeleteRow(index)}
                className="bg-red-500 hover:bg-red-600 text-white rounded-full"
              />
            </span>
          )}
        </div>
      )}

      <div className="flex-1">
        {row.isOpen && (
          <div className="mt-2 bg-white dark:bg-gray-800 border border-gray-300 dark:border-gray-600 rounded shadow-lg w-64 p-2">
            <input
              type="text"
              placeholder="Search for websocket signals..."
              className="w-full mb-2 px-2 py-1 border rounded bg-white dark:bg-gray-700 text-black dark:text-white"
              value={row.searchTerm}
              onChange={(e) => onSearch(index, e.target.value)}
            />
            <div className="max-h-32 overflow-y-auto">
              {isLoadingSignals ? (
                <div className="px-2 py-1 text-gray-500">
                  Loading signals...
                </div>
              ) : filteredSignals.length === 0 ? (
                <div className="px-2 py-1 text-gray-500">No signals found</div>
              ) : (
                filteredSignals.map((signal) => (
                  <div
                    key={signal.name}
                    onClick={() => handleSignalSelect(signal.name)}
                    className="px-2 py-1 hover:bg-gray-100 dark:hover:bg-gray-700 cursor-pointer text-black dark:text-white"
                  >
                    <div className="font-medium">{signal.name}</div>
                    <div className="text-xs text-gray-500 dark:text-gray-400">
                      {signal.unit}
                    </div>
                  </div>
                ))
              )}
            </div>
          </div>
        )}
      </div>
    </div>
  );
};
