"use client";

import { useCallback, useState } from "react";
import { PlusButton } from "@/components/icons/PlusButton";
import { SignalType } from "@/lib/types/Signal";
import { WidgetData, MockGraphConfig, MockSignalType } from "@/lib/types/Widget";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog"
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover"

function MockModal({ closeModal, onAddWidget }: {
  closeModal: () => void;
  onAddWidget: (widget: WidgetData) => void;
}) {
  const [mockName, setMockName] = useState("Mock Signal");
  const [mockType, setMockType] = useState<MockSignalType>(MockSignalType.Numerical);
  const [mockDelay, setMockDelay] = useState(100);
  const [mockMin, setMockMin] = useState<number>(-10);
  const [mockMax, setMockMax] = useState<number>(10);

  const handleMockSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    const config: MockGraphConfig = {
      signalName: mockName,
      type: mockType,
      delay: mockDelay,
      initialPoints: 0,
      min: mockType === MockSignalType.Numerical ? mockMin : undefined,
      max: mockType === MockSignalType.Numerical ? mockMax : undefined,
    };
    onAddWidget({
      type: SignalType.MOCK,
      configs: [config],
      id: "",
    });
    closeModal();
  };
  return (
    <>
      <DialogHeader>
        <DialogTitle>Configure Mock Widget</DialogTitle>
        <DialogDescription> options for configuring the mock widget </DialogDescription>
      </DialogHeader>
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
            onChange={(e) => setMockType(e.target.value as MockSignalType)}
            className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
          >
            <option value={MockSignalType.Numerical}>Numerical</option>
            <option value={MockSignalType.Enumeration}>Enumeration</option>
          </select>
        </div>
        {mockType === MockSignalType.Numerical && (
          <div className="flex gap-4">
            <div className="flex-1">
              <label className="block text-sm font-medium text-gray-700 mb-1">
                Min Value
              </label>
              <input
                type="number"
                value={mockMin}
                onChange={(e) => setMockMin(Number(e.target.value))}
                className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
              />
            </div>
            <div className="flex-1">
              <label className="block text-sm font-medium text-gray-700 mb-1">
                Max Value
              </label>
              <input
                type="number"
                value={mockMax}
                onChange={(e) => setMockMax(Number(e.target.value))}
                className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
              />
            </div>
          </div>
        )}
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
          <button type="button" onClick={closeModal} className="px-4 py-2 text-gray-600 hover:bg-gray-100 rounded cursor-pointer">
            Cancel
          </button>
          <button type="submit" className="px-4 py-2 bg-blue-600 text-white hover:bg-blue-700 rounded">
            Create Widget
          </button>
        </div>
      </form>
    </>
  )
}

export function WidgetAdder({ onAddWidget }: {
  onAddWidget: (widget: WidgetData) => void;
}) {
  const [isOpen, setIsOpen] = useState(false);
  const [mockModalOpen, setMockModalOpen] = useState(false);

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

  const handleAddMock = useCallback(() => {
    setIsOpen(false);
    setMockModalOpen(true);
  }, [setIsOpen, setMockModalOpen]);

  return (
    <>
      {/* Default Popover */}
      <Popover open={isOpen} onOpenChange={setIsOpen}>
        <div className="w-full flex justify-center">
          <PopoverTrigger asChild>
            <PlusButton className="cursor-pointer" />
          </PopoverTrigger>
        </div>
        <PopoverContent>
          <button onClick={handleAddEnum} className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700" >
            Add Enum Widget
          </button>
          <button onClick={handleAddNumerical} className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100" >
            Add Numerical Widget
          </button>
          <button onClick={handleAddMock} className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100" >
            Add Mock Widget (Debug)
          </button>
        </PopoverContent>
      </Popover>
      {/* MOCK MODAL */}
      <Dialog open={mockModalOpen} onOpenChange={setMockModalOpen}>
        <DialogContent>
          <MockModal closeModal={() => setMockModalOpen(false)} onAddWidget={onAddWidget} />
        </DialogContent>
      </Dialog>
    </>
  );
}

