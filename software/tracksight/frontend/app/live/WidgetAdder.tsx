"use client";

import { useCallback, useState, SubmitEvent } from "react";
import { PlusButton } from "@/components/icons/PlusButton";
import { SignalType } from "@/lib/types/Signal";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog"
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover"
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import chroma from "chroma-js";
import { generateRandomColorPalette, MOCK_STATES } from "@/components/widgets/MockWidget";

export function WidgetAdder() {
  const [isOpen, setIsOpen] = useState(false);
  const [mockModalOpen, setMockModalOpen] = useState(false);
  const { appendWidget: onAddWidget } = useWidgetManager();

  // TODO
  const handleAddEnum = () => {
    onAddWidget({
      type: SignalType.ENUM, signals: [{
        signal_name: Math.random().toString(),
        delay: 100,
        initialPoints: 0,
        options: {
          colorPalette: generateRandomColorPalette(MOCK_STATES.length),
        },
        color: chroma.random(),
        type: SignalType.ENUM,
      }], id: Math.random().toString(),
    });
    setIsOpen(false);
  };

  const handleAddNumerical = () => {
    onAddWidget({
      type: SignalType.NUMERICAL, id: Math.random().toString(), signals: [{
        signal_name: Math.random().toString(),
        delay: 100,
        min: 0,
        max: 100,
        color: chroma.random(),
        type: SignalType.NUMERICAL,
      }]
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
    </>
  );
}

