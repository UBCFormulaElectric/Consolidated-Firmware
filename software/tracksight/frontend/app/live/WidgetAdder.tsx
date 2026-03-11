"use client";

import { useCallback, useState, SubmitEvent } from "react";
import { PlusButton } from "@/components/icons/PlusButton";
import { SignalType } from "@/lib/types/Signal";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog"
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover"
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import chroma from "chroma-js";
import { generateRandomColorPalette, MOCK_STATES } from "@/components/widgets/MockWidget";

function MockWidgetModalForm({ closeModal }: {
  closeModal: () => void;
}) {
  const { appendWidget: onAddWidget } = useWidgetManager();
  const [mockName, setMockName] = useState("Mock Signal");
  const [mockType, setMockType] = useState<SignalType>(SignalType.NUMERICAL);
  const [mockDelay, setMockDelay] = useState(100);
  const [mockMin, setMockMin] = useState<number>(-10);
  const [mockMax, setMockMax] = useState<number>(10);

  const handleMockSubmit = (e: SubmitEvent) => {
    e.preventDefault();
    if (mockType === SignalType.ENUM) {
      onAddWidget({
        type: SignalType.ENUM,
        signals: [{
          signal_name: mockName,
          delay: mockDelay,
          initialPoints: 0,
          options: {
            colorPalette: generateRandomColorPalette(MOCK_STATES.length), // hardcoding this should be fine since it's only for mock data?
          },
          color: chroma.random()
        }],
        id: "",
        mock: true,
      });
    } else if (mockType === SignalType.NUMERICAL) {
      onAddWidget({
        type: SignalType.NUMERICAL,
        signals: [{
          signal_name: mockName,
          delay: mockDelay,
          min: mockMin,
          max: mockMax,
          color: chroma.random()
        }],
        id: "",
        mock: true,
      });
    } else { }

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
            onChange={(e) => setMockType(e.target.value as SignalType)}
            className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
          >
            <option value={SignalType.NUMERICAL}>Numerical</option>
            <option value={SignalType.ENUM}>Enumeration</option>
          </select>
        </div>
        {mockType === SignalType.NUMERICAL && (
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

function NumericalWidgetModalForm({ closeModal }: { closeModal: () => void; }) {

  const { appendWidget: onAddWidget } = useWidgetManager();
  const [numericalSignalName, setNumericalSignalName] = useState("na"); // this should be fuzzily found in a selector or sm

  const handleNumericalSubmit = (e: SubmitEvent) => {
    e.preventDefault();

    onAddWidget({
      type: SignalType.NUMERICAL,
        signals: [{
          signal_name: numericalSignalName,
          delay: 10, // TODO : do we get this from backend? or use lookup table given signal name.
          min: 0,// TODO : do we get this from backend? or use lookup table given signal name.
          max: 100,// TODO : do we get this from backend? or use lookup table given signal name.
          color: chroma.random()
        }],
        id: numericalSignalName,
        mock: false,
      });
    closeModal();
  }

  return (
    <>
      <DialogHeader>
        <DialogTitle>New Numerical Widget</DialogTitle>
        <DialogDescription> options for configuring the mock widget </DialogDescription>
      </DialogHeader>
      <form onSubmit={handleNumericalSubmit} className="space-y-4">
        <div>
          <label className="block text-sm font-medium text-gray-700 mb-1">
            Signal Name
          </label>
          <input
            type="text"
            value={numericalSignalName}
            onChange={(e) => setNumericalSignalName(e.target.value)}
            className="w-full border rounded px-3 py-2 text-gray-900 bg-white"
            required
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

export function WidgetAdder() {
  const [isOpen, setIsOpen] = useState(false);
  const [mockModalOpen, setMockModalOpen] = useState(false);
  const [numericalModelOpen, setNumericalModelOpen] = useState(false);
  const { appendWidget: onAddWidget } = useWidgetManager();

  // TODO
  const handleAddEnum = () => {
    onAddWidget({ type: SignalType.ENUM, signals: [], id: "" });
    setIsOpen(false);
  };

  const handleAddNumerical = () => {
    setNumericalModelOpen(true);
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
          <MockWidgetModalForm closeModal={() => setMockModalOpen(false)} />
        </DialogContent>
      </Dialog>
      {/* NUMERICAL MODAL */}
      <Dialog open={numericalModelOpen} onOpenChange={setNumericalModelOpen}>
        <DialogContent>
          <NumericalWidgetModalForm closeModal={() => setNumericalModelOpen(false)} />
        </DialogContent>
      </Dialog>
    </>
  );
}

