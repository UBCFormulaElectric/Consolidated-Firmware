"use client";

import { X } from "lucide-react";
import React, { useMemo, useState, useEffect, useRef, useCallback } from "react";
import { PlusButton } from "@/components/icons/PlusButton";
import { DEBUG, BACKEND_URL, SignalMeta } from "@/lib/SignalConfig";

// Internal extension with pre-lowercased fields to avoid per-keystroke toLowerCase allocations
interface SignalMetaWithLC extends SignalMeta {
  lcName: string;
  lcMsgName: string;
}

// Type for each row's state
export interface RowItem {
  isOpen: boolean;
  selectedSignal?: string;
  hasCreatedComponent?: boolean;
}

interface RowEditorProps {
  index: number;
  row: RowItem;
  onToggle: (idx: number) => void;
  onSelect: (idx: number, opt: string) => void;
  onCreateComponent?: (idx: number, signalName: string) => void;
  onDeleteRow?: (idx: number) => void;
}

const RowEditorInner: React.FC<RowEditorProps> = ({
  index,
  row,
  onToggle,
  onSelect,
  onCreateComponent,
  onDeleteRow,
}) => {
  // LOCAL STATE (one-time fetch on first open for this row instance)
  const [signals, setSignals] = useState<SignalMetaWithLC[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [hasFetched, setHasFetched] = useState(false);
  const abortRef = useRef<AbortController | null>(null);
  // Local search term (removed from parent to avoid parent re-renders per keystroke)
  const [inputTerm, setInputTerm] = useState("");
  const [debouncedTerm, setDebouncedTerm] = useState("");
  useEffect(() => {
    const t = setTimeout(() => setDebouncedTerm(inputTerm), 150);
    return () => clearTimeout(t);
  }, [inputTerm]);
  // Reset local input when dropdown is newly opened
  useEffect(() => {
    if (row.isOpen === true) {
      setInputTerm("");
      setDebouncedTerm("");
    }
  }, [row.isOpen]);

  const fetchSignals = async () => {
    if (loading) return;
    setLoading(true);
    setError(null);
    const ac = new AbortController();
    abortRef.current = ac;
    try {
      DEBUG && console.log("[DropdownSearch] Fetching signals from backend /signal");
      const res = await fetch(`${BACKEND_URL}/signal`, { signal: ac.signal });
      if (!res.ok) throw new Error(`Failed to fetch signals (${res.status})`);
      const raw = await res.json();
      const mapped: SignalMetaWithLC[] = (raw || [])
        .filter((s: any) => s && s.name)
        .map((s: any) => {
          const name: string = s.name;
          const msgName: string = s.msg_name ?? "";
          return {
            name,
            unit: s.unit ?? "",
            cycle_time_ms: s.cycle_time_ms,
            msg_id: s.id,
            msg_name: msgName,
            enum: s.enum,
            lcName: name.toLowerCase(),
            lcMsgName: msgName.toLowerCase(),
          };
        });
      setSignals(mapped);
      setHasFetched(true);
      DEBUG && console.log(`[DropdownSearch] Loaded ${mapped.length} signals`);
    } catch (e: any) {
      if (e.name !== "AbortError") {
        setError(e.message || "Unknown error");
        DEBUG && console.warn("[DropdownSearch] Fetch error", e);
      }
    } finally {
      setLoading(false);
    }
  };

  // Trigger fetch only once when dropdown first opens
  useEffect(() => {
    if (row.isOpen && !hasFetched) {
      fetchSignals();
    }
    return () => abortRef.current?.abort();
  }, [row.isOpen, hasFetched]);

  // Filter signals based on debounced search term
  const filteredSignals = useMemo(() => {
    const term = debouncedTerm.trim().toLowerCase();
    if (!term) return signals;
    const numeric = !isNaN(Number(term)) ? Number(term) : null;
    return signals.filter(
      (s) =>
        s.lcName.includes(term) ||
        s.lcMsgName.includes(term) ||
        (numeric !== null && s.msg_id === numeric)
    );
  }, [signals, debouncedTerm]);

  const handleSignalSelect = useCallback((signalName: string) => {
    DEBUG && console.log(`[ui] RowEditor selecting ${signalName} -> create component (component will subscribe)`);
    // Call the parent's onSelect handler
    onSelect(index, signalName);
    // Trigger component creation if callback is provided
    if (onCreateComponent) {
      DEBUG && console.log(`[ui] RowEditor creating component for ${signalName} (row ${index})`);
      onCreateComponent(index, signalName);
    }
  }, [index, onSelect, onCreateComponent]);

  return (
    <div className="inline-block p-4 sticky left-2 ">
      {/* Only show plus button and delete button if component hasn't been created */}
      {!row.hasCreatedComponent && (
        <div className="flex items-center gap-2">
          <div>
            <PlusButton onClick={() => onToggle(index)} />
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
              placeholder="Search for signals..."
              className="w-full mb-2 px-2 py-1 border rounded bg-white dark:bg-gray-700 text-black dark:text-white"
              value={inputTerm}
              onChange={(e) => setInputTerm(e.target.value)}
            />
            <div className="max-h-32 overflow-y-auto">
              {loading ? (
                <div className="px-2 py-1 text-gray-500">Loading signals...</div>
              ) : error ? (
                <div className="px-2 py-1 text-red-600 text-xs flex flex-col gap-1">
                  <span>Error: {error}</span>
                  <button
                    onClick={() => fetchSignals()}
                    className="border px-2 py-0.5 rounded text-xs hover:bg-gray-100 dark:hover:bg-gray-700"
                  >Retry</button>
                </div>
              ) : !hasFetched ? (
                <div className="px-2 py-1 text-gray-500 text-xs">Open to load signals...</div>
              ) : signals.length === 0 ? (
                <div className="px-2 py-1 text-gray-500 text-xs">No signals fetched</div>
              ) : filteredSignals.length === 0 ? (
                <div className="px-2 py-1 text-gray-500 text-xs">No matches</div>
              ) : (
                filteredSignals.map((signal) => (
                  <div
                    key={signal.name}
                    onClick={() => handleSignalSelect(signal.name)}
                    className="px-2 py-1 hover:bg-gray-100 dark:hover:bg-gray-700 cursor-pointer text-black dark:text-white"
                  >
                    <div className="font-medium">{signal.name}</div>
                    <div className="text-xs text-gray-500 dark:text-gray-400">{signal.unit}</div>
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

// Memoize to skip re-render if parent updates unrelated state
export const RowEditor = React.memo(RowEditorInner);
