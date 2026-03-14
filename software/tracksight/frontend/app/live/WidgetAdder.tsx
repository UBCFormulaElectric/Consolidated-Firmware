"use client";

import { useCallback, useState, SubmitEvent } from "react";
import { PlusButton } from "@/components/icons/PlusButton";
import { SignalType } from "@/lib/types/Signal";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover"
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";
import { generateRandomColorPalette, MOCK_STATES } from "@/components/widgets/MockWidget";

export function WidgetAdder() {
  const [isOpen, setIsOpen] = useState(false);
  const [mockModalOpen, setMockModalOpen] = useState(false);
  const { appendWidget: onAddWidget } = useWidgetManager();

  // TODO
  const handleAddEnum = () => {
    const signalName = Math.random().toString();

    onAddWidget({
      type: "enumTimeline",
      data: [],
      options: {
        colorPalette: {
          [signalName]: generateRandomColorPalette(MOCK_STATES.length)
        },
        height: 256,
        timeTickCount: 6,
      },
      signals: [{
        id: Math.random(),
        name: signalName,
        cycle_time_ms: 100,
        enum: {
          items: Object.fromEntries(MOCK_STATES.map((state, i) => [i, state])),
          name: "Mock Enum",
        },
        max_val: MOCK_STATES.length,
        min_val: 0,
        msg_name: "Mock Enum Signal",
        tx_node: "Mock Node",
        type: SignalType.ENUM
      }], id: Math.random().toString(),
    });
    setIsOpen(false);
  };

  const handleAddNumerical = () => {
    const signalName = Math.random().toString();

    onAddWidget({
      id: Math.random().toString(),
      type: "numericalGraph",
      data: [],
      options: {
        colorPalette: {
          [signalName]: generateRandomColorPalette(1)[0]
        },
        height: 256,
        timeTickCount: 6,
      },
      signals: [{
        id: Math.random(),
        name: signalName,
        cycle_time_ms: 100,
        min_val: 0,
        max_val: 100,
        type: SignalType.NUMERICAL,
        tx_node: "Mock Node",
        msg_name: "Mock Numerical Signal",
        unit: "units",
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

