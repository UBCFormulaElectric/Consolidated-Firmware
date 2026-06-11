"use client";

import { ChevronDown } from "lucide-react";
import React from "react";

import { cn } from "@/lib/utils";

type DropdownTriggerProps = {
    icon: React.ReactNode;
    value: React.ReactNode;
    open: boolean;
    label?: string;
    valueClassName?: string;
} & React.ComponentPropsWithoutRef<"button">;

const DropdownTrigger = React.forwardRef<HTMLButtonElement, DropdownTriggerProps>(function DropdownTrigger(
    { icon, value, open, label, valueClassName = "text-xl leading-none", className, disabled, ...buttonProps },
    ref
) {
    return (
        <button
            ref={ref}
            type="button"
            disabled={disabled}
            className={cn(
                "flex min-w-0 items-center justify-between gap-3 rounded-2xl border border-gray-200 bg-white px-4 py-3 text-left shadow-[0_12px_34px_rgba(15,23,42,0.12)] transition-colors hover:bg-gray-50",
                disabled ? "cursor-not-allowed opacity-70" : "cursor-pointer",
                className
            )}
            {...buttonProps}
        >
            <div className="flex min-w-0 items-center gap-3">
                <div className="flex size-10 shrink-0 items-center justify-center rounded-xl bg-gray-100 text-gray-900">{icon}</div>
                <div className="min-w-0">
                    {label ? <div className="text-sm font-semibold uppercase tracking-[0.16em] text-gray-500">{label}</div> : null}
                    <div className={cn("truncate font-semibold text-gray-950", valueClassName)}>{value}</div>
                </div>
            </div>
            <ChevronDown className={cn("size-6 shrink-0 text-gray-500 transition-transform", open && "rotate-180")} strokeWidth={2.25} />
        </button>
    );
});

export default DropdownTrigger;
