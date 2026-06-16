"use client";

import { Check, ChevronDown, Globe } from "lucide-react";
import { useState } from "react";

import { Popover, PopoverContent, PopoverTrigger } from "@/components/ui/popover";
import { TIMEZONE_OPTIONS, useTimezone } from "@/lib/contexts/TimezoneContext";
import { cn } from "@/lib/utils";

export function TimezoneSelector() {
    const { timezone, setTimezone } = useTimezone();
    const [open, setOpen] = useState(false);

    const activeOption = TIMEZONE_OPTIONS.find((o) => o.value === timezone) || TIMEZONE_OPTIONS[0];

    return (
        <Popover open={open} onOpenChange={setOpen}>
            <PopoverTrigger asChild>
                <button type="button" className="flex items-center gap-2 rounded border border-black bg-white py-1.5 pl-3 pr-2 text-left transition-colors hover:cursor-pointer hover:border-black/75">
                    <span className="flex items-center gap-1.5 text-[0.6rem] font-semibold uppercase tracking-wide text-gray-500">
                        <Globe className="size-3" />
                        Timezone
                    </span>
                    <span className="max-w-44 truncate text-sm font-semibold text-gray-900">{activeOption.label}</span>
                    <ChevronDown className="size-4 shrink-0 text-gray-500" strokeWidth={2.25} />
                </button>
            </PopoverTrigger>
            <PopoverContent className="w-56 p-1 z-50 bg-white border border-black shadow-lg rounded-md" align="end" sideOffset={4}>
                <div className="flex flex-col">
                    {TIMEZONE_OPTIONS.map((option) => {
                        const isSelected = option.value === timezone;
                        return (
                            <button
                                key={option.value}
                                onClick={() => {
                                    setTimezone(option.value);
                                    setOpen(false);
                                }}
                                className={cn("flex items-center gap-2 rounded px-3 py-2 text-sm transition-colors hover:cursor-pointer", isSelected ? "bg-blue-100 text-blue-600 font-semibold" : "text-gray-800 hover:bg-gray-100")}
                            >
                                <span className="flex-1 text-left">{option.label}</span>
                                {isSelected && <Check className="size-4 text-blue-600" strokeWidth={2.5} />}
                            </button>
                        );
                    })}
                </div>
            </PopoverContent>
        </Popover>
    );
}
