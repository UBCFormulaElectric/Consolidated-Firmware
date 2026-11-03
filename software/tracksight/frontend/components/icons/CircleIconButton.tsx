"use client";

import { LucideIcon } from "lucide-react";
import { ComponentProps } from "react";

import { cn } from "@/lib/utils";

const BUTTON_ICON_SIZE_PX = 16;
const BUTTON_ICON_STROKE_WIDTH_PX = 1.75;

export function CircleIconButton(props: ComponentProps<"button"> & { Icon: LucideIcon }) {
    const { Icon, className, ...buttonProps } = props;

    return (
        <button type="button" {...buttonProps} className={cn("flex size-8 shrink-0 cursor-pointer items-center justify-center rounded-full text-white transition-transform duration-150 hover:scale-105 active:scale-95", className)}>
            <Icon size={BUTTON_ICON_SIZE_PX} strokeWidth={BUTTON_ICON_STROKE_WIDTH_PX} absoluteStrokeWidth />
        </button>
    );
}
