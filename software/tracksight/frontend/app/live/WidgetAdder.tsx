"use client";

import { useState } from "react";

import { PlusButton } from "@/components/icons/PlusButton";
import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { useWidgetManager } from "@/components/widgets/WidgetManagerContext";

const DEFAULT_WIDGET_OPTIONS = {
    height: 256,
    timeTickCount: 6,
} as const;

export function WidgetAdder() {
    const [isOpen, setIsOpen] = useState(false);
    const { appendWidget } = useWidgetManager();

    const handleAddEnum = () => {
        appendWidget({
            id: "",
            type: "enumTimeline",
            data: [],
            signals: [],
            options: {
                colorPalette: {},
                ...DEFAULT_WIDGET_OPTIONS,
            },
        });
        setIsOpen(false);
    };

    const handleAddNumerical = () => {
        appendWidget({
            id: "",
            type: "numericalGraph",
            data: [],
            signals: [],
            options: {
                colorPalette: {},
                ...DEFAULT_WIDGET_OPTIONS,
            },
        });
        setIsOpen(false);
    };

    return (
        <Popover open={isOpen} onOpenChange={setIsOpen}>
            <div className="w-full flex justify-center mt-2">
                <PopoverTrigger asChild>
                    <PlusButton className="cursor-pointer" />
                </PopoverTrigger>
            </div>
            <PopoverContent>
                <button onClick={handleAddEnum} className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700">
                    Add Enum Widget
                </button>
                <button onClick={handleAddNumerical} className="w-full text-left px-4 py-2 hover:bg-gray-100 text-sm text-gray-700 border-t border-gray-100">
                    Add Numerical Widget
                </button>
            </PopoverContent>
        </Popover>
    );
}
