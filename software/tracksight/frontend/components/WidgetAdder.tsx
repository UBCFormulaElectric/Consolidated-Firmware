"use client";

import React, { useState, useRef, useEffect } from "react";
import { PlusButton } from "./PlusButton";
import { SignalType } from "@/lib/types/Signal";
import { WidgetData, MockGraphConfig } from "@/lib/types/Widget";

interface WidgetAdderProps {
  onAddWidget: (widget: WidgetData) => void;
}

export const WidgetAdder: React.FC<WidgetAdderProps> = ({ onAddWidget }) => {
  const [isOpen, setIsOpen] = useState(false);
  const [showMockModal, setShowMockModal] = useState(false);
  const menuRef = useRef<HTMLDivElement>(null);

  const [mockName, setMockName] = useState("Mock Signal");
  const [mockType, setMockType] = useState<"numerical" | "enumeration">("numerical");
  const [mockDelay, setMockDelay] = useState(100);

  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      if (menuRef.current && !menuRef.current.contains(event.target as Node)) {
        setIsOpen(false);
      }
    };
    document.addEventListener("mousedown", handleClickOutside);
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, []);

  const handleAddEnum = () => {
    onAddWidget({
      type: SignalType.ENUM,
      signal: null,
      options: {
        colorPalette: ["#FF637E", "#FFB86A", "#05DF72", "#51A2FF"],
      },
      id: "", 
    });
    setIsOpen(false);
  };

  const handleAddNumerical = () => {
    onAddWidget({
      type: SignalType.NUMERICAL,
      signals: [],
      id: "", 
    });
    setIsOpen(false);
  };

  const handleMockSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    const config: MockGraphConfig = {
      signalName: mockName,
      type: mockType,
      delay: mockDelay,
      initialPoints: 0,
    };
    onAddWidget({
      type: SignalType.MOCK,
      configs: [config],
      id: "", 
    });
    setShowMockModal(false);
    setIsOpen(false);
    setMockName("Mock Signal");
    setMockType("numerical");
    setMockDelay(100);
  };

  return (
    <div className="sticky inline-block left-[50vw] z-50 mb-4">
      <div className="relative inline-block" ref={menuRef}>
        <div onClick={() => setIsOpen(!isOpen)} className="cursor-pointer">
          <PlusButton />
        </div>

        {isOpen && !showMockModal && (
          <div className="absolute top-full left-1/2 -translate-x-1/2 mt-2 w-56 bg-white rounded-lg shadow-xl border border-gray-200 py-1 z-50">
            <button
              onClick={handleAddEnum}
              className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700"
            >
              Add Enum Widget
            </button>
            <button
              onClick={handleAddNumerical}
              className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100"
            >
              Add Numerical Widget
            </button>
            <button
              onClick={() => setShowMockModal(true)}
              className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100"
            >
              Add Mock Widget (Debug)
            </button>
          </div>
        )}

        {showMockModal && (
          <div
            className="fixed inset-0 bg-black/50 flex items-center justify-center z-[100]"
            onClick={(e) => {
              e.stopPropagation();
              setShowMockModal(false);
              setIsOpen(false);
            }}
          >
            <div
              className="bg-white rounded-lg shadow-xl p-6 w-96 max-w-full m-4"
              onClick={(e) => e.stopPropagation()}
            >
              <h3 className="text-lg font-bold mb-4">Configure Mock Widget</h3>
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
                    onChange={(e) => setMockType(e.target.value as "numerical" | "enumeration")}
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                  >
                    <option value="numerical">Numerical</option>
                    <option value="enumeration">Enumeration</option>
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
                    min="1"
                    max="5000"
                    className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
                  />
                </div>
                <div className="flex justify-end gap-2 pt-2">
                  <button
                    type="button"
                    onClick={() => {
                      setShowMockModal(false);
                      setIsOpen(false);
                    }}
                    className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded"
                  >
                    Cancel
                  </button>
                  <button
                    type="submit"
                    className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded"
                  >
                    Create Widget
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

