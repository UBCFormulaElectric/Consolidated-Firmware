"use client";

import React, { useState, useRef, useEffect } from "react";
import { PlusButton } from "./PlusButton";
import { SignalType } from "@/lib/SignalConfig";
import { MockGraphConfig } from "./widgets/MockGraph";

interface InsertionBarProps {
  onInsert: () => void;
  onInsertMock: (config: MockGraphConfig) => void;
}

export const InsertionBar: React.FC<InsertionBarProps> = ({
  onInsert,
  onInsertMock,
}) => {
  const [isOpen, setIsOpen] = useState(false);
  const [showMockModal, setShowMockModal] = useState(false);
  const menuRef = useRef<HTMLDivElement>(null);

  const [mockName, setMockName] = useState("Mock Signal");
  const [mockType, setMockType] = useState<SignalType>(SignalType.Numerical);
  const [mockDelay, setMockDelay] = useState(100);
  const [mockInitialPoints, setMockInitialPoints] = useState(100);

  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      if (menuRef.current && !menuRef.current.contains(event.target as Node)) {
        setIsOpen(false);
      }
    };
    document.addEventListener("mousedown", handleClickOutside);
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, []);

  const handleMockSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    onInsertMock({
      signalName: mockName,
      type: mockType,
      delay: mockDelay,
      initialPoints: mockInitialPoints,
    });
    setShowMockModal(false);
    setIsOpen(false);
  };

  return (
    <div className="sticky inline-block left-[50vw] z-50">
      <div className="relative inline-block" ref={menuRef}>
        <div onClick={() => setIsOpen(!isOpen)} className="cursor-pointer">
          <PlusButton />
        </div>

        {isOpen && !showMockModal && (
          <div className="absolute top-full left-1/2 -translate-x-1/2 mt-2 w-48 bg-white rounded-lg shadow-xl border border-gray-200 py-1 z-50">
            <button
              onClick={() => {
                onInsert();
                setIsOpen(false);
              }}
              className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700"
            >
              Add Signal Graph
            </button>
            <button
              onClick={() => setShowMockModal(true)}
              className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100"
            >
              Add Mock Graph (Debug)
            </button>
          </div>
        )}

        {showMockModal && (
          <div
            className="fixed inset-0 bg-black/50 flex items-center justify-center z-[100]"
            onClick={(e) => e.stopPropagation()}
          >
            <div className="bg-white rounded-lg shadow-xl p-6 w-96 max-w-full m-4">
              <h3 className="text-lg font-bold mb-4">Configure Mock Graph</h3>
              <form onSubmit={handleMockSubmit} className="space-y-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Signal Name
                  </label>
                  <input
                    type="text"
                    value={mockName}
                    onChange={(e) => setMockName(e.target.value)}
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                    required
                  />
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Type
                  </label>
                  <select
                    value={mockType}
                    onChange={(e) => setMockType(e.target.value as SignalType)}
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                  >
                    <option value={SignalType.Numerical}>Numerical</option>
                    <option value={SignalType.Enumeration}>Enumeration</option>
                  </select>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Update Delay (ms)
                  </label>
                  <input
                    type="number"
                    value={mockDelay}
                    onChange={(e) => setMockDelay(Number(e.target.value))}
                    min="0"
                    max="5000"
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                  />
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Initial Points
                  </label>
                  <input
                    type="number"
                    value={mockInitialPoints}
                    onChange={(e) =>
                      setMockInitialPoints(Number(e.target.value))
                    }
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                  />
                </div>
                <div className="flex justify-end gap-2 pt-2">
                  <button
                    type="button"
                    onClick={() => setShowMockModal(false)}
                    className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded"
                  >
                    Cancel
                  </button>
                  <button
                    type="submit"
                    className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded"
                  >
                    Create Graph
                  </button>
                </div>
              </form>
            </div>
          </div>
        )}
      </div>
    </div>
  );
};
